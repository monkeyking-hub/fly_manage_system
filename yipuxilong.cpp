#include "yipuxilong.h"
#include "ui_yipuxilong.h"
#include <interfacemanager.h>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
Yipuxilong::Yipuxilong(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Yipuxilong)
{
    ui->setupUi(this);
}

Yipuxilong::~Yipuxilong()
{
    delete ui;
}

void Yipuxilong::on_toBetaButton_clicked()
{
    InterfaceManager::instance()->switchToPage("/MainWindow/Beta");
}


void Yipuxilong::on_toGameButton_clicked()
{
    InterfaceManager::instance()->switchToPage("/MainWindow/Beta/Gama");
}


void Yipuxilong::on_RequestButton_clicked()
{
    // 创建 QNetworkAccessManager 对象
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    // 创建 GET 请求
    QUrl url("http://127.0.0.1:5001/login");
    QNetworkRequest request(url);

    // 发送请求
     QNetworkReply *reply = manager->get(request);

    // 连接信号槽以处理响应
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            // 请求成功，处理返回的数据
            QByteArray response = reply->readAll();
            // 处理响应数据（例如，解析机票信息）
            QString showHelloStr = QString::fromUtf8(response);
            ui->showHelloLabel->setText(showHelloStr);
        } else {
            // 请求失败，处理错误
            QString error = "请检查服务器是否启动";
            ui->showHelloLabel->setText(error);
            qDebug() << "Error: " << reply->errorString();
        }

        // 释放资源
        reply->deleteLater();
    });
}

