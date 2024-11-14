#include "mainwindow.h"
#include <QPushButton>
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //表示一开始就进入Beta界面,即幕布第一张就是Beta
    InterfaceManager::instance()->switchToPage("/MainWindow/Beta");


    return a.exec();
}
