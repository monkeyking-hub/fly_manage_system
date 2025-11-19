#include "mainwindow.h"
#include "interfacemanager.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 设置堆叠控件为中央部件,仅仅在幕布类(即当前MainWindow)要执行
    setWindowTitle("云程");
    setFixedSize(1300, 700);
    setCentralWidget(InterfaceManager::instance()->m_stackedWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
