#include "orderwindow.h"
#include "ui_orderwindow.h"
#include <QPalette>
#include <QPixmap>
#include <QResource>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QFontDatabase>  // 用于加载自定义字体 // 引入 inorder 界面
#include <QTabWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include "orderwidget.h"
#include "order.h"
#include <QScrollArea>
#include "maininterface.h"
#include "usermanager.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>




// orderwindow 构造函数
orderwindow::orderwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::order_2)  // 这里应该是 Ui::order_2，而不是 Ui::orderwindow
{
    ui->setupUi(this);

    ui->cs->setObjectName("centralWidget");

    // 设置背景图片（仅应用于 centralWidget）
    ui->cs->setStyleSheet(
        "#centralWidget {"
        "   background-image: url(:/sky3.png);"
        "   background-repeat: no-repeat;"
        "   background-position: center;"
        "   background-size: cover;"  // 修改为 cover 以确保图片覆盖整个区域
        "}"
        );

    // 动态创建 QTabWidget
    QTabWidget *tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget); // 将 tabWidget 作为主窗口的中央控件

    // 创建每个页面
    QWidget *allOrdersPage = createOrderPage("全部订单");
    QWidget *pendingPage = createOrderPage("待支付");
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


QWidget* orderwindow::createOrderPage(const QString &type)
{
    QWidget *page = new QWidget(this);
    QVBoxLayout *pageLayout = new QVBoxLayout(page);

    // 获取当前用户信息
    UserManager *userManager = UserManager::getInstance();
    User currentUser = userManager->getCurrentUser(); // 假设你有一个 `getCurrentUser` 函数
    int userId = 1;//currentUser.id;
    QString username = currentUser.username;

    // 订单类型过滤标签
    QLabel *label = new QLabel(QString("订单类型：%1").arg(type), page);
    label->setStyleSheet("font-size: 18px; font-weight: bold; color: black;");
    pageLayout->addWidget(label);

    QLabel *userLabel = new QLabel(QString("用户：%1 (ID: %2)").arg(username).arg(userId), page);
    userLabel->setStyleSheet("font-size: 16px; color: gray;");
    pageLayout->addWidget(userLabel);

    // 创建一个 QScrollArea 用于滚动订单列表
    QScrollArea *scrollArea = new QScrollArea(page);
    QWidget *container = new QWidget(scrollArea);
    QVBoxLayout *containerLayout = new QVBoxLayout(container);

    // 使用网络请求获取订单 ID 列表
    fetchOrderIds(userId, containerLayout, type, container); // 调用 fetchOrderIds 获取订单 ID

    // 设置滚动区域
    scrollArea->setWidget(container);
    pageLayout->addWidget(scrollArea);

    return page;
}



void orderwindow::fetchOrderIds(int userId, QVBoxLayout *containerLayout, const QString &type, QWidget *container)
{
    // 使用网络请求获取订单 ID 列表
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl("http://192.168.110.12:8080/api/orders/search"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 请求体，传递用户 ID
    QJsonObject requestBody;
    requestBody["userId"] = userId;

    QNetworkReply *reply = manager->post(request, QJsonDocument(requestBody).toJson());

    // Lambda 函数处理响应
    connect(reply, &QNetworkReply::finished, [this, reply, containerLayout, type, container]() {
        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::warning(this, "Error", "Failed to fetch orders: " + reply->errorString());
            reply->deleteLater();
            return;
        }

        // 解析响应数据
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject responseObject = jsonResponse.object();
        QJsonArray orderArray = responseObject["data"].toArray();

        QList<int> orderIds;

        // 获取所有订单的 ID
        for (const QJsonValue &value : orderArray) {
            QJsonObject obj = value.toObject();
            orderIds.append(obj["id"].toInt());  // 假设 ID 是整数类型
        }

        // 获取订单的详细信息
        fetchOrderDetails(orderIds, containerLayout, type, container);  // 调用 fetchOrderDetails 获取订单详情
    });
}


void orderwindow::fetchOrderDetails(const QList<int> &orderIds, QVBoxLayout *containerLayout, const QString &type, QWidget *container)
{
    // 使用网络请求获取订单详情数据
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    // 为了逐个请求订单详情，保持一个索引来访问每个订单
    int currentIndex = 0;

    // 定义 sendRequest 函数，捕获所需变量
    std::function<void()> sendRequest = [&]() {
        if (currentIndex >= orderIds.size()) {
            // 所有订单的详细信息都已经加载完毕
            return;
        }

        int orderId = orderIds[currentIndex];
        QNetworkRequest request(QUrl("http://192.168.110.12:8080/api/flights/queryById"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        // 请求体，传递当前订单 ID
        QJsonObject requestBody;
        requestBody["orderIds"] = QJsonArray{ orderId };  // 每次只请求一个订单

        QNetworkReply *reply = manager->post(request, QJsonDocument(requestBody).toJson());

        // Lambda 函数处理响应
        connect(reply, &QNetworkReply::finished, [this, reply, containerLayout, type, container, orderId, &currentIndex, &sendRequest, &orderIds]
() {
            if (reply->error() != QNetworkReply::NoError) {
                QMessageBox::warning(this, "Error", "Failed to fetch order details: " + reply->errorString());
                reply->deleteLater();
                return;
            }

            // 解析响应数据
            QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
            QJsonObject responseObject = jsonResponse.object();
            QJsonArray orderDetailsArray = responseObject["data"].toArray();

            if (orderDetailsArray.isEmpty()) {
                reply->deleteLater();
                return;
            }

            QJsonObject orderDetails = orderDetailsArray[0].toObject();

            // 根据返回的订单详情构建 Order 对象
            Order::Status status = Order::Pending; // 默认状态
            QString statusStr = orderDetails["status"].toString();
            if (statusStr == "Pending payment") status = Order::Pending;
            else if (statusStr == "Confirmed") status = Order::Confirmed;
            else if (statusStr == "Completed") status = Order::Completed;
            else if (statusStr == "Cancelled") status = Order::Cancelled;

            Order order(
                orderDetails["id"].toString(),
                orderDetails["passenger"].toString(),
                QString::number(orderDetails["price"].toDouble(), 'f', 2),
                orderDetails["departure"].toString(),
                orderDetails["destination"].toString(),
                orderDetails["airlineCompany"].toString(),
                orderDetails["flightNumber"].toString(),
                orderDetails["departureTime"].toString(),
                orderDetails["arrivalTime"].toString(),
                orderDetails["aircraftModel"].toString(),
                orderDetails["seatType"].toString(),
                status
                );

            // 创建 OrderWidget 组件
            OrderWidget *orderWidget = new OrderWidget(order, container);

            // 连接点击信号到槽函数
            connect(orderWidget, &OrderWidget::orderClicked, this, &orderwindow::showOrderDetails);

            // 为 OrderWidget 添加样式
            orderWidget->setStyleSheet(
                "#OrderWidget {"
                "   background-color: rgba(255, 255, 255, 200);"
                "   border: 2px solid #cccccc;"
                "   border-radius: 8px;"
                "   margin: 10px;"
                "   padding: 8px;"
                "} "
                );

            // 添加到布局中
            containerLayout->addWidget(orderWidget);

            // 如果不是最后一个订单，添加分割线
            if (currentIndex < orderIds.size() - 1) {
                QWidget *line = new QWidget(container);
                line->setFixedHeight(2);
                line->setStyleSheet("background-color: black;");
                line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

                containerLayout->addWidget(line);
            }

            // 删除回复对象
            reply->deleteLater();

            // 增加索引，继续请求下一个订单
            currentIndex++;

            // 递归调用发送下一个请求
            sendRequest();
        });
    };

    // 开始请求第一个订单
    sendRequest();
}






void orderwindow::showOrderDetails(const Order &order)
{
    OrderDetailWindow *detailWindow = new OrderDetailWindow(order, nullptr); // 父窗口改为 nullptr
    detailWindow->setAttribute(Qt::WA_DeleteOnClose);
    detailWindow->show();
    detailWindow->raise(); // 提升到最前
    detailWindow->activateWindow(); // 激活窗口
    qDebug() << "Creating detail window for order:" << order.orderNumber();
}

