#include "adminhomewindow.h"
#include "ui_adminhomewindow.h"

adminHomeWindow::adminHomeWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::adminHomeWindow)
{
    ui->setupUi(this);

    QPixmap pixmap(":/adminshouye.jpg");
    QPixmap scaledPixmap = pixmap.scaled(ui->label->size(),
                                         Qt::KeepAspectRatio,
                                         Qt::SmoothTransformation);
    ui->label->setPixmap(scaledPixmap);
}

adminHomeWindow::~adminHomeWindow()
{
    delete ui;
}
