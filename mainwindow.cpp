#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)//初始化父窗口,使得有那些功能
    , ui(new Ui::MainWindow)//Ui:MainWindow是设计师界面生成的类
{
    ui->setupUi(this);//这个ui就是那个生成的类的指针(Ui里面的MainWindow)
}

MainWindow::~MainWindow()
{
    delete ui;
}
