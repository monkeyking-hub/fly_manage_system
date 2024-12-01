#include "inorder.h"
#include "ui_inorder.h"
#include <QTabBar>
#include <QStackedWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>

inorder::inorder(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::inorder)
{
    ui->setupUi(this);

    // 创建 QStackedWidget 来管理页面
    stackedWidget = new QStackedWidget(this);

    // 创建各个页面
    QWidget *allOrdersPage = createPage("所有订单");
    QWidget *canceledOrdersPage = createPage("已取消订单");
    QWidget *pendingPaymentPage = createPage("待付款订单");
    QWidget *upcomingTripPage = createPage("待出行订单");

    // 将页面添加到 QStackedWidget 中
    stackedWidget->addWidget(allOrdersPage);
    stackedWidget->addWidget(canceledOrdersPage);
    stackedWidget->addWidget(pendingPaymentPage);
    stackedWidget->addWidget(upcomingTripPage);

    // 创建 QTabBar 作为导航栏
    tabBar = new QTabBar(this);
    tabBar->addTab("所有订单");
    tabBar->addTab("已取消订单");
    tabBar->addTab("待付款订单");
    tabBar->addTab("待出行订单");

    // 使用布局将组件添加到界面
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->centralwidget);
    mainLayout->addWidget(tabBar);
    mainLayout->addWidget(stackedWidget);

    // 信号槽：切换页面
    connect(tabBar, &QTabBar::currentChanged, stackedWidget, &QStackedWidget::setCurrentIndex);
}

inorder::~inorder()
{
    delete ui;
}

// 辅助函数：根据页面名称来创建不同内容的页面
QWidget *inorder::createPage(const QString &title) {
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);

    // 创建标题
    QLabel *label = new QLabel(title, page);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    // 根据 title 添加不同的控件
    if (title == "所有订单") {
        // 为所有订单页添加特定控件
        QPushButton *button = new QPushButton("查看所有订单", page);
        layout->addWidget(button);
        QTableWidget *table = new QTableWidget(5, 3, page);  // 5x3 的表格
        table->setHorizontalHeaderLabels({"订单号", "客户", "状态"});
        layout->addWidget(table);
    } else if (title == "已取消订单") {
        // 为已取消订单页添加特定控件
        QPushButton *button = new QPushButton("查看已取消订单", page);
        layout->addWidget(button);
        QTableWidget *table = new QTableWidget(3, 3, page);  // 3x3 的表格
        table->setHorizontalHeaderLabels({"订单号", "取消原因", "取消时间"});
        layout->addWidget(table);
    } else if (title == "待付款订单") {
        // 为待付款订单页添加特定控件
        QPushButton *button = new QPushButton("查看待付款订单", page);
        layout->addWidget(button);
        QTableWidget *table = new QTableWidget(4, 3, page);  // 4x3 的表格
        table->setHorizontalHeaderLabels({"订单号", "金额", "付款状态"});
        layout->addWidget(table);
    } else if (title == "待出行订单") {
        // 为待出行订单页添加特定控件
        QPushButton *button = new QPushButton("查看待出行订单", page);
        layout->addWidget(button);
        QTableWidget *table = new QTableWidget(2, 3, page);  // 2x3 的表格
        table->setHorizontalHeaderLabels({"订单号", "出行日期", "目的地"});
        layout->addWidget(table);
    }

    return page;
}

void inorder::setInitialTab(int index)
{
    if (index >= 0 && index < stackedWidget->count()) {  // 使用 stackedWidget 进行页签判断
        stackedWidget->setCurrentIndex(index); // 切换到指定页签
    }
}
