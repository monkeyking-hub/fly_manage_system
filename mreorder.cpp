#include "mreorder.h"

#include <QVBoxLayout>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QLabel>
#include <QDateTime>
#include <QNetworkRequest>

// 析构函数
MReorderWindow::~MReorderWindow() {}

// 构造函数
MReorderWindow::MReorderWindow(QWidget *parent)
    : QWidget(parent)
{
    // 设置主窗口背景颜色
    this->setStyleSheet("background-color: #f0f0f0;");

    // 通用字体设置
    QFont largerFont("Arial", 12); // 设置字体族和大小

    // 创建控件
    orderIdEdit = new QLineEdit(this);
    orderIdEdit->setPlaceholderText("请输入 ID");
    orderIdEdit->setFixedSize(300, 45); // 设置固定宽度和高度
    orderIdEdit->setFont(largerFont);
    orderIdEdit->setStyleSheet(
        "QLineEdit {"
        "   border: 2px solid #ccc;"
        "   border-radius: 10px;"
        "   padding: 5px 10px;"
        "   font-size: 12pt;"
        "   min-height: 45px;"
        "}"
        );

    statusCombo = new QComboBox(this);
    statusCombo->addItem("已支付");
    statusCombo->addItem("未支付");
    statusCombo->addItem("已退款");
    statusCombo->setFixedSize(300, 45);
    statusCombo->setFont(largerFont);
    statusCombo->setStyleSheet(
        "QComboBox {"
        "   border: 2px solid #ccc;"
        "   border-radius: 10px;"
        "   padding: 5px 10px;"
        "   font-size: 12pt;"
        "   min-height: 45px;"
        "}"
        );

    dateEdit = new QDateEdit(QDate::currentDate(), this);
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("yyyy-MM-dd");
    dateEdit->setFixedSize(300, 45);
    dateEdit->setFont(largerFont);
    dateEdit->setStyleSheet(
        "QDateEdit {"
        "   border: 2px solid #ccc;"
        "   border-radius: 10px;"
        "   padding: 5px 10px;"
        "   font-size: 12pt;"
        "   min-height: 45px;"
        "}"
        );

    submitButton = new QPushButton("提交", this);
    submitButton->setFixedSize(300, 45);
    submitButton->setFont(largerFont);
    submitButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #007BFF;"
        "   color: white;"
        "   font-weight: bold;"
        "   border: none;"
        "   border-radius: 10px;"
        "   padding: 10px;"
        "   font-size: 12pt;"
        "   min-height: 45px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0056b3;"
        "}"
        );

    // 布局部分
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    // 标签样式也相应调整
    QLabel *userIdLabel = new QLabel("订单 ID:", this);
    userIdLabel->setFont(largerFont);
    userIdLabel->setStyleSheet(
        "QLabel {"
        "   background-color: #E8F5E9;"
        "   padding: 8px;"
        "   border-radius: 5px;"
        "   font-size: 12pt;"
        "}"
        );
    layout->addWidget(userIdLabel);

    orderIdEdit->setStyleSheet(
        "QLineEdit {"
        "   background-color: #E8F5E9;"
        "   border: 2px solid #66BB6A;"
        "   border-radius: 10px;"
        "   padding: 5px 10px;"
        "   font-size: 12pt;"
        "   min-height: 45px;"
        "}"
        );
    layout->addWidget(orderIdEdit);

    QLabel *statusLabel = new QLabel("订单状态:", this);
    statusLabel->setFont(largerFont);
    statusLabel->setStyleSheet(
        "QLabel {"
        "   background-color: #E8F5E9;"
        "   padding: 8px;"
        "   border-radius: 5px;"
        "   font-size: 12pt;"
        "}"
        );
    layout->addWidget(statusLabel);

    statusCombo->setStyleSheet(
        "QComboBox {"
        "   background-color: #E8F5E9;"
        "   border: 2px solid #66BB6A;"
        "   border-radius: 10px;"
        "   padding: 5px 10px;"
        "   font-size: 12pt;"
        "   min-height: 45px;"
        "}"
        );
    layout->addWidget(statusCombo);

    QLabel *dateLabel = new QLabel("请选择修改日期:", this);
    dateLabel->setFont(largerFont);
    dateLabel->setStyleSheet(
        "QLabel {"
        "   background-color: #E8F5E9;"
        "   padding: 8px;"
        "   border-radius: 5px;"
        "   font-size: 12pt;"
        "}"
        );
    layout->addWidget(dateLabel);

    dateEdit->setStyleSheet(
        "QDateEdit {"
        "   background-color: #E8F5E9;"
        "   border: 2px solid #66BB6A;"
        "   border-radius: 10px;"
        "   padding: 5px 10px;"
        "   font-size: 12pt;"
        "   min-height: 45px;"
        "}"
        );
    layout->addWidget(dateEdit);


    // 提交按钮
    submitButton->setStyleSheet(
        "background-color: #2196F3; color: white; font-weight: bold; "
        "border: none; border-radius: 10px; padding: 10px;"
        "font-size: 18px"
        );
    layout->addWidget(submitButton);


    // 设置布局
    setLayout(layout);

    // 设置网络管理器
    networkManager = new QNetworkAccessManager(this);

    // 信号和槽
    connect(submitButton, &QPushButton::clicked, [this]() {
        onSubmitClicked(dateEdit->date());
    });
}


void MReorderWindow::onSubmitClicked(const QDate &selectedDate)
{
    bool ok;
    int orderId = orderIdEdit->text().toInt(&ok);
    if (!ok || orderId <= 0) {
        QMessageBox::warning(this, "Input Error", "请输入正确的订单号（正整数）。");
        return;
    }

    QString status = statusCombo->currentText();
    QTime currentTime = QTime::currentTime();
    QDateTime dateTime(selectedDate, currentTime);
    int paymentTime = dateTime.toSecsSinceEpoch();

    QJsonObject json;
    json["orderId"] = orderId;
    json["status"] = status;
    json["paymentTime"] = paymentTime;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QNetworkRequest request(QUrl("http://127.0.0.1:8080/api/orders/update"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    submitButton->setEnabled(false);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MReorderWindow::onReplyFinished);
    networkManager->post(request, data);
}

void MReorderWindow::onReplyFinished(QNetworkReply *reply)
{
    submitButton->setEnabled(true);

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject jsonResponse = doc.object();

        if (jsonResponse["code"].toInt() == 200) {
            QMessageBox::information(this, "Success", "Order updated successfully.");
        } else {
            QMessageBox::warning(this, "Error", jsonResponse["message"].toString());
        }
    } else {
        QMessageBox::warning(this, "Network Error",
                             QString("Error: %1").arg(reply->errorString()));
    }

    reply->deleteLater();
}
