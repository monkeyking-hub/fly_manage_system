#include "inorder.h"
#include "ui_inorder.h"

inorder::inorder(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::inorder)
{
    ui->setupUi(this);
}

inorder::~inorder()
{
    delete ui;
}
