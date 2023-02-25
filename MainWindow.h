#pragma once

#include <QMainWindow>

#include "PopupWindowContainer.h"
#include <QVector>

#include "EventLoop.h"
#include <QCloseEvent>
#include <QApplication>
#include "PopMsgBox.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace PopMsgBox
{

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
    QColor m_color{0,0,0,180};
    Ui::MainWindow *ui;
};

}