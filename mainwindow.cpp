#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
    const auto popUp = new PopUp();
    popUp->setPopupText(ui->textEdit->toPlainText());

    //aintnop.todo add removing deleted <popup> elements from QVector
    for (const auto e : m_popups)
    {
        e->moveUp(popUp->height());
    }

    popUp->show();
    m_popups.push_front(popUp);
    connect(popUp, &PopUp::destroyed, [this, popUp](){
        m_popups.removeOne(popUp);
    });
}
