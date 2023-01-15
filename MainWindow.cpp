#include "MainWindow.h"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    const auto popUp = new PopupWindow();
    popUp->setPopupText(ui->textEdit->toPlainText());

    //aintnop.todo add removing deleted <popup> elements from QVector
    for (const auto e : m_popups)
    {
        e->moveUp(popUp->height());
    }

    popUp->show();
    m_popups.push_front(popUp);
    connect(popUp, &PopupWindow::destroyed, [this, popUp](){
        m_popups.removeOne(popUp);
    });
}