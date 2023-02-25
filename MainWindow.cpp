#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include <QColorDialog>

PopMsgBox::MainWindow::MainWindow(QWidget *parent)
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

    /*buttons*/
    //info
    connect(ui->infoButton, &QPushButton::clicked, this, [this](){
        PopMsgBox::instance().enqueueMessage(ui->textEdit_2->toPlainText().append(" %1").arg(m_counter++), ui->textEdit->toPlainText(), convert(MessageType::Info));
    });

    //warning
    connect(ui->warnButton, &QPushButton::clicked, this, [this](){
        PopMsgBox::instance().enqueueMessage(ui->textEdit_2->toPlainText().append(" %1").arg(m_counter++), ui->textEdit->toPlainText(), convert(MessageType::Warning));
    });

    //error
    connect(ui->errorButton, &QPushButton::clicked, this, [this](){
        PopMsgBox::instance().enqueueMessage(ui->textEdit_2->toPlainText().append(" %1").arg(m_counter++), ui->textEdit->toPlainText(), convert(MessageType::Error));
    });
}

PopMsgBox::MainWindow::~MainWindow()
{
    delete ui;
    PMB_TRACK("~MainWindow()")
}

void PopMsgBox::MainWindow::on_pushButton_clicked()
{
    PopMsgBox::instance().enqueueMessage(ui->textEdit_2->toPlainText().append(" %1").arg(m_counter++), ui->textEdit->toPlainText(), m_color);
}
