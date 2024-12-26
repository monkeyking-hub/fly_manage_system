#include "boarderitem.h"
#include "ui_boarderitem.h"
int boarderItem:: num=1;
boarderItem::boarderItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::boarderItem)
{
    ui->setupUi(this);
    ui->lbl_nb->setText(QString::number(static_cast<int>(num++)));
}

boarderItem::~boarderItem()
{
    delete ui;
}
