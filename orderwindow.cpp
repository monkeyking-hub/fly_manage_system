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
#include "maininterface.h"
#include "order.h"
#include "orderwidget.h"
#include "ui_orderwindow.h"
#include "usermanager.h"
#include <interfacemanager.h>

// orderwindow 构造函数
orderwindow::orderwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::order_2) // 这里应该是 Ui::order_2，而不是 Ui::orderwindow
{
    ui->setupUi(this);

    ui->cs->setObjectName("centralWidget");

    // 动态创建 QTabWidget
    QTabWidget *tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget); // 将 tabWidget 作为主窗口的中央控件
    // 获取当前用户信息
    UserManager *userManager = UserManager::getInstance();
    User currentUser = userManager->getCurrentUser();
    int userId =1;// currentUser.id;
    qDebug()<<"拿到用户id了"<<userId;

    // 获取所有订单 ID 和航班号，并获取详细信息
    fetchOrders(userId);

    // 创建每个页面

    QWidget *pendingPage = createOrderPage("待支付");
    QWidget *allOrdersPage = createOrderPage("全部订单");
    QWidget *upcomingPage = createOrderPage("待出行");
    QWidget *canceledPage = createOrderPage("已取消");

    // 添加到 TabWidget
    tabWidget->addTab(allOrdersPage, QIcon(":/order.png"), "全部订单");
    tabWidget->addTab(pendingPage, QIcon(":/unpayorder.png"), "待支付");
    tabWidget->addTab(upcomingPage, QIcon(":/bag.png"), "待出行");
    tabWidget->addTab(canceledPage, QIcon(":/deletedorder.png"), "已取消");

    // 设置 tab 的样式
    QString tabStyle = R"(
        QTabBar::tab {
            min-height: 50px;                /* 按钮高度 */
            min-width: 266px;               /* 按钮宽度 */
            font-size: 24px;                /* 字体大小 */
            font-weight: bold;              /* 字体加粗 */
            color: black;                   /* 文字颜色 */
            padding: 5px;                   /* 内边距 */
            border: 2px solid #000;         /* 边框 */
            border-radius: 15px;            /* 圆角 */
            background-color: rgba(255, 255, 255, 200); /* 背景色 */
        }
        QTabBar::tab:hover {
            background-color: rgba(0, 0, 255, 50); /* 悬停背景色 */
            color: white;                          /* 悬停文字颜色 */
        }
        QTabBar::tab:selected {
            background-color: rgba(0, 0, 255, 100); /* 选中背景色 */
            color: white;                           /* 选中文字颜色 */
        }
    )";
    tabWidget->tabBar()->setStyleSheet(tabStyle);
    tabWidget->tabBar()->setIconSize(QSize(40, 40)); // 设置图标大小
}

orderwindow::~orderwindow()
{
    delete ui;
}

void orderwindow::fetchOrders(int userId) {
    // 创建网络访问管理器
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    // 设置请求 URL
    QNetworkRequest request(QUrl("http://192.168.110.12:8080/api/orders/search"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    qDebug() << "发送请求获取订单信息";

    // 构建请求体，包含用户 ID
    QJsonObject requestBody;
    requestBody["userId"] = userId;

    // 转换为 QByteArray 并发送 POST 请求
    QByteArray jsonData = QJsonDocument(requestBody).toJson();
    QNetworkReply *reply = manager->post(request, jsonData);

    // 处理网络请求结果
    connect(reply, &QNetworkReply::finished, this, [this, reply, userId]() {
        if (reply->error() == QNetworkReply::NoError) {
            // 解析返回的订单数据
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonArray ordersArray = doc.object()["data"].toArray();

            if (ordersArray.isEmpty()) {
                // 如果没有订单数据，返回
                qDebug() << "没有找到订单";
                return;
            }

            // 遍历所有订单并处理
            for (const QJsonValue &orderValue : ordersArray) {
                QJsonObject orderData = orderValue.toObject();
                QString orderId = QString::number(orderData["id"].toInt());
                QString flightNumber = orderData["flightNumber"].toString();
                double price = orderData["price"].toDouble();

                // 获取航班详细信息
                QJsonObject flightDetails = orderData["flightDetails"].toObject();
                QString departure = flightDetails["departure"].toString();
                QString destination = flightDetails["destination"].toString();
                QString airline = flightDetails["airlineCompany"].toString();
                QString departureTime = QDateTime::fromSecsSinceEpoch(flightDetails["departureTime"].toInt()).toString();
                QString arrivalTime = QDateTime::fromSecsSinceEpoch(flightDetails["arrivalTime"].toInt()).toString();
                QString aircraftType = flightDetails["aircraftModel"].toString();
                QString seatClass = flightDetails["seatType"].toString();
                QString boardingGate = flightDetails["boardingGate"].toString();
                QString arrivalAirport = flightDetails["arrivalAirport"].toString();
                QString departureAirport = flightDetails["departureAirport"].toString();
                int firstClassSeats = flightDetails["firstClassSeats"].toInt();
                int economyClassSeats = flightDetails["economyClassSeats"].toInt();
                int businessClassSeats = flightDetails["businessClassSeats"].toInt();
                double firstClassPrice = flightDetails["firstClassPrice"].toDouble();
                double economyClassPrice = flightDetails["economyClassPrice"].toDouble();
                double businessClassPrice = flightDetails["businessClassPrice"].toDouble();

                qDebug() << arrivalTime;
                // 创建订单对象并填充数据
                Order order;
                order.m_orderNumber = orderId;
                order.m_passenger = QString::number(userId);  // 假设用户 ID 为乘客姓名
                order.m_amount = QString::number(price);
                order.m_flightNumber = flightNumber;
                order.m_status = Order::Pending;  // 默认为 Pending

                // 设置航班信息
                order.m_departure = departure;
                order.m_destination = destination;
                order.m_airline = airline;
                order.m_departureTime = departureTime;
                order.m_arrivalTime = arrivalTime;
                order.m_aircraftType = aircraftType;
                order.m_seatClass = seatClass;
                order.m_flightId = QString::number(flightDetails["id"].toInt());
                order.m_boardingGate = boardingGate;
                order.m_arrivalAirport = arrivalAirport;
                order.m_departureAirport = departureAirport;
                order.m_firstClassSeats = firstClassSeats;
                order.m_economyClassSeats = economyClassSeats;
                order.m_businessClassSeats = businessClassSeats;
                order.m_firstClassPrice = firstClassPrice;
                order.m_economyClassPrice = economyClassPrice;
                order.m_businessClassPrice = businessClassPrice;

                // 将订单添加到全局订单列表
                InterfaceManager::instance()->m_orderList.append(order);
            }

            qDebug() << "成功获取并处理所有订单及航班信息";
        } else {
            // 错误处理
            qWarning() << "获取订单失败: " << reply->errorString();
        }

        // 删除 reply，避免内存泄漏
        reply->deleteLater();
    });
}



// 创建订单页面并展示所有订单
QWidget *orderwindow::createOrderPage(const QString &type)
{
    QWidget *page = new QWidget(this);
    QVBoxLayout *pageLayout = new QVBoxLayout(page);

    // 设置 pageLayout 布局的间隙
    pageLayout->setSpacing(20);  // 每个 OrderWidget 之间的空隙
    pageLayout->setContentsMargins(30, 30, 30, 30); // 设置边距，确保控件不会太靠边

    auto &orderList = InterfaceManager::instance()->m_orderList;

    // 遍历订单列表，根据订单类型显示订单
    for (const Order &order : orderList) {
        // 根据订单类型过滤订单
        if ((type == "全部订单") ||
            (type == "待支付" && order.status() == Order::Pending) ||
            (type == "待出行" && order.status() == Order::Confirmed) ||
            (type == "已取消" && order.status() == Order::Cancelled)) {

            // 创建 OrderWidget 来显示订单
            OrderWidget *orderWidget = new OrderWidget(order, pageLayout->parentWidget());

            // 设置 OrderWidget 的大小策略
            orderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

            // 为 OrderWidget 添加样式
            orderWidget->setStyleSheet(
                "#OrderWidget {"
                "   background-color: rgba(255, 255, 255, 200);"
                "   border: 2px solid #cccccc;"
                "   border-radius: 8px; "
                "   margin: 10px;"  // 为每个 OrderWidget 设置 margin
                "   padding: 8px;"
                "} "
                );

            // 连接点击事件信号和槽
            connect(orderWidget, &OrderWidget::orderClicked, this, &orderwindow::showOrderDetails);

            // 将 OrderWidget 添加到布局
            pageLayout->addWidget(orderWidget);
        }
    }

    return page;
}



void orderwindow::showOrderDetails(const Order &order)
{
    OrderDetailWindow *detailWindow = new OrderDetailWindow(order, nullptr); // 父窗口改为 nullptr
    detailWindow->setAttribute(Qt::WA_DeleteOnClose);
    detailWindow->show();
    detailWindow->raise();          // 提升到最前
    detailWindow->activateWindow(); // 激活窗口
    qDebug() << "Creating detail window for order:" << order.orderNumber();
}
