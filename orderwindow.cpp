#include "orderwindow.h"
#include "ui_orderwindow.h"
#include <QPalette>
#include <QPixmap>
#include <QResource>

orderwindow::orderwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::order_2)
{
    ui->setupUi(this);

    // 加载资源文件中的图片
    QResource::registerResource("images.qrc");
    QPixmap bgPixmap("://blue.png");

    // 设置窗口背景图片
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(bgPixmap));
    this->setPalette(palette);

    // 设置窗口标题为"订单界面"
    this->setWindowTitle("订单界面");

    // 设置窗口大小
    this->resize(471, 600); // 这里将窗口大小设置为宽800像素，高600像素，你可以根据需求调整具体数值

    QPixmap iconPixmap(":/order.png"); // 替换为你实际的图标路径，这里假设图标也在资源文件中
    ui->order->setIcon(iconPixmap);


    QPixmap iconPixmap2(":/deletedorder.png"); // 替换为你实际的图标路径，这里假设图标也在资源文件中
    ui->order2->setIcon(iconPixmap2);

    QPixmap iconPixmap3(":/unpayorder.png"); // 替换为你实际的图标路径，这里假设图标也在资源文件中
    ui->order3->setIcon(iconPixmap3);

    QPixmap iconPixmap4(":/bag.png"); // 替换为你实际的图标路径，这里假设图标也在资源文件中
    ui->order4->setIcon(iconPixmap4);

    QSize iconSize(30, 30);
    ui->order->setIconSize(iconSize);
    ui->order2->setIconSize(iconSize);
    ui->order3->setIconSize(iconSize);
    ui->order4->setIconSize(iconSize);
}

orderwindow::~orderwindow()
{
    delete ui;
}
