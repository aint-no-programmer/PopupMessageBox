#pragma once

#include <QMainWindow>

#include "PopupWindowContainer.h"
#include <QVector>

#include "EventLoop.h"

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

private slots:
    void on_pushButton_clicked();
private:
    QColor m_color{0,0,0,180};
    Ui::MainWindow *ui;
};

}