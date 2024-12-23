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
    int userId =currentUser.id;
    m_passengerName = currentUser.username;


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
                order.m_passenger = m_passengerName;  // 假设用户 ID 为乘客姓名
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

    // 创建 QVBoxLayout 用于容纳订单控件
    QVBoxLayout *pageLayout = new QVBoxLayout();
    pageLayout->setSpacing(20);  // 设置每个订单控件之间的间隙
    pageLayout->setContentsMargins(30, 30, 30, 30); // 设置页面边距

    // 创建 QScrollArea，包含订单列表的滚动区域
    QScrollArea *scrollArea = new QScrollArea(this);
    QWidget *scrollWidget = new QWidget(scrollArea);
    scrollWidget->setLayout(pageLayout);  // 将订单的布局添加到滚动区域的子控件中
    scrollArea->setWidget(scrollWidget);  // 设置滚动区域的子控件
    scrollArea->setWidgetResizable(true);  // 允许内容大小自适应调整

    // 设置滚动条样式（可选）
    scrollArea->setStyleSheet(
        "QScrollArea { "
        "   border: none; "
        "   background-color: #f0f0f0; "
        "}"
        "QScrollBar:vertical { "
        "   border: 2px solid #ccc; "
        "   background: #f0f0f0; "
        "   width: 12px; "
        "   margin: 22px 0 22px 0; "
        "   border-radius: 6px; "
        "} "
        "QScrollBar::handle:vertical { "
        "   background: #aaa; "
        "   min-height: 20px; "
        "   border-radius: 6px; "
        "} "
        "QScrollBar::handle:vertical:hover { "
        "   background: #888; "
        "} "
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { "
        "   background: none; "
        "   border: none; "
        "} "
        );

    // 创建筛选框（出发地、目的地、航空公司）
    QComboBox *departureComboBox = new QComboBox(this);
    QComboBox *destinationComboBox = new QComboBox(this);
    QComboBox *airlineComboBox = new QComboBox(this);


    // 设置初始筛选选项
    departureComboBox->addItem("所有出发地");
    destinationComboBox->addItem("所有目的地");
    airlineComboBox->addItem("所有航空公司");


    // 动态获取目的地、出发地和航空公司选项
    QSet<QString> departureSet, destinationSet, airlineSet;
    for (const Order &order : InterfaceManager::instance()->m_orderList) {
        departureSet.insert(order.departure());
        destinationSet.insert(order.destination());
        airlineSet.insert(order.airline());
    }

    // 将 QSet 转换为 QList 并添加到 ComboBox 中
    departureComboBox->addItems(QList<QString>(departureSet.begin(), departureSet.end()));
    destinationComboBox->addItems(QList<QString>(destinationSet.begin(), destinationSet.end()));
    // 将航空公司列表传递给 setupAirlineComboBox
    QList<QString> airlines(airlineSet.begin(), airlineSet.end());
    std::sort(airlines.begin(), airlines.end());
    setupAirlineComboBox(airlineComboBox, airlines);


    //筛选框样式
    departureComboBox->setStyleSheet(
        "QComboBox {"
        "    border: 2px solid #f3f3f3;"  /*设置线宽*/
        "    background-color: rgb(237, 242, 255);"  /*背景颜色*/
        "    border-radius: 10px;"  /*圆角*/
        "    padding: 1px 2px 1px 2px;"  /*针对于组合框中的文本内容*/
        "    font-size: 15px;"  /*增大字体大小*/
        "    text-align: bottom;"
        "    height: 30px;"
        "    min-width: 9em;"  /*组合框的最小宽度*/
        "    border-style: solid;"  /*边框为实线型*/
        "    border-width: 2px;"  /*边框宽度*/
        "    border-color: green;"  /*边框颜色*/
        "    padding-left: 10px;"  /*左侧边距*/
        "}"
        "QComboBox::drop-down {"
        "    subcontrol-origin: padding;"
        "    subcontrol-position: top right;"  /*放于右方顶部*/
        "    width: 50px;"  /*设置按钮范围宽度*/
        "    border-top-right-radius: 3px;"  /*设置边框圆角*/
        "    border-bottom-right-radius: 3px;"
        "}"
        "QComboBox::down-arrow {"
        "    border-image: url(:/newicon/qidian.png);"  /*自定义图片填充*/
        "    width: 27px;"  /*设置该图标的宽高*/
        "    height: 27px;"
        "}"
        "QComboBox QAbstractItemView {"
        "    border: 2px solid #f3f3f3;"  /*边框宽度、线形、颜色*/
        "    background-color: rgba(237, 242, 255, 1);"  /*背景颜色*/
        "    border-radius: 15px;"  /*圆角*/
        "    padding: 1px 2px 1px 2px;"  /*针对于组合框中的文本内容*/
        "    min-width: 9em;"  /*组合框的最小宽度*/
        "}"
        "QComboBox QAbstractItemView::item {"
        "    border-radius: 15px;"  /*圆角*/
        "    height: 30px;"  /*项的高度（设置pComboBox->setView(new QListView());后，该项才起作用）*/
        "    background-color: rgb(237, 242, 255);"
        "}"
        "QComboBox QAbstractItemView::item:hover {"
        "    color: #FFFFF0;"
        "    background-color: rgb(98, 0, 255);"  /*整个下拉窗体越过每项的背景色*/
        "}"
        "QComboBox::item:selected {"
        "    color: #FFFFF0;"
        "    background-color: rgb(0, 85, 200);"
        "}"
        );


    destinationComboBox->setStyleSheet(
        "QComboBox {"
        "    border: 2px solid #f3f3f3;"  /*设置线宽*/
        "    background-color: rgb(237, 242, 255);"  /*背景颜色*/
        "    border-radius: 10px;"  /*圆角*/
        "    padding: 1px 2px 1px 2px;"  /*针对于组合框中的文本内容*/
        "    font-size: 15px;"  /*增大字体大小*/
        "    text-align: bottom;"
        "    min-width: 9em;"  /*组合框的最小宽度*/
        "    height: 30px;"
        "    border-style: solid;"  /*边框为实线型*/
        "    border-width: 2px;"  /*边框宽度*/
        "    border-color: green;"  /*边框颜色*/
        "    padding-left: 10px;"  /*左侧边距*/
        "}"
        "QComboBox::drop-down {"
        "    subcontrol-origin: padding;"
        "    subcontrol-position: top right;"  /*放于右方顶部*/
        "    width: 50px;"  /*设置按钮范围宽度*/
        "    border-top-right-radius: 3px;"  /*设置边框圆角*/
        "    border-bottom-right-radius: 3px;"
        "}"
        "QComboBox::down-arrow {"
        "    border-image: url(:/newicon/zdian.png);"  /*自定义图片填充*/
        "    width: 27px;"  /*设置该图标的宽高*/
        "    height: 27px;"
        "}"
        "QComboBox QAbstractItemView {"
        "    border: 2px solid #f3f3f3;"  /*边框宽度、线形、颜色*/
        "    background-color: rgba(237, 242, 255, 1);"  /*背景颜色*/
        "    border-radius: 15px;"  /*圆角*/
        "    padding: 1px 2px 1px 2px;"  /*针对于组合框中的文本内容*/
        "    min-width: 9em;"  /*组合框的最小宽度*/
        "}"
        "QComboBox QAbstractItemView::item {"
        "    border-radius: 15px;"  /*圆角*/
        "    height: 30px;"  /*项的高度（设置pComboBox->setView(new QListView());后，该项才起作用）*/
        "    background-color: rgb(237, 242, 255);"
        "}"
        "QComboBox QAbstractItemView::item:hover {"
        "    color: #FFFFF0;"
        "    background-color: rgb(98, 0, 255);"  /*整个下拉窗体越过每项的背景色*/
        "}"
        "QComboBox::item:selected {"
        "    color: #FFFFF0;"
        "    background-color: rgb(0, 85, 200);"
        "}"
        );


    airlineComboBox->setStyleSheet(
        "QComboBox {"
        "    border: 2px solid #f3f3f3;"  /*设置线宽*/
        "    background-color: rgb(237, 242, 255);"  /*背景颜色*/
        "    height: 30px;"
        "    border-radius: 10px;"  /*圆角*/
        "    padding: 1px 2px 1px 2px;"  /*针对于组合框中的文本内容*/
        "    font-size: 15px;"  /*增大字体大小*/
        "    text-align: bottom;"
        "    min-width: 9em;"  /*组合框的最小宽度*/
        "    border-style: solid;"  /*边框为实线型*/
        "    border-width: 2px;"  /*边框宽度*/
        "    border-color: green;"  /*边框颜色*/
        "    padding-left: 10px;"  /*左侧边距*/
        "}"
        "QComboBox::drop-down {"
        "    subcontrol-origin: padding;"
        "    subcontrol-position: top right;"  /*放于右方顶部*/
        "    width: 50px;"  /*设置按钮范围宽度*/
        "    border-top-right-radius: 3px;"  /*设置边框圆角*/
        "    border-bottom-right-radius: 3px;"
        "}"
        "QComboBox::down-arrow {"
        "    border-image: url(:/newicon/plane.png);"  /*自定义图片填充*/
        "    width: 27px;"  /*设置该图标的宽高*/
        "    height: 27px;"
        "}"
        "QComboBox QAbstractItemView {"
        "    border: 2px solid #f3f3f3;"  /*边框宽度、线形、颜色*/
        "    background-color: rgba(237, 242, 255, 1);"  /*背景颜色*/
        "    border-radius: 15px;"  /*圆角*/
        "    padding: 1px 2px 1px 2px;"  /*针对于组合框中的文本内容*/
        "    min-width: 9em;"  /*组合框的最小宽度*/
        "}"
        "QComboBox QAbstractItemView::item {"
        "    border-radius: 15px;"  /*圆角*/
        "    height: 30px;"  /*项的高度（设置pComboBox->setView(new QListView());后，该项才起作用）*/
        "    background-color: rgb(237, 242, 255);"
        "}"
        "QComboBox QAbstractItemView::item:hover {"
        "    color: #FFFFF0;"
        "    background-color: rgb(98, 0, 255);"  /*整个下拉窗体越过每项的背景色*/
        "}"
        "QComboBox::item:selected {"
        "    color: #FFFFF0;"
        "    background-color: rgb(0, 85, 200);"
        "}"
        );


    // 将筛选框放在页面的顶部，使用 QHBoxLayout 横向排列
    QHBoxLayout *filterLayout = new QHBoxLayout();
    filterLayout->addWidget(departureComboBox);
    filterLayout->addWidget(destinationComboBox);
    filterLayout->addWidget(airlineComboBox);

    // 添加筛选框布局到页面布局
    QVBoxLayout *mainLayout = new QVBoxLayout(page);
    mainLayout->addLayout(filterLayout);  // 添加筛选框的横向布局
    mainLayout->addWidget(scrollArea);     // 添加滚动区域到页面布局

    // // 模拟订单数据
    // InterfaceManager::instance()->m_orderList.clear();
    // InterfaceManager::instance()->m_orderList.append(Order{
    //     "101", "张三", "2000", "上海", "北京", "南方航空", "CZ123", "2024-01-01 08:00",
    //     "2024-01-01 10:00", "空客A320", "经济舱", Order::Pending, "FL123", "A1", "首都机场",
    //     "浦东机场", 10, 50, 20, 3500.0, 1200.0, 2200.0
    // });
    // InterfaceManager::instance()->m_orderList.append(Order{
    //     "102", "李四", "1500", "广州", "深圳", "厦门航空", "MF456", "2024-01-02 09:00",
    //     "2024-01-02 10:30", "波音737", "商务舱", Order::Confirmed, "FL456", "B3", "宝安机场",
    //     "白云机场", 5, 60, 15, 5000.0, 2000.0, 3000.0
    // });

    // 获取订单列表
    auto &orderList = InterfaceManager::instance()->m_orderList;

    // 定义订单更新函数
    auto updateOrderDisplay = [this, pageLayout, departureComboBox, destinationComboBox, airlineComboBox](const QString &departure, const QString &destination, const QString &airline) {
        // 清空当前显示的订单
        QLayoutItem *item;
        while ((item = pageLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

        // 遍历订单列表，根据筛选条件显示订单
        for (const Order &order : InterfaceManager::instance()->m_orderList) {
            // 根据筛选条件过滤订单
            if ((departure == "所有出发地" || order.departure() == departure) &&
                (destination == "所有目的地" || order.destination() == destination) &&
                (airline == "所有航空公司" || order.airline() == airline)) {

                // 创建 OrderWidget 用于显示订单
                OrderWidget *orderWidget = new OrderWidget(order, this);  // 修改为传递父级控件为 scrollWidget

                // 设置 OrderWidget 的大小策略
                orderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

                // 为 OrderWidget 添加样式
                orderWidget->setStyleSheet(
                    "#OrderWidget {"
                    "   background-color: rgba(255, 255, 255, 200);"
                    "   border: 2px solid #cccccc; "
                    "   border-radius: 8px; "
                    "   margin: 5px;"  // 为每个 OrderWidget 设置 margin
                    "   padding: 8px;"
                    "} "
                    );

                // 连接 OrderWidget 的点击事件与槽
                connect(orderWidget, &OrderWidget::orderClicked, this, &orderwindow::showOrderDetails);

                // 将 OrderWidget 添加到布局
                pageLayout->addWidget(orderWidget);
            }
        }
    };

    // 初始化显示的订单
    updateOrderDisplay("所有出发地", "所有目的地", "所有航空公司");

    // 连接筛选框的变化信号
    connect(departureComboBox, &QComboBox::currentTextChanged, this, [updateOrderDisplay, departureComboBox, destinationComboBox, airlineComboBox](const QString &selectedDeparture) {
        updateOrderDisplay(selectedDeparture, destinationComboBox->currentText(), airlineComboBox->currentText());
    });
    connect(destinationComboBox, &QComboBox::currentTextChanged, this, [updateOrderDisplay, departureComboBox, destinationComboBox, airlineComboBox](const QString &selectedDestination) {
        updateOrderDisplay(departureComboBox->currentText(), selectedDestination, airlineComboBox->currentText());
    });
    connect(airlineComboBox, &QComboBox::currentTextChanged, this, [updateOrderDisplay, departureComboBox, destinationComboBox, airlineComboBox](const QString &selectedAirline) {
        updateOrderDisplay(departureComboBox->currentText(), destinationComboBox->currentText(), selectedAirline);
    });

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
