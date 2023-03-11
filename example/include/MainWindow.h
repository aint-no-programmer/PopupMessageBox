#pragma once

#include <QMainWindow>
#include <QCloseEvent>
#include "PopMsgBox.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    int m_counter{0};
    bool m_onClosing{ false };
    enum class MessageType
    {
        Info = 0,
        Warning,
        Error,
        None = 255
    };
public:
    using PopMsgBox = PopMsgBox::PopMsgBox;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void closeEvent(QCloseEvent* event) override
    {
        if (m_onClosing)
        {
            event->accept();
            PMB_TRACE("MainWindow::close")
            return;
        }

        PMB_TRACE("popMsgBox->stop()")
    	PopMsgBox::instance().stop();
        event->ignore();
        m_onClosing = true;
        connect(&PopMsgBox::instance(), &PopMsgBox::s_stopped, this, &QMainWindow::close,Qt::QueuedConnection);

        hide();
        //connect(popMsgBox, &PopMsgBox::s_stopped, qApp, &QApplication::quit, Qt::QueuedConnection);
    }
private slots:
    void on_pushButton_clicked();
private:
    QColor convert(MessageType t_messageType) const 
    {
        switch (t_messageType)
        {
        case MessageType::Info:     return QColor(Qt::darkBlue);//QColor(0, 0, 0, 180);
        case MessageType::Warning:  return QColor(Qt::darkYellow);
        case MessageType::Error:    return QColor(Qt::darkRed);
        case MessageType::None:     //to default
        default:
        {
            //error case
            return {};  //invalid QColor
        }
        }
    }

    QColor m_color{0,0,0,180};
    Ui::MainWindow *ui;
};