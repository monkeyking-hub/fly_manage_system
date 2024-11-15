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


}

BetaWindow::~BetaWindow()
{


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

