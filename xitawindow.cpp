#include "xitawindow.h"
#include "ui_xitawindow.h"
#include <interfacemanager.h>
XitaWindow::XitaWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::XitaWindow)
{
    ui->setupUi(this);
}

XitaWindow::~XitaWindow()
{
    delete ui;
}

void XitaWindow::on_toBetaButton_clicked()
{
    InterfaceManager::instance()->switchToPage("/MainWindow/Beta");
}
