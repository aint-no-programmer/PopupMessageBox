#include "EventLoop.h"
#include <QDebug>

PopMsgBox::EventLoop::EventLoop(PopupWindowContainer* t_popupWindowContainer, QObject* t_parent):
	QObject(t_parent),
	m_popupWindowContainer(t_popupWindowContainer)
{
	m_onMotion = t_popupWindowContainer->isOnMotion();
	connect(t_popupWindowContainer, &PopupWindowContainer::s_onMotion, [this](bool t_onMotion)
	{
		QMutexLocker lock(&m_mutex_2);
		m_onMotion = t_onMotion;
		QString str = m_onMotion ? "true" : "false";
		qDebug() << "PopupWindowContainer::s_onMotion -> " + str;
		if (!m_onMotion) m_condVar_2.notify_one();
	});
	connect(this, &EventLoop::s_pushMessage, t_popupWindowContainer, [this, t_popupWindowContainer](const QString& t_title, const QString& t_message, const QColor& t_color)
	{
		t_popupWindowContainer->pushMessage(t_title, t_message, t_color);
	}, Qt::BlockingQueuedConnection);

	m_running.store(true);
	m_stopFuture = QtConcurrent::run(this, &EventLoop::threadFunc);
}

PopMsgBox::EventLoop::~EventLoop() noexcept
{
	enqueue([this]
	{
		m_running.store(false);
	});

	m_stopFuture.waitForFinished();

	qDebug() << "~EventLoop()";
}

void PopMsgBox::EventLoop::enqueueMessage(const QString& t_title, const QString& t_message, const QColor& t_color)
{
	enqueue([this, t_title, t_message, t_color]()
		{
			emit s_pushMessage(t_title, t_message, t_color);
		}
	);
}

void PopMsgBox::EventLoop::enqueue(callable_t&& callable) noexcept
{
	{
		QMutexLocker lock(&m_mutex);
		m_writeBuffer.emplace_back(std::move(callable));
	}

	m_condVar.notify_one();
}

void PopMsgBox::EventLoop::threadFunc() noexcept
{
	std::vector<callable_t> readBuffer;
		
	while (m_running.load())
	{
		{
			QMutexLocker lock(&m_mutex);
			if (m_writeBuffer.empty())
			{
				m_condVar.wait(lock.mutex());
			}

			std::swap(readBuffer, m_writeBuffer);
		}

		for (callable_t& func : readBuffer)
		{
			{
				QMutexLocker lock(&m_mutex_2);
				if (m_onMotion)
				{
					qDebug() << "waiting...";
					m_condVar_2.wait(lock.mutex());
				}
			}

			qDebug() << "before func()";
			func();
			qDebug() << "after func()";
		}
			
		readBuffer.clear();
	}
}
