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
#include <QMainWindow>
#include <QPushButton>    // 添加这行
#include <QToolBar>       // 添加这行
#include <QTabWidget>

orderwindow::orderwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::order_2)
{
    ui->setupUi(this);
    ui->cs->setObjectName("centralWidget");

    // 创建并初始化 tabWidget
    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    // 创建刷新按钮
    refreshButton = new QPushButton("刷新订单", this);
    refreshButton->setStyleSheet(R"(
        QPushButton {
            background-color: #4CAF50;
            border: none;
            color: white;
            padding: 10px 20px;
            text-align: center;
            text-decoration: none;
            font-size: 16px;
            margin: 4px 2px;
            border-radius: 8px;
        }
        QPushButton:hover {
            background-color: #45a049;
        }
    )");

    // 创建工具栏并添加刷新按钮
    QToolBar *toolbar = addToolBar("工具栏");
    toolbar->addWidget(refreshButton);

    // 连接刷新按钮的信号
    connect(refreshButton, &QPushButton::clicked,
            this, &orderwindow::onRefreshButtonClicked);
}

// 将原来的 onCommandLinkButton5Clicked 函数内容移到这个新函数中
void orderwindow::onRefreshButtonClicked()
{
    qDebug() << "刷新订单界面";

    // 清除现有的标签页
    tabWidget->clear();

    // 获取当前用户信息
    int userId = UserManager::getInstance()->getCurrentUser().id;
    m_passengerName = UserManager::getInstance()->getCurrentUser().username;

    // 获取所有订单 ID 和航班号，并获取详细信息
    fetchOrders(userId);


    // 创建每个页面
    QWidget *allOrdersPage = createOrderPage("全部订单");
    QWidget *pendingPage = createOrderPage("未支付");
    QWidget *upcomingPage = createOrderPage("已支付");
    QWidget *canceledPage = createOrderPage("已退款");

    // 添加到 TabWidget
    tabWidget->addTab(allOrdersPage, QIcon(":/order.png"), "全部订单");
    tabWidget->addTab(pendingPage, QIcon(":/unpayorder.png"), "未支付");
    tabWidget->addTab(upcomingPage, QIcon(":/bag.png"), "已支付");
    tabWidget->addTab(canceledPage, QIcon(":/deletedorder.png"), "已退款");

    // 设置 tab 的样式
    QString tabStyle = R"(
        QTabBar::tab {
            min-height: 50px;
            min-width: 266px;
            font-size: 24px;
            font-weight: bold;
            color: black;
            padding: 5px;
            border: 2px solid #000;
            border-radius: 15px;
            background-color: rgba(255, 255, 255, 200);
        }
        QTabBar::tab:hover {
            background-color: rgba(0, 0, 255, 50);
            color: white;
        }
        QTabBar::tab:selected {
            background-color: rgba(0, 0, 255, 100);
            color: white;
        }
    )";
    tabWidget->tabBar()->setStyleSheet(tabStyle);
    tabWidget->tabBar()->setIconSize(QSize(40, 40));

}


orderwindow::~orderwindow()
{
    delete ui;
}

void orderwindow::fetchOrders(int userId) {

    InterfaceManager::instance()->m_orderList.clear();
     QEventLoop eventLoop;
    // 创建网络访问管理器
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);


    // 设置请求 URL
    // QNetworkRequest request(QUrl("http://192.168.110.12:8080/api/orders/search"));
    QNetworkRequest request(QUrl("http://127.0.0.1:8080/api/orders/search"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    qDebug() << "发送请求获取订单信息";

    // 构建请求体，包含用户 ID
    QJsonObject requestBody;
    requestBody["userId"] = userId;
    qDebug() << userId;

    // 转换为 QByteArray 并发送 POST 请求
    QByteArray jsonData = QJsonDocument(requestBody).toJson();
    QNetworkReply *reply = manager->post(request, jsonData);
    // 连接事件循环到网络请求完成信号
    connect(reply, &QNetworkReply::finished, this, [&eventLoop]() {
        // 创建一个单次触发的定时器
        QTimer::singleShot(1000, &eventLoop, &QEventLoop::quit);  // 1000毫秒 = 1秒
    });

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
                order.m_passenger = m_passengerName;
                order.m_amount = QString::number(orderData["price"].toDouble());
                order.m_flightNumber = orderData["flightNumber"].toString();
                order.m_status = orderData["status"].toString();
                if(orderData["status"].toString()=="Pending payment")order.m_status ="已支付";
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

        // 确保释放reply对象
        reply->deleteLater();
    });
    eventLoop.exec();
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

    // // 模拟订单数据
    // InterfaceManager::instance()->m_orderList.clear();
    // InterfaceManager::instance()->m_orderList.append(Order{
    //     "101", "张三", "2000", "上海", "北京", "南方航空", "CZ123", "2024-01-01 08:00",
    //     "2024-01-01 10:00", "空客A320", "经济舱", "已支付", "FL123", "A1", "首都机场",
    //     "浦东机场", 10, 50, 20, 3500.0, 1200.0, 2200.0
    // });
    // InterfaceManager::instance()->m_orderList.append(Order{
    //     "102", "李四", "1500", "广州", "深圳", "厦门航空", "MF456", "2024-01-02 09:00",
    //     "2024-01-02 10:30", "波音737", "商务舱", "未支付", "FL456", "B3", "宝安机场",
    //     "白云机场", 5, 60, 15, 5000.0, 2000.0, 3000.0
    // });

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



    // 获取订单列表
    auto &orderList = InterfaceManager::instance()->m_orderList;

    // 定义订单更新函数，增加 type 参数，用于根据页面类型显示不同的订单状态，注意，这相当于一个独立的函数
    auto updateOrderDisplay = [this, pageLayout, departureComboBox, destinationComboBox, airlineComboBox](const QString &departure, const QString &destination, const QString &airline, const QString &type) {
        // 清空当前显示的订单
        QLayoutItem *item;
        if (InterfaceManager::instance()->m_orderList.empty()) {
            qDebug() << "当前为空数组";
        }
        while ((item = pageLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

        // 遍历订单列表，根据订单状态先筛选
        for (const Order &order : InterfaceManager::instance()->m_orderList) {
            bool statusMatch = false;

            // 根据状态筛选订单
            if (type == "未支付" && order.status() == "未支付") {
                statusMatch = true;
            }
            else if (type == "已支付" && order.status() == "已支付") {
                statusMatch = true;
            }
            else if (type == "已退款" && order.status() == "已退款") {
                statusMatch = true;
            }
            else if (type == "全部订单") {
                statusMatch = true;
            }

            // 如果订单状态不匹配，跳过此订单
            if (!statusMatch) {
                continue;
            }

            // 订单状态匹配后，根据出发地、目的地、航空公司进行进一步筛选
            bool match = (departure == "所有出发地" || order.departure() == departure) &&
                         (destination == "所有目的地" || order.destination() == destination) &&
                         (airline == "所有航空公司" || order.airline() == airline);

            // 如果订单满足所有筛选条件，创建 OrderWidget 并显示
            if (match) {
                qDebug() << "现在开始显示符合条件的订单";
                qDebug() << "Order Status: " << order.status() << ", Type: " << type;

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
                connect(orderWidget, &OrderWidget::orderClicked, this, &orderwindow::showOrderDetails);
                pageLayout->addWidget(orderWidget);
            }
        }
    };





    // 初始化显示的订单
    updateOrderDisplay("所有出发地", "所有目的地", "所有航空公司",type);

    // 连接筛选框的变化信号
    connect(departureComboBox, &QComboBox::currentTextChanged, this, [updateOrderDisplay, departureComboBox, destinationComboBox, airlineComboBox, type](const QString &selectedDeparture) {
        updateOrderDisplay(selectedDeparture, destinationComboBox->currentText(), airlineComboBox->currentText(), type);
    });

    connect(destinationComboBox, &QComboBox::currentTextChanged, this, [updateOrderDisplay, departureComboBox, destinationComboBox, airlineComboBox,type](const QString &selectedDestination) {
        updateOrderDisplay(departureComboBox->currentText(), selectedDestination, airlineComboBox->currentText(), type);
    });
    connect(airlineComboBox, &QComboBox::currentTextChanged, this, [updateOrderDisplay, departureComboBox, destinationComboBox, airlineComboBox,type](const QString &selectedAirline) {
        updateOrderDisplay(departureComboBox->currentText(), destinationComboBox->currentText(), selectedAirline, type);
    });

    // 返回页面
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
