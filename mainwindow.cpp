#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "InterfaceManager.h"
#include "betawindow.h"
#include <QStackedWidget>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    // 设置堆叠控件为中央部件,仅仅在幕布类(即当前MainWindow要执行
    setCentralWidget(InterfaceManager::instance()->m_stackedWidget);

}

MainWindow::~MainWindow()
{
    InterfaceManager::instance()->unregisterPage("/MainWindow/Beta");
    delete ui;
}

