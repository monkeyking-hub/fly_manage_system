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
#include <interfacemanager.h>
#include <QWidget>
#include "orderwindow.h"
#include <QComboBox>
#include <QDebug>
#include <QFontDatabase> // 用于加载自定义字体 // 引入 inorder 界面
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPalette>
#include <QPixmap>
#include <QResource>
#include <QScrollArea>
#include <QTabWidget>
#include <QTableWidget>
#include <QTimer>
#include <QVBoxLayout>
#include "order.h"
#include "orderwidget.h"
#include <interfacemanager.h>
#include <QMainWindow>
#include <QPushButton>    // 添加这行
#include <QToolBar>       // 添加这行
#include <QTabWidget>
#include <QEventLoop>


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
    QWidget *resultWidget = new QWidget();
    resultLayout = new QVBoxLayout(resultWidget);  // 这里将resultLayout移到类成员变量
    resultWidget->setLayout(resultLayout);
     QEventLoop eventLoop;
    InterfaceManager::instance()->m_orderList.clear();
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
    QNetworkRequest request(QUrl("http://127.0.0.1:8080/api/orders/search"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = manager->post(request, requestData);
    connect(reply, &QNetworkReply::finished, this, [&eventLoop]() {
        // 创建一个单次触发的定时器
        QTimer::singleShot(1000, &eventLoop, &QEventLoop::quit);  // 1000毫秒 = 1秒
    });
 qDebug() << "发送请求" ;
    // 处理网络请求结果
    connect(reply, &QNetworkReply::finished, this, [this, reply, userId]() {
        if (reply->error() == QNetworkReply::NoError) {
            // 读取所有响应数据
            QByteArray responseData = reply->readAll();
            qDebug() << "Raw response:" << QString(responseData);

            // 解析返回的JSON数据
            QJsonDocument doc = QJsonDocument::fromJson(responseData);
            QJsonObject rootObj = doc.object();

            // 检查响应状态
            int code = rootObj["code"].toInt();
            if (code != 200) {
                qWarning() << "API返回错误:" << rootObj["message"].toString();
                return;
            }

            // 获取data数组
            QJsonArray ordersArray = rootObj["data"].toArray();
            if (ordersArray.isEmpty()) {
                qDebug() << "没有找到订单, 用户ID:" << userId;
                return;
            }

            // 遍历所有订单
            for (const QJsonValue &orderValue : ordersArray) {
                QJsonObject orderData = orderValue.toObject();

                qDebug() << "\n=== 订单详细信息 ===";
                qDebug() << "订单ID:" << orderData["id"].toInt();
                qDebug() << "航班号:" << orderData["flightNumber"].toString();
                qDebug() << "价格:" << orderData["price"].toDouble();
                qDebug() << "状态:" << orderData["status"].toString();

                // 直接从订单数据中获取航班信息（不需要flightDetails子对象）
                qDebug() << "\n--- 航班详细信息 ---";
                qDebug() << "出发地:" << orderData["departure"].toString();
                qDebug() << "目的地:" << orderData["destination"].toString();
                qDebug() << "航空公司:" << orderData["airlineCompany"].toString();
                qDebug() << "起飞时间:" << QDateTime::fromSecsSinceEpoch(orderData["departureTime"].toInt()).toString();
                qDebug() << "到达时间:" << QDateTime::fromSecsSinceEpoch(orderData["arrivalTime"].toInt()).toString();
                qDebug() << "机型:" << orderData["aircraftModel"].toString();
                qDebug() << "座位类型:" << orderData["seatType"].toString();
                qDebug() << "登机口:" << orderData["boardingGate"].toString();

                // 创建订单对象
                Order order;
                order.m_orderNumber = QString::number(orderData["id"].toInt());
                order.m_passenger = "用户"+order.m_orderNumber;
                order.m_amount = QString::number(orderData["price"].toDouble());
                order.m_flightNumber = orderData["flightNumber"].toString();

                order.m_status = orderData["status"].toString();
                if(orderData["status"].toString()=="Pending payment")
                {
                    order.m_status="已支付" ;
                }

                // 设置航班信息
                order.m_departure = orderData["departure"].toString();
                order.m_destination = orderData["destination"].toString();
                order.m_airline = orderData["airlineCompany"].toString();
                order.m_departureTime = QDateTime::fromSecsSinceEpoch(orderData["departureTime"].toInt()).toString();
                order.m_arrivalTime = QDateTime::fromSecsSinceEpoch(orderData["arrivalTime"].toInt()).toString();
                order.m_aircraftType = orderData["aircraftModel"].toString();
                order.m_seatClass = orderData["seatType"].toString();
                order.m_flightId = QString::number(orderData["flightId"].toInt());
                order.m_boardingGate = orderData["boardingGate"].toString();
                order.m_arrivalAirport = orderData["arrivalAirport"].toString();
                order.m_departureAirport = orderData["departureAirport"].toString();
                order.m_firstClassSeats = orderData["firstClassSeats"].toInt();
                order.m_economyClassSeats = orderData["economyClassSeats"].toInt();
                order.m_businessClassSeats = orderData["businessClassSeats"].toInt();
                order.m_firstClassPrice = orderData["firstClassPrice"].toDouble();
                order.m_economyClassPrice = orderData["economyClassPrice"].toDouble();
                order.m_businessClassPrice = orderData["businessClassPrice"].toDouble();

                // 将订单添加到全局订单列表
                InterfaceManager::instance()->m_orderList.append(order);
            }

            qDebug() << "成功获取并处理所有订单信息";
            qDebug() << "订单总数:" << InterfaceManager::instance()->m_orderList.size();
        } else {
            qWarning() << "网络请求失败:" << reply->errorString();
        }

        for (const Order &order : InterfaceManager::instance()->m_orderList)
            {

            OrderWidget *orderWidget = new OrderWidget(order, this);
            orderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
            orderWidget->setStyleSheet(
                "#OrderWidget {"
                "   background-color: rgba(255, 255, 255, 200);"
                "   border: 2px solid #cccccc; "
                "   border-radius: 8px; "
                "   margin: 5px;"  // 为每个 OrderWidget 设置 margin
                "   padding: 8px;"
                "} "
                );
             qDebug() << "放置order";
            resultLayout->addWidget(orderWidget);
        }
        // 确保释放reply对象
        reply->deleteLater();
    });
     eventLoop.exec();
}
