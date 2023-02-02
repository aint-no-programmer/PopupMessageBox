#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PopMsgBox::MainWindow w;
    w.show();
    return a.exec();
}
