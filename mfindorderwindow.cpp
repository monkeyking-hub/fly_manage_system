#include "mfindorderwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QSpacerItem>
#include <QDebug>
#include "order.h"
#include "orderwidget.h"
#include <interfacemanager.h>

MFindOrderWindow::MFindOrderWindow(QWidget *parent)
    : QWidget(parent)
    , userIdLineEdit(new QLineEdit(this))
    , searchButton(new QPushButton("搜索", this))
    , resultWidget(new QWidget(this))
    , resultLayout(new QVBoxLayout(resultWidget))
{
    // 设置界面布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    // 创建一个水平布局
    QHBoxLayout *messageLayout1 = new QHBoxLayout();
    messageLayout1->setContentsMargins(0, 0, 0, 0); // 去除布局的边距

    // 创建第一张图片
    QLabel *messageLabel1 = new QLabel(this);
    QPixmap boxIcon(":/newicon/newxiangzi.jpg");
    boxIcon = boxIcon.scaled(100, 100, Qt::KeepAspectRatio); // 缩小图片
    messageLabel1->setPixmap(boxIcon);

    // 创建第一段文字
    QLabel *textLabel1 = new QLabel("良好的态度是客服的第一要义", this);
    textLabel1->setAlignment(Qt::AlignCenter);  // 使文字居中
    textLabel1->setStyleSheet("font-size: 12px;");
    textLabel1->setFixedWidth(500); // 直接设置固定宽度

    // 创建弹簧，将它们放在布局的两端
    QSpacerItem *leftSpacer = new QSpacerItem(20, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *rightSpacer = new QSpacerItem(20, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    // 添加弹簧、图片和文字到布局
    messageLayout1->addItem(leftSpacer);      // 左边的弹簧
    messageLayout1->addWidget(messageLabel1); // 第一张图片
    messageLayout1->addWidget(textLabel1);    // 文字
    messageLayout1->addItem(rightSpacer);     // 右边的弹簧

    // 将布局添加到主布局中
    layout->addLayout(messageLayout1);

    // 第二张图片和文字，采用类似的布局方式
    QHBoxLayout *messageLayout2 = new QHBoxLayout();
    messageLayout2->setContentsMargins(0, 0, 0, 0); // 去除布局的边距

    QLabel *messageLabel2 = new QLabel(this);
    QPixmap smileIcon(":/newicon/newqincai.jpg");
    smileIcon = smileIcon.scaled(100, 100, Qt::KeepAspectRatio); // 调整为相同大小
    messageLabel2->setPixmap(smileIcon);

    QLabel *textLabel2 = new QLabel("不好的态度会导致不必要的冲突哦", this);
    textLabel2->setAlignment(Qt::AlignCenter);  // 使文字居中
    textLabel2->setStyleSheet("font-size: 14px;");
    textLabel2->setFixedWidth(500); // 直接设置固定宽度

    // 再次添加弹簧到布局
    QSpacerItem *leftSpacer2 = new QSpacerItem(20, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *rightSpacer2 = new QSpacerItem(20, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    messageLayout2->addItem(leftSpacer2);      // 左边的弹簧
    messageLayout2->addWidget(messageLabel2);  // 第二张图片
    messageLayout2->addWidget(textLabel2);     // 文字
    messageLayout2->addItem(rightSpacer2);     // 右边的弹簧

    // 将第二个布局添加到主布局中
    layout->addLayout(messageLayout2);



    // 为提示框加上背景颜色和圆角效果
    QString messageStyle = "background-color: orange; "
                           "border-radius: 15px; "
                           "padding: 15px; "
                           "font-size: 20px; ";

    textLabel1->setStyleSheet(messageStyle);
    textLabel2->setStyleSheet(messageStyle);





    // 创建一个水平布局
    QHBoxLayout *idLayout = new QHBoxLayout();
    QLabel *idLabel = new QLabel("用户ID: ", this);
    idLabel->setFixedSize(40,40);

    // 调整 userIcon 图片大小
    QPixmap userIcon(":/newicon/fuser.png");
    userIcon = userIcon.scaled(40, 40, Qt::KeepAspectRatio); // 缩小图片
    QLabel *userIconLabel = new QLabel(this);
    userIconLabel->setPixmap(userIcon);

    // 创建弹簧来将控件紧密排列
    QSpacerItem *spacer = new QSpacerItem(10, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    // 将图片、标签和输入框加入布局
    idLayout->addItem(spacer);
    idLayout->addWidget(userIconLabel);
    idLayout->addWidget(idLabel);
    idLayout->addWidget(userIdLineEdit);
    idLayout->addItem(spacer);

    // 设置输入框样式（圆角）并控制输入框的宽度
    userIdLineEdit->setStyleSheet("QLineEdit {"
                                  "border-radius: 8px; "
                                  "border: 1px solid #ccc; "
                                  "padding: 5px; "
                                  "width: 50px; "  // 设置输入框宽度为50px
                                  "}");
    userIdLineEdit->setFixedWidth(300); // 直接设置固定宽度
    userIdLineEdit->setFixedHeight(40);

    // 将布局添加到主布局中
    layout->addLayout(idLayout);






    // 设置按钮样式
    searchButton->setStyleSheet("QPushButton {"
                                "border-radius: 8px; "
                                "background-color: #4CAF50; "
                                "color: white; "
                                "padding: 5px 10px; "
                                "font-size: 22px; "
                                "width: 150px;"
                                "height: 50px"
                                "} "
                                "QPushButton:hover {"
                                "background-color: #45a049; "
                                "} ");
    layout->addWidget(searchButton, 0, Qt::AlignHCenter);  // 将按钮放在中心

    // 设置结果显示区域样式
    resultWidget->setLayout(resultLayout);
    resultWidget->setStyleSheet(
        "border-radius: 15px; "
        "border: 4px solid #3498db; "
        "padding: 0px; "
        );

    // 滑动区域
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(resultWidget);
    scrollArea->setWidgetResizable(true);
    layout->addWidget(scrollArea);

    // 连接搜索按钮点击事件
    connect(searchButton, &QPushButton::clicked, this, &MFindOrderWindow::onSearchButtonClicked);
}

MFindOrderWindow::~MFindOrderWindow() {}

void MFindOrderWindow::onSearchButtonClicked()
{
    // 清空之前的结果
    clearResults();

    QString userId = userIdLineEdit->text();
    if (userId.isEmpty()) {
        QMessageBox::warning(this, "用户名不能为空", "请输入有效的ID.");
        return;
    }

    // 创建请求体
    QJsonObject requestObj;
    requestObj["userId"] = userId.toInt(); // 用户ID

    QJsonDocument requestDoc(requestObj);
    QByteArray requestData = requestDoc.toJson();

    // 网络请求
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl("http://127.0.0.1:8080/api/orders/search"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = manager->post(request, requestData);
    connect(reply, &QNetworkReply::finished, this, [this, reply, userId]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(responseData);
            QJsonObject rootObj = doc.object();

            // 检查响应状态
            int code = rootObj["code"].toInt();
            if (code != 200) {
                QMessageBox::warning(this, "错误", rootObj["message"].toString());
                reply->deleteLater();
                return;
            }

            // 获取订单数据
            QJsonArray ordersArray = rootObj["data"].toArray();
            if (ordersArray.isEmpty()) {
                QMessageBox::information(this, "查询结果", "未找到相关订单。");
                reply->deleteLater();
                return;
            }

            // 添加订单到结果显示区域
            for (const QJsonValue &orderValue : ordersArray) {
                QJsonObject orderData = orderValue.toObject();

                Order order;
                order.m_orderNumber = QString::number(orderData["id"].toInt());
                order.m_flightNumber = orderData["flightNumber"].toString();
                order.m_amount = QString::number(orderData["price"].toDouble());
                order.m_status = orderData["status"].toString();

                OrderWidget *orderWidget = new OrderWidget(order, this);
                orderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
                orderWidget->setStyleSheet(
                    "#OrderWidget {"
                    "   background-color: rgba(255, 255, 255, 200);"
                    "   border: 2px solid #cccccc; "
                    "   border-radius: 8px; "
                    "   margin: 5px;"
                    "   padding: 8px;"
                    "} "
                    );

                resultLayout->addWidget(orderWidget);
            }
        } else {
            QMessageBox::warning(this, "网络错误", reply->errorString());
        }

        reply->deleteLater();
    });
}

void MFindOrderWindow::clearResults()
{
    // 清空布局中的所有子控件
    QLayoutItem *child;
    while ((child = resultLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            delete child->widget();
        }
        delete child;
    }

    // 清空全局订单列表
    InterfaceManager::instance()->m_orderList.clear();
}
