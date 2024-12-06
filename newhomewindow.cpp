#include "newhomewindow.h"
#include "ui_newhomewindow.h"
#include "interfacemanager.h"
#include <QCommandLinkButton>
#include <QApplication>
#include <QWidget>
#include "user.h"
#include "usermanager.h"

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
    QLabel* lbl=ui->label_2;
    User currentUser = UserManager::getInstance()->getCurrentUser();
    lbl->setText(currentUser.username);
    connect(UserManager::getInstance(),&UserManager::currentUserChanged,this,[lbl](const User& User){lbl->setText(User.username);});
    connect(ui->commandLinkButton_3,&QCommandLinkButton::clicked,this,&newHomeWindow::onCommandLinkeButton3Clicked);
    connect(ui->commandLinkButton_4, &QCommandLinkButton::clicked, this, &newHomeWindow::commandLinkButton4Clicked);
    connect(ui->commandLinkButton_5, &QCommandLinkButton::clicked, this, &newHomeWindow::commandLinkButton5Clicked);
    connect(ui->commandLinkButton_6, &QCommandLinkButton::clicked, this, &newHomeWindow::commandLinkButton6Clicked);
}

void newHomeWindow::onCommandLinkeButton3Clicked()
{

    InterfaceManager::instance()->switchToPage("fzj_window");
}

newHomeWindow::~newHomeWindow()
{
    delete ui;
}
