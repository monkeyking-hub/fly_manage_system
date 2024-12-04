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
            min-width: 253px;               /* 按钮宽度 */
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




// 创建每个订单页面的函数
QWidget* orderwindow::createOrderPage(const QString &type)
{
    QWidget *page = new QWidget(this);
    QVBoxLayout *pageLayout = new QVBoxLayout(page);

    // 订单类型过滤标签
    QLabel *label = new QLabel(QString("订单类型：%1").arg(type), page);
    label->setStyleSheet("font-size: 18px; font-weight: bold; color: black;");
    pageLayout->addWidget(label);

    // 创建一个 QScrollArea 用于滚动订单列表
    QScrollArea *scrollArea = new QScrollArea(page);
    QWidget *container = new QWidget(scrollArea);
    QVBoxLayout *containerLayout = new QVBoxLayout(container);

    // 模拟一些订单数据（实际应该从数据库加载）
    QList<Order> orders;
    orders.append(Order("33533359121", "冯泽加, 王沐臣", "2024-05-30", "¥1240", "珠海", "合肥", Order::Upcoming));
    orders.append(Order("33533359122", "李阳, 陈珂", "2024-05-31", "¥300", "广州", "上海", Order::Pending));
    orders.append(Order("33533359123", "刘鑫, 张浩", "2024-06-01", "¥800", "北京", "深圳", Order::Canceled));

    // 为每个订单创建 OrderWidget
    for (const Order &order : orders) {
        OrderWidget *orderWidget = new OrderWidget(order, container);
        containerLayout->addWidget(orderWidget);  // 将订单加入布局
    }

    // 设置滚动区域
    scrollArea->setWidget(container);
    pageLayout->addWidget(scrollArea);  // 将 QScrollArea 加入页面布局

    return page;
}
