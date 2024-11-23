#include "gamawindow.h"
#include "ui_gamawindow.h"
#include <interfacemanager.h>
GamaWindow::GamaWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GamaWindow)
{
    ui->setupUi(this);
}

GamaWindow::~GamaWindow()
{
    delete ui;
}

void GamaWindow::on_toBetaButton_clicked()
{
    //点击跳转beta界面
    InterfaceManager::instance()->switchToPage("/MainWindow/Beta");
}
