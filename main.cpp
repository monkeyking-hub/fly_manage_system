#include <QApplication>
#include <QPushButton>
#include "mainwindow.h"
#include <system.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    System system;
    system.start(); //这里面注册窗口

    return a.exec();
}
