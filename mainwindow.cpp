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
    m_parentName = "";
    m_name = "MainWindow";

    // 创建页面并注册
    QString fullPath = m_parentName + "/" + m_name;

    page1 = new BetaWindow(fullPath, "Page1");
    page2 = new BetaWindow(fullPath, "Page2");
    // 设置堆叠控件为中央部件
    setCentralWidget(InterfaceManager::instance()->m_stackedWidget);

    // 默认显示 page1
    //InterfaceManager::instance()->switchToPage(fullPath + "/Page1");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    // 切换到 Page2
    InterfaceManager::instance()->switchToPage("/MainWindow/Page2");
}
