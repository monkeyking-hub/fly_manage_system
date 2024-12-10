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

    QPixmap pixmap1(":/newHomeWindow1.png");
    QPixmap scaledPixmap1=pixmap1.scaled(ui->label_12->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_12->setPixmap(scaledPixmap1);

    QPixmap pixmap2(":/newHomeWindow2.png");
    QPixmap scaledPixmap2=pixmap2.scaled(ui->label_15->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_15->setPixmap(scaledPixmap2);

    QPixmap pixmap3(":/newHomeWindow3.png");
    QPixmap scaledPixmap3=pixmap3.scaled(ui->label_11->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_11->setPixmap(scaledPixmap3);

    QPixmap pixmap4(":/newHomeWindow4.png");
    QPixmap scaledPixmap4=pixmap4.scaled(ui->label_5->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_5->setPixmap(scaledPixmap4);

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
