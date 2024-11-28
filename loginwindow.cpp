#include "loginwindow.h"
#include <QGraphicsDropShadowEffect>
#include "ui_loginwindow.h"
#include <interfacemanager.h>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

loginWindow::loginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::loginWindow)
{
    ui->setupUi(this);

    //设置图片1
    QPixmap *pix1 = new QPixmap(":/blue.png");
    QSize sz1 = ui->label_image->size();
    ui->label_image->setPixmap(pix1->scaled(sz1));

    //设置图片2
    QPixmap *pix2 = new QPixmap(":/logo.png");
    QSize sz2 = ui->label_logo->size();
    ui->label_logo->setPixmap(pix2->scaled(sz2));

    //设置图片1和2的阴影效果
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(-3, 0);
    shadow->setColor(QColor("#888888"));
    shadow->setBlurRadius(30);
    ui->label_image->setGraphicsEffect(shadow); //设置图片1

    //连接登录按钮点击事件
    connect(ui->pushButton_sign_in, &QPushButton::clicked, this, &loginWindow::onLoginButtonClicked);
    //连接注册按钮点击事件
    connect(ui->pushButton_sign_up,
            &QPushButton::clicked,
            this,
            &loginWindow::onSignUpButtonClicked);
}

void loginWindow::onLoginButtonClicked() //点击登录按钮触发事件
{
    InterfaceManager::instance()->switchToPage("lxt_mainInterface"); //跳转到应用主界面
    QString usrname = ui->LineEdit_usrname->text();
    QString password = ui->lineEdit_pwd->text();
    emit loginRequested(usrname,password);
}

void loginWindow::onSignUpButtonClicked() //点击注册按钮触发事件
{
    InterfaceManager::instance()->switchToPage("lxt_registerWindow"); //跳转到注册界面
}

loginWindow::~loginWindow()
{
    delete ui;
}

void loginHandler::handleLogin(const QString& username, const QString& password)
{
    qDebug()<<"username:"<<username;
    qDebug()<<"pwd:"<<password;

    // // 创建网络管理器
    // QNetworkAccessManager* manager = new QNetworkAccessManager();

    // // 设置请求 URL
    // QUrl url("http://localhost:8080/api/users/login"); // 替换为你的 API 地址
    // QNetworkRequest request(url);

    // // 设置请求头
    // request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // // 构建 JSON 请求体
    // QJsonObject json;
    // json["username"] = username;
    // json["password"] = password;
    // QJsonDocument jsonDoc(json);
    // QByteArray requestData = jsonDoc.toJson();

    // // 发送 POST 请求
    // QNetworkReply* reply = manager->post(request, requestData);

    // // 连接信号，等待响应
    // connect(reply, &QNetworkReply::finished, [reply]() {
    //     if (reply->error() == QNetworkReply::NoError) {
    //         // 请求成功，读取响应数据
    //         QByteArray responseData = reply->readAll();
    //         QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    //         QJsonObject responseObject = jsonResponse.object();

    //         // 解析响应 JSON
    //         QString message = responseObject["message"].toString();
    //         QString token = responseObject["token"].toString();

    //         qDebug() << "Login Successful: " << message;
    //         qDebug() << "Token: " << token;
    //     } else {
    //         // 请求失败
    //         qDebug() << "Error:" << reply->errorString();
    //     }
    //     reply->deleteLater(); // 释放资源
    // });

    // 清理管理器（这里可以根据实际需求决定是否释放）
}
