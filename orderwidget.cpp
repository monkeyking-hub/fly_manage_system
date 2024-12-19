#include "orderwidget.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QSpacerItem>
#include <QWidget>
#include <QFrame>  // 用于横线分隔符

// OrderWidget 构造函数
OrderWidget::OrderWidget(const Order &order, QWidget *parent)
    : QWidget(parent), m_order(order)
{
    // 设置样式
    this->setObjectName("OrderWidget");
    this->setMinimumSize(1000, 300);

    this->setStyleSheet(
        "#OrderWidget {"
        "   background-color: rgba(255, 255, 255, 200);"
        "   border: 2px solid #cccccc;"
        "   border-radius: 8px;"
        "   margin: 10px;"
        "   padding: 8px;"
        "}"
        );

    // 创建垂直布局
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 订单号
    m_orderNumberLabel = new QLabel(QString("订单号: %1").arg(order.orderNumber()), this);
    m_orderNumberLabel->setStyleSheet("font-size: 24px; color: black; font-weight: bold;");
    layout->addWidget(m_orderNumberLabel);

    // 出行人
    QLabel *passengerLabel = new QLabel(QString("出行人: %1").arg(order.passenger()), this);
    passengerLabel->setStyleSheet("font-size: 20px; color: black;");
    layout->addWidget(passengerLabel);

    // 航空公司和航班号
    QLabel *airlineLabel = new QLabel(QString("航空公司: %1  航班号: %2")
                                          .arg(order.airline()).arg(order.flightNumber()), this);
    airlineLabel->setStyleSheet("font-size: 20px; color: black;");
    layout->addWidget(airlineLabel);

    // 飞机型号和仓位等级
    QLabel *aircraftLabel = new QLabel(QString("飞机型号: %1  仓位等级: %2")
                                           .arg(order.aircraftType()).arg(order.seatClass()), this);
    aircraftLabel->setStyleSheet("font-size: 20px; color: black;");
    layout->addWidget(aircraftLabel);

    // 起飞时间和到达时间
    QLabel *timeLabel = new QLabel(QString("起飞时间: %1  到达时间: %2")
                                       .arg(order.departureTime()).arg(order.arrivalTime()), this);
    timeLabel->setStyleSheet("font-size: 20px; color: black;");
    layout->addWidget(timeLabel);

    // 出发地和目的地
    QLabel *routeLabel = new QLabel(QString("出发地: %1  目的地: %2")
                                        .arg(order.departure()).arg(order.destination()), this);
    routeLabel->setStyleSheet("font-size: 20px; color: black;");
    layout->addWidget(routeLabel);

    // 舱位等级
    QLabel *seatClassLabel = new QLabel(QString("舱位等级: %1").arg(order.seatClass()), this);
    seatClassLabel->setStyleSheet("font-size: 20px; color: black;");
    layout->addWidget(seatClassLabel);

    // 金额
    QLabel *amountLabel = new QLabel(QString("金额: %1").arg(order.amount()), this);
    amountLabel->setStyleSheet("font-size: 20px; color: green; font-weight: bold;");
    layout->addWidget(amountLabel);


    // 状态
    QString statusString;
    switch (order.status()) {
    case Order::Pending:
        statusString = "待支付";
        break;
    case Order::Confirmed:
        statusString = "已确认";
        break;
    case Order::Completed:
        statusString = "已完成";
        break;
    case Order::Cancelled:
        statusString = "已取消";
        break;
    }

    QLabel *statusLabel = new QLabel(QString("订单状态: %1").arg(statusString), this);
    statusLabel->setStyleSheet("font-size: 20px; color: red;");
    layout->addWidget(statusLabel);

    // 分隔符（横线）
    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget(line);
}

const Order& OrderWidget::getOrder() const
{
    return m_order;
}

void OrderWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit orderClicked(m_order);
    }
    QWidget::mousePressEvent(event);
}

// 鼠标进入事件
void OrderWidget::enterEvent(QEnterEvent *event)
{
    m_isHovered = true;

    m_orderNumberLabel->setStyleSheet("font-size: 24px; color: blue; font-weight: bold;");

    this->setStyleSheet(
        "#OrderWidget {"
        "   background-color: rgba(173, 216, 230, 200);"
        "   border: 2px solid #cccccc;"
        "   border-radius: 8px;"
        "   margin: 10px;"
        "   padding: 8px;"
        "}"
        );

    QWidget::enterEvent(event);
}

// 鼠标离开事件
void OrderWidget::leaveEvent(QEvent *event)
{
    m_isHovered = false;

    m_orderNumberLabel->setStyleSheet("font-size: 24px; color: black; font-weight: bold;");

    this->setStyleSheet(
        "#OrderWidget {"
        "   background-color: rgba(255, 255, 255, 200);"
        "   border: 2px solid #cccccc;"
        "   border-radius: 8px;"
        "   margin: 10px;"
        "   padding: 8px;"
        "}"
        );

    QWidget::leaveEvent(event);
}
