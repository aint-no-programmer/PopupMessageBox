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

namespace PopMsgBox
{
    class PopMsgBox : public QObject
    {
        Q_OBJECT

    	QPointer<EventLoop> m_eventLoop{ nullptr };
    public:
        //static PopMsgBox& instance()
        //{
        //    static PopMsgBox instance;
        //    return instance;
        //}
        void enqueueMessage(const QString& t_title, const QString& t_message, const QColor& t_color)
        {
            if (m_eventLoop) m_eventLoop->enqueueMessage(t_title, t_message, t_color);
        }

        void stop() const
        {
            m_eventLoop->deleteLater();
        }

        //PopMsgBox(const PopMsgBox&) = delete;
        //PopMsgBox(PopMsgBox&&) = delete;
        //PopMsgBox& operator=(const PopMsgBox&) = delete;
        //PopMsgBox& operator=(PopMsgBox&&) = delete;

        PopMsgBox()
        {
            const auto popupWindowContainer = new PopupWindowContainer(10000);
            m_eventLoop = new EventLoop(popupWindowContainer);
            connect(m_eventLoop, &EventLoop::destroyed, popupWindowContainer, &PopupWindowContainer::deleteLater, Qt::QueuedConnection);

        	const auto thread = new QThread;
            m_eventLoop->moveToThread(thread);
            connect(m_eventLoop, &EventLoop::destroyed, thread, &QThread::quit, Qt::QueuedConnection);

        	connect(thread, &QThread::finished, thread, &QThread::deleteLater);
            connect(thread, &QThread::finished, this, &PopMsgBox::s_stopped);

            thread->start();
        }
        //private:
        ~PopMsgBox() override
        {
            PMB_TRACK("~PopMsgBox")
            if (m_eventLoop) m_eventLoop->deleteLater();
        }
    signals:
        void s_stopped();
    };

    Q_GLOBAL_STATIC(PopMsgBox, popMsgBox)

class MainWindow : public QMainWindow
{
    Q_OBJECT
    int m_counter{0};
    bool m_onClosing{ false };
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void closeEvent(QCloseEvent* event) override
    {
        if (m_onClosing)
        {
            event->accept();
            PMB_TRACK("MainWindow::close")
            return;
        }

        PMB_TRACK("popMsgBox->stop()")
        popMsgBox->stop();
        event->ignore();
        m_onClosing = true;
        connect(popMsgBox, &PopMsgBox::s_stopped, this, &QMainWindow::close,Qt::QueuedConnection);

        hide();
        //connect(popMsgBox, &PopMsgBox::s_stopped, qApp, &QApplication::quit, Qt::QueuedConnection);
    }
private slots:
    void on_pushButton_clicked();
private:
    QColor m_color{0,0,0,180};
    Ui::MainWindow *ui;
};

}