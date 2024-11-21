#include "registerwindow.h"
#include "ui_registerwindow.h"

registerWindow::registerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::registerWindow)
{
    ui->setupUi(this);

    //设置图片
    QPixmap *pix = new QPixmap(":/girl1.png");
    QSize sz = ui->label_image->size();
    ui->label_image->setPixmap(pix->scaled(sz));

}

registerWindow::~registerWindow()
{
    delete ui;
}
