#include "betawindow.h"
#include "ui_betawindow.h"

betawindow::betawindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::betawindow)
{
    ui->setupUi(this);
}

betawindow::~betawindow()
{
    delete ui;
}
