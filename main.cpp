#include "mainwindow.h"
#include <QPushButton>
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    InterfaceManager::instance()->switchToPage("/MainWindow/Beta");


    return a.exec();
}
