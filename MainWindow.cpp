#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto setButtonColor = [this](const QColor& t_color){
        ui->pushButton_2->setStyleSheet(QString("background-color: %1").arg(t_color.name()));
    };

    setButtonColor(m_color);

    connect(ui->pushButton_2, &QPushButton::clicked, [this, setButtonColor](){
        m_color = QColorDialog::getColor(m_color);
        setButtonColor(m_color);
    });
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    const auto popUp = new PopupWindow();
    popUp->createMessage(ui->textEdit_2->toPlainText(), ui->textEdit->toPlainText(), m_color);

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
