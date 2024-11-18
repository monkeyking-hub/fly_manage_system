#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QGraphicsDropShadowEffect>

loginWindow::loginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::loginWindow)
{
    ui->setupUi(this);

    //设置图片1
    QPixmap *pix1=new QPixmap(":/blue.png");
    QSize sz1=ui->label_image->size();
    ui->label_image->setPixmap(pix1->scaled(sz1));

    //设置图片2
    QPixmap *pix2=new QPixmap(":/logo.png");
    QSize sz2=ui->label_logo->size();
    ui->label_logo->setPixmap(pix2->scaled(sz2));

    //设置图片1和2的阴影效果
    QGraphicsDropShadowEffect *shadow=new QGraphicsDropShadowEffect(this);
    shadow->setOffset(-3,0);
    shadow->setColor(QColor("#888888"));
    shadow->setBlurRadius(30);
    ui->label_image->setGraphicsEffect(shadow); //设置图片1

    //连接登录按钮点击事件
    connect(ui->pushButton_sign_in,&QPushButton::clicked,this,&loginWindow::onLoginButtonClicked);
}

void loginWindow::onLoginButtonClicked()
{

}

loginWindow::~loginWindow()
{
    delete ui;
}
