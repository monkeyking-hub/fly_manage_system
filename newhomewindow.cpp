#include "newhomewindow.h"
#include "ui_newhomewindow.h"

newHomeWindow::newHomeWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::newHomeWindow)
{
    ui->setupUi(this);

    ui->centralwidget->setStyleSheet(
        "#centralwidget{"
        "background-image: url(:/homeWindowBKG.png);"  // 设置背景图片
        "background-repeat: no-repeat;"                  // 背景图片不重复
        "background-position: center;"                   // 背景图片居中
        "background-size: cover;"        // 背景图片拉伸以覆盖整个窗口
        "}"
        );

}
newHomeWindow::~newHomeWindow()
{
    delete ui;
}
