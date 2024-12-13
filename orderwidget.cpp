#include "orderwidget.h"
#include <QLabel>
#include <QVBoxLayout>
<<<<<<< Updated upstream
#include <QMouseEvent>
=======
#include <QSpacerItem>
#include <QWidget>
#include <QFrame>  // 用于横线分隔符
>>>>>>> Stashed changes

// OrderWidget 构造函数
OrderWidget::OrderWidget(const Order &order, QWidget *parent)
    : QWidget(parent), m_order(order)
{
    // 设置这个订单块的样式，添加边框
    this->setObjectName("OrderWidget"); // 设置对象名称
    this->setMinimumSize(1000, 300); // 设置最小宽度为 400，高度为 200

    // 设置样式
    this->setStyleSheet(
        "#OrderWidget {"
        "   background-color: rgba(255, 255, 255, 200);"
<<<<<<< Updated upstream
        "   border: 2px solid #cccccc;"  // 添加灰色边框
        "   border-radius: 8px;"         // 圆角边框
        "   margin: 10px;"               // 外边距，控制组件间间隔
        "   padding: 8px;"               // 内边距，内容和边框的距离
        "}"
        "#OrderWidget:hover {"
        "   background-color: rgba(173, 216, 230, 200);"  // 淡蓝色背景
        "}"
=======
        "   border-radius: 8px;"
        "   margin: 5px;"
        "   padding: 8px;"
        "} "
>>>>>>> Stashed changes
        );

    // 创建垂直布局
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 订单号
    m_orderNumberLabel = new QLabel(QString("订单号: %1").arg(order.orderNumber()), this);
    m_orderNumberLabel->setStyleSheet("font-size: 24px; color: black; font-weight: bold;");
    layout->addWidget(m_orderNumberLabel);

    // 出行人
    m_passengerLabel = new QLabel(QString("出行人: %1").arg(order.passenger()), this);
    m_passengerLabel->setStyleSheet("font-size: 20px; color: black;");
    layout->addWidget(m_passengerLabel);

    // 预定日期
    m_bookingDateLabel = new QLabel(QString("预定日期: %1").arg(order.bookingDate()), this);
    m_bookingDateLabel->setStyleSheet("font-size: 20px; color: black;");
    layout->addWidget(m_bookingDateLabel);

    // 金额
    m_amountLabel = new QLabel(QString("金额: %1").arg(order.amount()), this);
    m_amountLabel->setStyleSheet("font-size: 20px; color: black;");
    layout->addWidget(m_amountLabel);

    // 路线
    m_routeLabel = new QLabel(QString("出发地 - 目的地: %1").arg(order.route()), this);
    m_routeLabel->setStyleSheet("font-size: 20px; color: black;");
    layout->addWidget(m_routeLabel);

    // 状态
    QString statusString;
    switch (order.status()) {
    case Order::Pending:
        statusString = "待支付";
        break;
    case Order::Upcoming:
        statusString = "待出行";
        break;
    case Order::Canceled:
        statusString = "已取消";
        break;
    }
<<<<<<< Updated upstream
    QLabel *statusLabel = new QLabel(QString("状态: %1").arg(statusString), this);
    statusLabel->setStyleSheet("font-size: 16px; color: red;");
    layout->addWidget(statusLabel);

    // 设置布局
    setLayout(layout);
=======
    m_statusLabel = new QLabel(QString("状态: %1").arg(statusString), this);
    m_statusLabel->setStyleSheet("font-size: 12px; color: red;");
    layout->addWidget(m_statusLabel);

    // 分隔符（横线）
    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget(line);

    // 为了确保订单块之间有间隔，我们加入一个 spacer
    QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    layout->addItem(spacer);
>>>>>>> Stashed changes
}

const Order& OrderWidget::getOrder() const
{
    return m_order;
}

void OrderWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit orderClicked(m_order); // 发射点击信号
    }
    QWidget::mousePressEvent(event); // 保持原有行为
}

// 鼠标进入事件
void OrderWidget::enterEvent(QEnterEvent *event)
{
    m_isHovered = true;

    // 设置订单号为蓝色
    m_orderNumberLabel->setStyleSheet("font-size: 24px; color: blue; font-weight: bold;");

    // 设置整个订单组件背景为淡蓝色
    this->setStyleSheet(
        "#OrderWidget {"
        "   background-color: rgba(173, 216, 230, 200);;"  // 淡蓝色背景
        "   border: 2px solid #cccccc;"
        "   border-radius: 8px;"
        "   margin: 10px;"
        "   padding: 8px;"
        "}"
        );

    QWidget::enterEvent(event);  // 调用父类的事件处理
}


// 鼠标离开事件
void OrderWidget::leaveEvent(QEvent *event)
{
    m_isHovered = false;

    // 恢复订单号颜色为黑色
    m_orderNumberLabel->setStyleSheet("font-size: 24px; color: black; font-weight: bold;");

    // 恢复整个订单组件背景为白色
    this->setStyleSheet(
        "#OrderWidget {"
        "   background-color: rgba(255, 255, 255, 200);"  // 恢复为白色背景
        "   border: 2px solid #cccccc;"
        "   border-radius: 8px;"
        "   margin: 10px;"
        "   padding: 8px;"
        "}"
        );

    QWidget::leaveEvent(event);  // 调用父类的事件处理
}

