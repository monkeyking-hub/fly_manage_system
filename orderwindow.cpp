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
#include <interfacemanager.h>
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
#include <QTimer>
#include <QComboBox>


// orderwindow 构造函数
orderwindow::orderwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::order_2)  // 这里应该是 Ui::order_2，而不是 Ui::orderwindow
{
    ui->setupUi(this);

    ui->cs->setObjectName("centralWidget");


    // 动态创建 QTabWidget
    QTabWidget *tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget); // 将 tabWidget 作为主窗口的中央控件
    // 获取当前用户信息
    UserManager *userManager = UserManager::getInstance();
    User currentUser = userManager->getCurrentUser();
    int userId = currentUser.id;
    qDebug()<<"拿到用户id了"<<userId;

    // 获取所有订单 ID 和航班号，并获取详细信息
    fetchOrderIds(userId);

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




//这个用来获取每个订单的id，出行人，航班号，价格
void orderwindow::fetchOrderIds(int userId) {
    // 创建网络访问管理器
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    // 设置请求 URL
    QNetworkRequest request(QUrl("/api/orders/search"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    qDebug()<<"进到第一个函数里了";


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
                // 如果没有订单数据，说明已经查询完所有数据，结束查询
                qDebug() << "No more orders found.";
                return;
            }

            // 只会获取一个订单，因为每次请求返回一个订单
            QJsonObject orderData = ordersArray[0].toObject();
            QString orderId = orderData["id"].toString();
            QString flightNumber = orderData["flightNumber"].toString();
            double price = orderData["price"].toDouble();

            // 创建订单对象并填充数据
            Order order;
            order.m_orderNumber = orderId;
            order.m_passenger = QString::number(userId);  // 用户 ID 作为乘客姓名
            order.m_amount = QString::number(price);
            order.m_flightNumber = flightNumber;

            // 设置订单状态，默认设为 Pending
            order.m_status = Order::Pending;

            // 将初始化的 Order 添加到全局列表
            InterfaceManager::instance()->m_orderList.append(order);

            // 存储航班号，稍后查询航班详情
            QList<QString> flightNumbers = {flightNumber};

            // 调用第二个函数根据航班号查询详细信息
            fetchOrderDetails(flightNumbers);  // 传入当前订单的航班号列表

            // 继续请求下一个订单
            fetchOrderIds(userId);  // 递归请求下一个订单
        } else {
            // 错误处理
            qWarning() << "Error fetching orders for user " << userId << ": " << reply->errorString();
        }

        // 删除 reply，避免内存泄漏
        reply->deleteLater();
    });
}



void orderwindow::fetchOrderDetails(const QList<QString> &flightNumbers)
{
    qDebug()<<"进到第二个函数里了";
    // 遍历每个航班号，获取相应的航班详细信息
    for (const QString &flightNumber : flightNumbers) {
        // 创建 QNetworkAccessManager 用于发起网络请求
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);

        // 设置请求 URL
        QNetworkRequest request(QUrl("http://yourapi.com/api/flights/queryById"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        // 构建请求体
        QJsonObject requestBody;
        requestBody["id"] = flightNumber.toInt();  // 假设航班号可以转换为整数

        // 转换为 QByteArray 发送请求
        QByteArray jsonData = QJsonDocument(requestBody).toJson();
        QNetworkReply *reply = manager->post(request, jsonData);

        // 使用 lambda 函数处理网络请求结果
        connect(reply, &QNetworkReply::finished, this, [this, reply, flightNumber]() {
            if (reply->error() == QNetworkReply::NoError) {
                // 解析航班详细信息
                QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
                QJsonObject flightDetails = doc.object()["data"].toObject();

                // 查找对应的订单
                for (Order &order : InterfaceManager::instance()->m_orderList) {
                    if (order.m_flightNumber == flightNumber) {
                        // 更新订单详细信息
                        order.m_departure = flightDetails["departure"].toString();
                        order.m_destination = flightDetails["destination"].toString();
                        order.m_airline = flightDetails["airlineCompany"].toString();
                        order.m_departureTime = QDateTime::fromSecsSinceEpoch(flightDetails["departureTime"].toInt()).toString();
                        order.m_arrivalTime = QDateTime::fromSecsSinceEpoch(flightDetails["arrivalTime"].toInt()).toString();
                        order.m_aircraftType = flightDetails["aircraftModel"].toString();
                        order.m_seatClass = flightDetails["seatType"].toString();
                        order.m_flightId = QString::number(flightDetails["id"].toInt());
                        order.m_boardingGate = flightDetails["boardingGate"].toString();
                        order.m_arrivalAirport = flightDetails["arrivalAirport"].toString();
                        order.m_departureAirport = flightDetails["departureAirport"].toString();
                        order.m_firstClassSeats = flightDetails["firstClassSeats"].toInt();
                        order.m_economyClassSeats = flightDetails["economyClassSeats"].toInt();
                        order.m_businessClassSeats = flightDetails["businessClassSeats"].toInt();
                        order.m_firstClassPrice = flightDetails["firstClassPrice"].toDouble();
                        order.m_economyClassPrice = flightDetails["economyClassPrice"].toDouble();
                        order.m_businessClassPrice = flightDetails["businessClassPrice"].toDouble();
                    }
                }
            } else {
                // 错误处理
                qWarning() << "Error fetching flight details for flight number " << flightNumber << ": " << reply->errorString();
            }

            // 删除 reply 确保内存不会泄漏
            reply->deleteLater();
        });
    }
}



//这个用来创建每个小订单矩形
void orderwindow::createOrderWidget(const Order &order, QVBoxLayout *containerLayout)
{
    // 创建订单 Widget，传入订单数据
    OrderWidget *orderWidget = new OrderWidget(order, this);

    // 连接信号槽，当订单被点击时显示订单详情
    connect(orderWidget, &OrderWidget::orderClicked, this, &orderwindow::showOrderDetails);

    // 将订单 Widget 添加到容器布局中
    containerLayout->addWidget(orderWidget);

    // 如果不是最后一个订单，添加分割线
    // 这里修复了对 containerLayout.count() 的判断
    // 因为我们没有提供 `orderIds` 的上下文信息，因此假设你想通过订单的数量来判断是否添加分割线
    if (containerLayout->count() != InterfaceManager::instance()->m_orderList.size()) {
        QWidget *line = new QWidget(this);
        line->setFixedHeight(2);
        line->setStyleSheet("background-color: black;");
        containerLayout->addWidget(line);
    }
}





//这个用来创建每个具体的订单界面
// 创建每个具体的订单页面，并添加筛选框
QWidget* orderwindow::createOrderPage(const QString &type)
{
    QWidget *page = new QWidget(this);
    QVBoxLayout *pageLayout = new QVBoxLayout(page);

    // 创建筛选框布局
    QHBoxLayout *filterLayout = new QHBoxLayout();

    // 设置布局的间距（增加它们之间的空隙）
    filterLayout->setSpacing(30);  // 增加控件之间的空隙，设置为 30px

    // 出发地筛选框
    QComboBox *departureComboBox = new QComboBox(this);
    departureComboBox->addItem("选择出发地");
    // 假设你有一个出发地列表，可以通过 API 或本地数据填充
    departureComboBox->addItem("New York");
    departureComboBox->addItem("Los Angeles");
    departureComboBox->addItem("San Francisco");
    departureComboBox->setMinimumWidth(220);  // 设置最小宽度为 220px
    departureComboBox->setStyleSheet("QComboBox { height: 80px; font-size: 16px; }");  // 增加高度，设置字体大小

    // 目的地筛选框
    QComboBox *destinationComboBox = new QComboBox(this);
    destinationComboBox->addItem("选择目的地");
    // 假设你有一个目的地列表，可以通过 API 或本地数据填充
    destinationComboBox->addItem("Los Angeles");
    destinationComboBox->addItem("San Francisco");
    destinationComboBox->addItem("New York");
    destinationComboBox->setMinimumWidth(220);  // 设置最小宽度为 220px
    destinationComboBox->setStyleSheet("QComboBox { height: 80px; font-size: 16px; }");  // 增加高度，设置字体大小

    // 航空公司筛选框
    QComboBox *airlineComboBox = new QComboBox(this);
    airlineComboBox->addItem("选择航空公司");
    // 假设你有一个航空公司列表，可以通过 API 或本地数据填充
    airlineComboBox->addItem("Airline XYZ");
    airlineComboBox->addItem("Airline ABC");
    airlineComboBox->addItem("Airline DEF");
    airlineComboBox->setMinimumWidth(220);  // 设置最小宽度为 220px
    airlineComboBox->setStyleSheet("QComboBox { height: 80px; font-size: 16px; }");  // 增加高度，设置字体大小

    // 将筛选框添加到布局，并设置每个 comboBox 的伸缩因子
    filterLayout->addWidget(departureComboBox, 1);  // 1 表示占据 1 份空间
    filterLayout->addWidget(destinationComboBox, 1); // 1 表示占据 1 份空间
    filterLayout->addWidget(airlineComboBox, 1);     // 1 表示占据 1 份空间

    // 将筛选框布局添加到页面布局中
    pageLayout->addLayout(filterLayout);

    // 等待所有订单加载完成后再创建页面内容
    QTimer::singleShot(1000, this, [this, pageLayout, type, page, departureComboBox, destinationComboBox, airlineComboBox]() {
        auto &orderList = InterfaceManager::instance()->m_orderList;

        // 连接筛选框的信号槽
        connect(departureComboBox, &QComboBox::currentTextChanged, this, [this, pageLayout, type, departureComboBox, destinationComboBox, airlineComboBox]() {
            updateOrderPage(type, pageLayout, departureComboBox, destinationComboBox, airlineComboBox);
        });

        connect(destinationComboBox, &QComboBox::currentTextChanged, this, [this, pageLayout, type, departureComboBox, destinationComboBox, airlineComboBox]() {
            updateOrderPage(type, pageLayout, departureComboBox, destinationComboBox, airlineComboBox);
        });

        connect(airlineComboBox, &QComboBox::currentTextChanged, this, [this, pageLayout, type, departureComboBox, destinationComboBox, airlineComboBox]() {
            updateOrderPage(type, pageLayout, departureComboBox, destinationComboBox, airlineComboBox);
        });

        // 初次加载订单列表
        updateOrderPage(type, pageLayout, departureComboBox, destinationComboBox, airlineComboBox);
    });

    return page;
}





// 更新订单页面内容，根据筛选条件过滤订单
void orderwindow::updateOrderPage(const QString &type, QVBoxLayout *pageLayout, QComboBox *departureComboBox, QComboBox *destinationComboBox, QComboBox *airlineComboBox)
{
    // 清空现有的订单小部件
    qDeleteAll(pageLayout->children());

    // 获取筛选框的选择值
    QString departureFilter = departureComboBox->currentText();
    QString destinationFilter = destinationComboBox->currentText();
    QString airlineFilter = airlineComboBox->currentText();

    // 遍历订单列表，根据筛选条件过滤订单
    auto &orderList = InterfaceManager::instance()->m_orderList;
    for (const Order &order : orderList) {
        if ((type == "全部订单") ||
            (type == "待支付" && order.status() == Order::Pending) ||
            (type == "待出行" && order.status() == Order::Confirmed) ||
            (type == "已取消" && order.status() == Order::Cancelled)) {

            // 根据筛选条件过滤订单
            if ((departureFilter == "选择出发地" || order.m_departure == departureFilter) &&
                (destinationFilter == "选择目的地" || order.m_destination == destinationFilter) &&
                (airlineFilter == "选择航空公司" || order.m_airline == airlineFilter)) {

                // 创建 OrderWidget
                OrderWidget *orderWidget = new OrderWidget(order, pageLayout->parentWidget());

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

                // 将 OrderWidget 添加到布局
                pageLayout->addWidget(orderWidget);
            }
        }
    }
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

