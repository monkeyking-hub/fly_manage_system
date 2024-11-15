#include "betawindow.h"
#include "InterfaceManager.h"
#include "ui_betawindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <gamawindow.h>
#include <xitawindow.h>
#include <yipuxilong.h>
BetaWindow::BetaWindow( QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BetaWindow)
{
     ui->setupUi(this);
    GamaWindow * gamawindow = new GamaWindow();
    XitaWindow * xitawindow = new XitaWindow();
    Yipuxilong * yipuxilong = new Yipuxilong();

    //注册gama界面
    InterfaceManager::instance()->registerPage("/MainWindow/Beta/Gama",gamawindow);
    //注册西塔
    InterfaceManager::instance()->registerPage("/MainWindow/Beta/Xita",xitawindow);
    //注册伊普西隆
    InterfaceManager::instance()->registerPage("/MainWindow/Beta/Yipuxilong",yipuxilong);

}

BetaWindow::~BetaWindow()
{
    //析构函数内解除对Gama的注册
    InterfaceManager::instance()->unregisterPage("/MainWindow/Beta/Gama");
    InterfaceManager::instance()->unregisterPage("/MainWindow/Beta/Xita");
    InterfaceManager::instance()->unregisterPage("/MainWindow/Beta/Yipuxilong");
    delete ui;
}


void BetaWindow::on_toGamabutton_clicked()
{
    //点击跳转gama界面
    InterfaceManager::instance()->switchToPage("/MainWindow/Beta/Gama");
}


void BetaWindow::on_toXitabutton_clicked()
{
    //跳转xita界面
    InterfaceManager::instance()->switchToPage("/MainWindow/Beta/Xita");
}


void BetaWindow::on_toYipuxilongbutton_clicked()
{
    //跳转Yipuxilong
    InterfaceManager::instance()->switchToPage("/MainWindow/Beta/Yipuxilong");
}

