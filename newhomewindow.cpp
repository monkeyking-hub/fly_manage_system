#include "newhomewindow.h"
#include "ui_newhomewindow.h"
#include "interfacemanager.h"
#include <QCommandLinkButton>
#include <QApplication>
#include <QWidget>

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

    connect(ui->commandLinkButton_3,&QCommandLinkButton::clicked,this,&newHomeWindow::onCommandButton3Clicked);
    connect(ui->commandLinkButton_4,&QCommandLinkButton::clicked,this,&newHomeWindow::onCommandButton4Clicked);
    connect(ui->commandLinkButton_5,&QCommandLinkButton::clicked,this,&newHomeWindow::onCommandButton5Clicked);
    connect(ui->commandLinkButton_6,&QCommandLinkButton::clicked,this,&newHomeWindow::onCommandButton6Clicked);
}

void newHomeWindow::onCommandButton3Clicked() //点击按钮后跳转到编辑个人信息界面
{
    InterfaceManager::instance()->switchToPage("fzj_window");
}

void newHomeWindow::onCommandButton4Clicked() //点击按钮后跳转到航班动态界面
{
    InterfaceManager::instance()->switchToPage("fzj_flightstatus");
}

void newHomeWindow::onCommandButton5Clicked() //点击按钮后跳转到订单界面
{
    InterfaceManager::instance()->switchToPage("wmc_orderWindow");
}

void newHomeWindow::onCommandButton6Clicked() //点击按钮后跳转到homewindow界面
{
    InterfaceManager::instance()->switchToPage("lxt_homeWindow");
}

newHomeWindow::~newHomeWindow()
{
    delete ui;
}
