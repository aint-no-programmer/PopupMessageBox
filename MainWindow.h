#pragma once

#include <QMainWindow>

#include "PopupWindowContainer.h"
#include <QVector>

#include "EventLoop.h"
#include <QCloseEvent>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace PopMsgBox
{

class MainWindow : public QMainWindow
{
    Q_OBJECT
	PopupWindowContainer m_popupWindowContainer;
    EventLoop* m_eventLoop{nullptr};
    int counter{0};
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void closeEvent(QCloseEvent* event) override
    {
        qDebug() << "closeEvent: 1. m_eventLoop";
        m_eventLoop->deleteLater();
        event->accept();
    }
private slots:
    void on_pushButton_clicked();
private:
    QColor m_color{0,0,0,180};
    Ui::MainWindow *ui;
};

}