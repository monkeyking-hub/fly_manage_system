#include "orderwindow.h"
#include "ui_orderwindow.h"
#include <QPalette>
#include <QPixmap>
#include <QResource>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QFontDatabase>  // 用于加载自定义字体

orderwindow::orderwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::order_2)
{
    ui->setupUi(this);

    // 确保在 .ui 文件中设置了 centralWidget 的 objectName
    ui->centralwidget->setObjectName("centralWidget");

    ui->centralwidget->setStyleSheet(
        "#centralWidget {" // 仅匹配 centralWidget 本身
        "   background-image: url(:/sky3.png);"
        "   background-repeat: no-repeat;"
        "   background-position: center;"
        "   background-size: contain;"
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
    QString buttonStyle =
        "QPushButton {"
        "   min-height: 40px;" // 最小高度设置为 50px
        "   font-size: 16px;"              // 字体大小
        "   font-weight: bold;"            // 字体加粗
        "   color: black;"                 // 文字颜色
        "   background: transparent;"      // 按钮背景透明
        "   border: 2px solid black;"      // 黑色边框
        "   border-radius: 10px;"          // 按钮圆角
        "   padding: 5px;"                 // 内边距
        "   transition: all 0.2s ease;"    // 平滑过渡效果（动画）
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(0, 0, 255, 50);"  // 鼠标悬停时背景色为透明蓝
        "   color: white;"                           // 悬停时文字颜色变为白色
        "   box-shadow: 0px 4px 8px rgba(0, 0, 255, 0.5);" // 添加阴影效果
        "}"
        "QPushButton:pressed {"
        "   background-color: rgba(0, 0, 255, 100);" // 按下时背景变为深蓝
        "   border-color: darkblue;"                // 按下时边框为深蓝
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
