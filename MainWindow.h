#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "PopupWindowContainer.h"
#include <QVector>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
	PopupWindowContainer m_popupWindowContainer;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_pushButton_clicked();
private:
    QColor m_color{0,0,0,180};
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
