#pragma once
 
#include <condition_variable>
#include <functional>
#include <future>
#include <thread>
#include <vector>

class EventLoop
{
public:
	using callable_t = std::function<void()>;
	
	EventLoop() = default;
	EventLoop(const EventLoop&) = delete;
	EventLoop(EventLoop&&) noexcept = delete;
	~EventLoop() noexcept
	{
		enqueue([this]
		{
			m_running = false;
		});
		m_thread.join();
	}
	
	EventLoop& operator= (const EventLoop&) = delete;
	EventLoop& operator= (EventLoop&&) noexcept = delete;
	
	void enqueue(callable_t&& callable) noexcept
	{
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			m_writeBuffer.emplace_back(std::move(callable));
		}
		m_condVar.notify_one();
	}
	
private:
	std::vector<callable_t> m_writeBuffer;
	std::mutex m_mutex;
	std::condition_variable m_condVar;
	bool m_running{ true };
	std::thread m_thread{ &EventLoop::threadFunc, this };
	
	void threadFunc() noexcept
	{
		std::vector<callable_t> readBuffer;
		
		while (m_running)
		{
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				m_condVar.wait(lock, [this]
				{
					return !m_writeBuffer.empty();
				});
				std::swap(readBuffer, m_writeBuffer);
			}
			
			for (callable_t& func : readBuffer)
			{
				func();
			}
			
			readBuffer.clear();
		}
	}
};