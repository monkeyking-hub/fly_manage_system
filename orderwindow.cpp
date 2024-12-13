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

    // 订单类型过滤标签
    QLabel *label = new QLabel(QString("订单类型：%1").arg(type), page);
    label->setStyleSheet("font-size: 18px; font-weight: bold; color: black;");
    pageLayout->addWidget(label);

    // 创建一个 QScrollArea 用于滚动订单列表
    QScrollArea *scrollArea = new QScrollArea(page);
    QWidget *container = new QWidget(scrollArea);
    QVBoxLayout *containerLayout = new QVBoxLayout(container);

    // 模拟一些订单数据（实际应该从数据库加载）
<<<<<<< Updated upstream
    QList<Order> allOrders;
    allOrders.append(Order("33533359121", "冯泽加, 王沐臣", "2024-05-30", "¥1240", "珠海", "合肥", Order::Upcoming));
    allOrders.append(Order("33533359122", "李阳, 陈珂", "2024-05-31", "¥300", "广州", "上海", Order::Pending));
    allOrders.append(Order("33533359123", "刘鑫, 张浩", "2024-06-01", "¥800", "北京", "深圳", Order::Canceled));
    allOrders.append(Order("33533359124", "王强, 周婷", "2024-06-02", "¥450", "杭州", "武汉", Order::Upcoming));
    allOrders.append(Order("33533359125", "张丽, 李超", "2024-06-03", "¥980", "南京", "成都", Order::Pending));
    allOrders.append(Order("33533359126", "陈华, 杨洋", "2024-06-04", "¥1200", "深圳", "西安", Order::Upcoming));
    allOrders.append(Order("33533359127", "赵敏, 何峰", "2024-06-05", "¥600", "重庆", "长沙", Order::Canceled));
    allOrders.append(Order("33533359128", "黄磊, 孙悦", "2024-06-06", "¥500", "昆明", "厦门", Order::Pending));
    allOrders.append(Order("33533359129", "周杰, 方媛", "2024-06-07", "¥720", "青岛", "福州", Order::Upcoming));
    allOrders.append(Order("33533359130", "李敏, 张辉", "2024-06-08", "¥850", "天津", "贵阳", Order::Pending));
    allOrders.append(Order("33533359131", "杨涛, 王梅", "2024-06-09", "¥650", "哈尔滨", "济南", Order::Canceled));
    allOrders.append(Order("33533359132", "徐阳, 邓雪", "2024-06-10", "¥780", "南昌", "乌鲁木齐", Order::Upcoming));
    allOrders.append(Order("33533359133", "邵东, 郑玉", "2024-06-11", "¥900", "拉萨", "香港", Order::Pending));
    allOrders.append(Order("33533359134", "韩军, 张晓", "2024-06-12", "¥1100", "兰州", "海口", Order::Upcoming));
=======

    QList<Order> orders;

    // 添加一些待出行的订单
    orders.append(Order("33533359121", "冯泽加, 王沐臣", "2024-05-30", "¥1240", "珠海", "合肥", Order::Upcoming));
    orders.append(Order("33533359122", "李阳, 陈珂", "2024-05-31", "¥300", "广州", "上海", Order::Upcoming));

    // 添加一些待支付的订单
    orders.append(Order("33533359123", "刘鑫, 张浩", "2024-06-01", "¥800", "北京", "深圳", Order::Pending));
    orders.append(Order("33533359124", "王颖, 刘敏", "2024-06-02", "¥150", "成都", "武汉", Order::Pending));

    // 添加一些已取消的订单
    orders.append(Order("33533359125", "张伟, 王敏", "2024-06-03", "¥500", "天津", "南京", Order::Canceled));
    orders.append(Order("33533359126", "陈宇, 张婷", "2024-06-04", "¥200", "西安", "成都", Order::Canceled));

>>>>>>> Stashed changes

    // 根据类型筛选订单
    QList<Order> filteredOrders;
    for (const Order &order : allOrders) {
        if (type == "全部订单" ||
            (type == "待支付" && order.status() == Order::Pending) ||
            (type == "待出行" && order.status() == Order::Upcoming) ||
            (type == "已取消" && order.status() == Order::Canceled)) {
            filteredOrders.append(order);
        }
    }

    // 动态生成订单组件
    for (int i = 0; i < filteredOrders.size(); ++i) {
        OrderWidget *orderWidget = new OrderWidget(filteredOrders[i], container);
        // 连接点击信号到槽函数
         connect(orderWidget, &OrderWidget::orderClicked, this, &orderwindow::showOrderDetails); // 连接点击信号到槽函数

        // 为 OrderWidget 添加样式
        orderWidget->setStyleSheet(
            "#OrderWidget {"
            "   background-color: rgba(255, 255, 255, 200);"
            "   border: 2px solid #cccccc;"
            "   border-radius: 8px;"
            "   margin: 10px;"
            "   padding: 8px;"
            "}"
            );

        containerLayout->addWidget(orderWidget);

        // 如果不是最后一个订单，添加分割线
        if (i < filteredOrders.size() - 1) {
            QWidget *line = new QWidget(container);
            line->setFixedHeight(2);
            line->setStyleSheet("background-color: black;");
            line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

            containerLayout->addWidget(line);
        }
    }

    // 设置滚动区域
    scrollArea->setWidget(container);
    pageLayout->addWidget(scrollArea);

    return page;
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

