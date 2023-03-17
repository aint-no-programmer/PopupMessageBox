#pragma once

#include <QMainWindow>

#include "PopupWindowContainer.h"
#include <QVector>

#include "EventLoop.h"
#include <QCloseEvent>
#include <QApplication>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace pmb
{
    class PopMsgBox : public QObject
    {
        Q_OBJECT

    	QPointer<EventLoop> m_eventLoop{ nullptr };
    public:
        static PopMsgBox& init(
            int t_displayDuration = 10000,
            const QEasingCurve& t_movingCurve = QEasingCurve::InOutSine,
            int t_appearanceDuration = 1000)
        {
            return instanceImpl(t_displayDuration, t_movingCurve, t_appearanceDuration);
        }
        static PopMsgBox& instanceImpl(
            int t_displayDuration,
            const QEasingCurve& t_movingCurve,
            int t_appearanceDuration)
        {
	        static PopMsgBox instance(t_displayDuration, t_movingCurve, t_appearanceDuration);
	        return instance;
        }

        static PopMsgBox& instance()
        {
            return instanceImpl(10000, QEasingCurve::InOutSine, 1000);
        }
        void enqueueMessage(const QString& t_title, const QString& t_message, const QColor& t_color) const 
        {
            if (m_eventLoop) m_eventLoop->enqueueMessage(t_title, t_message, t_color);
        }

        void stop() const
        {
            m_eventLoop->deleteLater();
        }

        PopMsgBox(const PopMsgBox&) = delete;
        PopMsgBox(PopMsgBox&&) = delete;
        PopMsgBox& operator=(const PopMsgBox&) = delete;
        PopMsgBox& operator=(PopMsgBox&&) = delete;

    private:
        PopMsgBox(
            int t_displayDuration, 
            const QEasingCurve& t_movingCurve,
            int t_appearanceDuration)
        {
            const auto popupWindowContainer = new PopupWindowContainer(t_displayDuration, t_movingCurve, t_appearanceDuration);

            //move popupWindowContainer to main Gui thread if it is not there
            if (thread() != QApplication::instance()->thread())
        	{
	            popupWindowContainer->moveToThread(QApplication::instance()->thread());
            }

            m_eventLoop = new EventLoop(popupWindowContainer);
            connect(m_eventLoop, &EventLoop::destroyed, popupWindowContainer, &PopupWindowContainer::deleteLater, Qt::QueuedConnection);

        	const auto thread = new QThread;
            m_eventLoop->moveToThread(thread);
            connect(m_eventLoop, &EventLoop::destroyed, thread, &QThread::quit, Qt::QueuedConnection);

        	connect(thread, &QThread::finished, thread, &QThread::deleteLater);
            connect(thread, &QThread::finished, this, &PopMsgBox::s_stopped);

            thread->start();
        }
        ~PopMsgBox() override
        {
            PMB_TRACE("~PopMsgBox")
            if (m_eventLoop) m_eventLoop->deleteLater();
        }
    signals:
        void s_stopped();
    };
}