#include "orderwindow.h"
#include "ui_orderwindow.h"
#include <QPalette>
#include <QPixmap>
#include <QResource>
#include <QLabel>
#include <QPixmap>
#include <QDebug>

orderwindow::orderwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::order_2)
{
    ui->setupUi(this);

    ui->centralwidget->setStyleSheet(
        "QWidget {"
        "   background-image: url(:/n6.png);"
        "   background-repeat: no-repeat;"
        "   background-position: center;"
        "   background-size: cover;" // 让图片自适应填充
        "}"
        );



    // 设置按钮图标
    QPixmap iconPixmap(":/order.png");
    ui->order->setIcon(iconPixmap);

    QPixmap iconPixmap2(":/deletedorder.png");
    ui->order2->setIcon(iconPixmap2);

    QPixmap iconPixmap3(":/unpayorder.png");
    ui->order3->setIcon(iconPixmap3);

    QPixmap iconPixmap4(":/bag.png");
    ui->order4->setIcon(iconPixmap4);

    QSize iconSize(30, 30);
    ui->order->setIconSize(iconSize);
    ui->order2->setIconSize(iconSize);
    ui->order3->setIconSize(iconSize);
    ui->order4->setIconSize(iconSize);

    // 调整按钮文字样式
    QString buttonStyle = "QPushButton {"
                          "   font-size: 16px;"  // 字体大小
                          "   font-weight: bold;" // 字体加粗
                          "   background-color: white;"       // 设置按钮背景为白色，突出图标
                          "   border: 2px solid black;"       // 添加黑色边框，模拟加粗效果
                          "   border-radius: 10px;"
                          "}";
    ui->order->setStyleSheet(buttonStyle);
    ui->order2->setStyleSheet(buttonStyle);
    ui->order3->setStyleSheet(buttonStyle);
    ui->order4->setStyleSheet(buttonStyle);
}

orderwindow::~orderwindow()
{
    delete ui;
}
