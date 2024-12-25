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

    // 创建控件
    orderIdEdit = new QLineEdit(this);
    orderIdEdit->setPlaceholderText("请输入 ID");
    orderIdEdit->setFixedWidth(300); // 固定宽度
    orderIdEdit->setStyleSheet("border: 2px solid #ccc; border-radius: 10px; padding: 5px;");

    statusCombo = new QComboBox(this);
    statusCombo->addItem("已支付");
    statusCombo->addItem("未支付");
    statusCombo->addItem("已退款");
    statusCombo->setFixedWidth(300); // 固定宽度
    statusCombo->setStyleSheet("border: 2px solid #ccc; border-radius: 10px; padding: 5px;");

    dateEdit = new QDateEdit(QDate::currentDate(), this);
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("yyyy-MM-dd");
    dateEdit->setFixedWidth(300); // 固定宽度
    dateEdit->setStyleSheet("border: 2px solid #ccc; border-radius: 10px; padding: 5px;");

    submitButton = new QPushButton("提交", this);
    submitButton->setFixedWidth(300); // 固定宽度
    submitButton->setStyleSheet(
        "background-color: #007BFF; color: white; font-weight: bold; "
        "border: none; border-radius: 10px; padding: 10px;"
        );

    // 布局
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignCenter); // 居中对齐
    layout->setSpacing(20); // 设置控件之间的间距

    // 用户 ID 标签和输入框
    QLabel *userIdLabel = new QLabel("用户 ID:", this);
    userIdLabel->setStyleSheet("background-color: #E8F5E9; padding: 5px; border-radius: 5px;");
    layout->addWidget(userIdLabel);

    orderIdEdit->setStyleSheet("background-color:#E8F5E9; border: 2px solid #66BB6A; border-radius: 10px; padding: 5px;");
    layout->addWidget(orderIdEdit);

    // 订单状态标签和下拉框
    QLabel *statusLabel = new QLabel("订单状态:", this);
    statusLabel->setStyleSheet("background-color: #E8F5E9; padding: 5px; border-radius: 5px;");
    layout->addWidget(statusLabel);

    statusCombo->setStyleSheet("background-color: #E8F5E9; border: 2px solid #66BB6A; border-radius: 10px; padding: 5px;");
    layout->addWidget(statusCombo);

    // 修改日期标签和日期选择框
    QLabel *dateLabel = new QLabel("请选择修改日期:", this);
    dateLabel->setStyleSheet("background-color: #E8F5E9; padding: 5px; border-radius: 5px;");
    layout->addWidget(dateLabel);

    dateEdit->setStyleSheet("background-color: #E8F5E9; border: 2px solid #66BB6A; border-radius: 10px; padding: 5px;");
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
