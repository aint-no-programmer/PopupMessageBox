#pragma once
 
#include <condition_variable>
#include <functional>
#include <future>
#include <thread>
#include <vector>

#include <QFuture>
#include <QtConcurrent/QtConcurrent>

#include "PopupWindowContainer.h"

namespace PopMsgBox
{

/*
 * This event loop based on another that was taken from https://habr.com/ru/post/665730/
 */

class EventLoop : public QObject
{
	Q_OBJECT
public:
	using callable_t = std::function<void()>;
	
	EventLoop(PopupWindowContainer* t_popupWindowContainer, QObject* t_parent = nullptr);
	~EventLoop() noexcept override;

	void enqueueMessage(const QString& t_title, const QString& t_message, const QColor& t_color);
signals:
	void s_pushMessage(const QString& t_title, const QString& t_message, const QColor& t_color);
private:
	void enqueue(callable_t&& callable) noexcept;
	void threadFunc() noexcept;

	//deleted
	EventLoop(const EventLoop&) = delete;
	EventLoop(EventLoop&&) noexcept = delete;
	EventLoop& operator= (const EventLoop&) = delete;
	EventLoop& operator= (EventLoop&&) noexcept = delete;

	//members
	PopupWindowContainer* m_popupWindowContainer{nullptr};
	bool m_onMotion;
	std::vector<callable_t> m_writeBuffer;
	QMutex m_mutex, m_mutex_2;
	QWaitCondition m_condVar, m_condVar_2;
	std::atomic<bool> m_running;
	QFuture<void> m_stopFuture;
};

}