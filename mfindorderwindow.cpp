#include "mfindorderwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

MFindOrderWindow::MFindOrderWindow(QWidget *parent)
    : QWidget(parent)
    , userIdLineEdit(new QLineEdit(this))
    , searchButton(new QPushButton("Search", this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(userIdLineEdit);
    layout->addWidget(searchButton);
    setLayout(layout);

    connect(searchButton, &QPushButton::clicked, this, &MFindOrderWindow::onSearchButtonClicked);
}

MFindOrderWindow::~MFindOrderWindow() {}

void MFindOrderWindow::onSearchButtonClicked()
{
    QString userId = userIdLineEdit->text();
    if (userId.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a User ID.");
        return;
    }

    // 创建请求体
    QJsonObject requestObj;
    requestObj["userId"] = userId.toInt(); // 传递用户ID

    QJsonDocument requestDoc(requestObj);
    QByteArray requestData = requestDoc.toJson();

    // 设置请求
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl("http://yourapiurl/api/orders/search"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = manager->post(request, requestData);

    // 响应处理
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
            QJsonObject responseObj = responseDoc.object(); // 在此声明 responseObj

            int code = responseObj["code"].toInt();
            if (code == 200) {
                // 查询成功，处理返回的数据
                QJsonArray data = responseObj["data"].toArray();
                if (data.isEmpty()) {
                    QMessageBox::information(this, "No Orders", "No orders found for this user.");
                } else {
                    // 处理订单信息
                    for (const QJsonValue &value : data) {
                        QJsonObject orderObj = value.toObject();
                        QString orderInfo = QString(
                                                "Flight: %1\nSeat Type: %2\nPrice: %3\nStatus: %4")
                                                .arg(orderObj["flightNumber"].toString())
                                                .arg(orderObj["seatType"].toString())
                                                .arg(orderObj["price"].toDouble())
                                                .arg(orderObj["status"].toString());

                        // 将订单信息显示在界面上，使用QListWidget等组件
                        // 你可以将订单项添加到QListWidget或其他适合显示的控件中
                    }
                }
            } else {
                QMessageBox::warning(this, "Query Failed", responseObj["message"].toString());
            }
        } else {
            QMessageBox::warning(this, "Network Error", "Failed to query orders.");
        }
        reply->deleteLater();
    });
}
