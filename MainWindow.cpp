#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
	m_popupWindowContainer(10000)
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

    /*buttons*/
    //info
    connect(ui->infoButton, &QPushButton::clicked, this, [this](){
        m_popupWindowContainer.pushMessage(ui->textEdit_2->toPlainText(), ui->textEdit->toPlainText(), PopupWindowContainer::MessageType::Info);
    });

    //warning
    connect(ui->warnButton, &QPushButton::clicked, this, [this](){
        m_popupWindowContainer.pushMessage(ui->textEdit_2->toPlainText(), ui->textEdit->toPlainText(), PopupWindowContainer::MessageType::Warning);
    });

    //error
    connect(ui->errorButton, &QPushButton::clicked, this, [this](){
        m_popupWindowContainer.pushMessage(ui->textEdit_2->toPlainText(), ui->textEdit->toPlainText(), PopupWindowContainer::MessageType::Error);
    });
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    m_popupWindowContainer.pushMessage(ui->textEdit_2->toPlainText(), ui->textEdit->toPlainText(), m_color);
}
