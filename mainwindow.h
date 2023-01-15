#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "popupwindow.h"
#include <QVector>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QVector<PopupWindow*> m_popups;
//    PopUp *popUp;
};
#endif // MAINWINDOW_H
