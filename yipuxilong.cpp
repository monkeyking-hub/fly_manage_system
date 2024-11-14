#include "yipuxilong.h"
#include "ui_yipuxilong.h"
#include <interfacemanager.h>
Yipuxilong::Yipuxilong(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Yipuxilong)
{
    ui->setupUi(this);
}

Yipuxilong::~Yipuxilong()
{
    delete ui;
}

void Yipuxilong::on_toBetaButton_clicked()
{
    InterfaceManager::instance()->switchToPage("/MainWindow/Beta");
}


void Yipuxilong::on_toGameButton_clicked()
{
    InterfaceManager::instance()->switchToPage("/MainWindow/Beta/Gama");
}

