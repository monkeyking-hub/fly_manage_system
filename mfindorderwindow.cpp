#include "mfindorderwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QWidget>

MFindOrderWindow::MFindOrderWindow(QWidget *parent)
    : QWidget(parent)
    , userIdLineEdit(new QLineEdit(this))
    , searchButton(new QPushButton("Search", this))
{
    // 设置界面布局
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 第一张图片小一点，文字和图片紧贴
    QLabel *messageLabel1 = new QLabel(this);
    QPixmap boxIcon(":/newicon/newxiangzi.jpg");
    boxIcon = boxIcon.scaled(100, 100, Qt::KeepAspectRatio); // 缩小图片
    messageLabel1->setPixmap(boxIcon);

    QLabel *textLabel1 = new QLabel("良好的态度是客服的第一要义", this);
    textLabel1->setAlignment(Qt::AlignCenter);  // 使文字居中
    textLabel1->setStyleSheet("font-size: 12px;");

    QHBoxLayout *messageLayout1 = new QHBoxLayout();
    messageLayout1->addWidget(messageLabel1);
    messageLayout1->addWidget(textLabel1);
    layout->addLayout(messageLayout1);

    // 第二张图片和文字调整为一样大小，文字居中
    QLabel *messageLabel2 = new QLabel(this);
    QPixmap smileIcon(":/newicon/newqincai.jpg");
    smileIcon = smileIcon.scaled(100, 100, Qt::KeepAspectRatio); // 调整为相同大小
    messageLabel2->setPixmap(smileIcon);

    QLabel *textLabel2 = new QLabel("不好的态度会导致不必要的冲突哦", this);
    textLabel2->setAlignment(Qt::AlignCenter);  // 使文字居中
    textLabel2->setStyleSheet("font-size: 14px;");

    QHBoxLayout *messageLayout2 = new QHBoxLayout();
    messageLayout2->addWidget(messageLabel2);
    messageLayout2->addWidget(textLabel2);
    layout->addLayout(messageLayout2);

    // 为提示框加上背景颜色和圆角效果
    QString messageStyle = "background-color: orange; "
                           "border-radius: 15px; "
                           "padding:15px; "
                           "font-size: 16px; "
                           "width: 300px;";
    textLabel1->setStyleSheet(messageStyle);
    textLabel2->setStyleSheet(messageStyle);

    // 添加搜索框
    QHBoxLayout *idLayout = new QHBoxLayout();
    QLabel *idLabel = new QLabel("User ID: ", this);

    // 调整 userIcon 图片大小
    QPixmap userIcon(":/newicon/fuser.png");
    userIcon = userIcon.scaled(20, 20, Qt::KeepAspectRatio); // 缩小图片
    QLabel *userIconLabel = new QLabel(this);
    userIconLabel->setPixmap(userIcon);

    idLayout->addWidget(userIconLabel);
    idLayout->addWidget(idLabel);
    idLayout->addWidget(userIdLineEdit);
    layout->addLayout(idLayout);

    // 设置输入框样式（圆角）
    userIdLineEdit->setStyleSheet("QLineEdit { border-radius: 8px; border: 1px solid #ccc; padding: 5px; width: 200px; }");

    // 设置按钮样式
    searchButton->setStyleSheet("QPushButton {"
                                "border-radius: 8px; "
                                "background-color: #4CAF50; "
                                "color: white; "
                                "padding: 5px 10px; "
                                "font-size: 12px; "
                                "width: 100px;"
                                "} "
                                "QPushButton:hover {"
                                "background-color: #45a049; "
                                "} ");
    layout->addWidget(searchButton, 0, Qt::AlignHCenter);  // 将按钮放在中心

    // 创建一个用于显示查询结果的区域
    QWidget *resultWidget = new QWidget();
    resultLayout = new QVBoxLayout(resultWidget);  // 这里将resultLayout移到类成员变量
    resultWidget->setLayout(resultLayout);

    // 设置滑动区域
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(resultWidget);
    scrollArea->setWidgetResizable(true);
    layout->addWidget(scrollArea);

    // 添加提示词
    QLabel *orderHeader = new QLabel("Search Results:", this);
    orderHeader->setStyleSheet("font-size: 16px; font-weight: bold;");
    resultLayout->addWidget(orderHeader);

    // 连接按钮点击事件
    connect(searchButton, &QPushButton::clicked, this, &MFindOrderWindow::onSearchButtonClicked);
}

MFindOrderWindow::~MFindOrderWindow() {}

void MFindOrderWindow::onSearchButtonClicked()
{
    QString userId = userIdLineEdit->text();
    if (userId.isEmpty()) {
        QMessageBox::warning(this, "用户名不能为空", "请输入有效的ID.");
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
            QJsonObject responseObj = responseDoc.object();

            int code = responseObj["code"].toInt();
            if (code == 200) {
                // 查询成功，处理返回的数据
                QJsonArray data = responseObj["data"].toArray();
                if (data.isEmpty()) {
                    QMessageBox::information(this, "No Orders", "No orders found for this user.");
                } else {
                    // 清空之前的结果
                    for (QObject *item : resultLayout->children()) {
                        QWidget *widget = qobject_cast<QWidget*>(item);  // 将 QObject 转换为 QWidget
                        if (widget) {
                            delete widget;  // 删除子 Widget
                        }
                    }

                    // 添加每个订单的信息
                    for (const QJsonValue &value : data) {
                        QJsonObject orderObj = value.toObject();

                        // 创建新的结果显示 Widget
                        QWidget *orderWidget = new QWidget(this);
                        QVBoxLayout *orderLayout = new QVBoxLayout(orderWidget);

                        // 显示航班号
                        QLabel *flightLabel = new QLabel(QString("Flight: %1").arg(orderObj["flightNumber"].toString()), this);
                        orderLayout->addWidget(flightLabel);

                        // 显示舱位等级
                        QLabel *seatLabel = new QLabel(QString("Seat Type: %1").arg(orderObj["seatType"].toString()), this);
                        orderLayout->addWidget(seatLabel);

                        // 显示价格
                        QLabel *priceLabel = new QLabel(QString("Price: %1").arg(orderObj["price"].toDouble()), this);
                        orderLayout->addWidget(priceLabel);

                        // 显示订单状态
                        QLabel *statusLabel = new QLabel(QString("Status: %1").arg(orderObj["status"].toString()), this);
                        orderLayout->addWidget(statusLabel);

                        // 将订单显示 widget 添加到 layout 中
                        resultLayout->addWidget(orderWidget);
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
