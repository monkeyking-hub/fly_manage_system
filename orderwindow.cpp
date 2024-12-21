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
    // 假设通过 API 获取订单 ID 列表
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);  // 创建一个局部的 manager

    QNetworkRequest request(QUrl("/api/orders/search"));
    QJsonObject requestBody;
    requestBody["userId"] = userId;
    QNetworkReply *reply = manager->post(request, QJsonDocument(requestBody).toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply, userId]() {
        if (reply->error() == QNetworkReply::NoError) {
            // 解析返回的订单数据
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonArray ordersArray = doc.object()["data"].toArray();
            QList<QString> flightNumbers;

            // 遍历订单数据，初始化每个 Order 对象
            for (const QJsonValue &value : ordersArray) {
                QJsonObject orderData = value.toObject();

                QString orderId = orderData["id"].toString();
                QString flightNumber = orderData["flightNumber"].toString();
                double price = orderData["price"].toDouble();

                Order order;
                order.m_orderNumber = orderId;
                order.m_passenger = QString::number(userId); // 用户 ID 作为乘客姓名
                order.m_amount = QString::number(price);
                order.m_flightNumber = flightNumber;

                // 设置订单的状态，默认设为 Pending
                order.m_status = Order::Pending;

                // 将初始化的 Order 添加到全局列表
                InterfaceManager::instance()->m_orderList.append(order);

                flightNumbers.append(flightNumber); // 存储航班号
            }

            // 调用第二个函数根据航班号查询详细信息
            fetchOrderDetails(flightNumbers);  // 修改这里，传入 flightNumbers
        }

        reply->deleteLater(); // 确保删除 reply，避免内存泄漏
    });
}



//这个根据上一个函数获得的航班号获取订单的详细信息
void orderwindow::fetchOrderDetails(const QList<QString> &flightNumbers)
{
    // 获取容器布局，如果需要的话，可以在此函数内创建布局
    QVBoxLayout *containerLayout = containerLayout;  // 这里替换为实际的布局

    // 遍历每个航班号，获取相应的航班详细信息
    for (const QString &flightNumber : flightNumbers) {
        // 使用 API 获取航班详细信息填充订单
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QNetworkRequest request(QUrl(QString("/api/flight/details/%1").arg(flightNumber)));
        QNetworkReply *reply = manager->get(request);

        // 使用 lambda 函数处理网络请求结果
        connect(reply, &QNetworkReply::finished, this, [this, reply, flightNumber, containerLayout]() {
            if (reply->error() == QNetworkReply::NoError) {
                // 解析航班详细信息
                QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
                QJsonObject flightDetails = doc.object();

                // 查找对应的订单
                for (Order &order : InterfaceManager::instance()->m_orderList) {
                    if (order.m_flightNumber == flightNumber) {
                        // 更新订单详细信息
                        order.m_departure = flightDetails["departure"].toString();
                        order.m_destination = flightDetails["destination"].toString();
                        order.m_airline = flightDetails["airlineCompany"].toString();
                        order.m_departureTime = flightDetails["departureTime"].toString();
                        order.m_arrivalTime = flightDetails["arrivalTime"].toString();
                        order.m_aircraftType = flightDetails["aircraftModel"].toString();
                        order.m_seatClass = flightDetails["seatType"].toString();
                        order.m_flightId = flightDetails["id"].toString();
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
            }

            reply->deleteLater(); // 确保删除 reply，避免内存泄漏
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
    departureComboBox->setStyleSheet("QComboBox { height: 40px; font-size: 16px; }");  // 增加高度，设置字体大小

    // 目的地筛选框
    QComboBox *destinationComboBox = new QComboBox(this);
    destinationComboBox->addItem("选择目的地");
    // 假设你有一个目的地列表，可以通过 API 或本地数据填充
    destinationComboBox->addItem("Los Angeles");
    destinationComboBox->addItem("San Francisco");
    destinationComboBox->addItem("New York");
    destinationComboBox->setMinimumWidth(220);  // 设置最小宽度为 220px
    destinationComboBox->setStyleSheet("QComboBox { height: 40px; font-size: 16px; }");  // 增加高度，设置字体大小

    // 航空公司筛选框
    QComboBox *airlineComboBox = new QComboBox(this);
    airlineComboBox->addItem("选择航空公司");
    // 假设你有一个航空公司列表，可以通过 API 或本地数据填充
    airlineComboBox->addItem("Airline XYZ");
    airlineComboBox->addItem("Airline ABC");
    airlineComboBox->addItem("Airline DEF");
    airlineComboBox->setMinimumWidth(220);  // 设置最小宽度为 220px
    airlineComboBox->setStyleSheet("QComboBox { height: 40px; font-size: 16px; }");  // 增加高度，设置字体大小

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

