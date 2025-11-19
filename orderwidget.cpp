#include "orderwidget.h"
#include <QFrame> // 用于横线分隔符
#include <QLabel>
#include <QMouseEvent>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QWidget>

// OrderWidget 构造函数
OrderWidget::OrderWidget(const Order &order, QWidget *parent)
    : QWidget(parent)
    , m_order(order)
{
    // 设置样式
    this->setObjectName("OrderWidget");
    this->setMinimumSize(1000, 300);

    this->setStyleSheet(
        "#OrderWidget {"
        "   background-color: rgba(255, 255, 255, 200);"
        "   border: 2px solid #cccccc; "
        "   border-radius: 8px;"
        "   margin: 10px;"
        "   padding: 8px;"
        "}"
        );

    // 创建垂直布局
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 订单号
    m_orderNumberLabel = new QLabel(QString("订单号: %1").arg(order.m_orderNumber), this);
    m_orderNumberLabel->setStyleSheet("font-size: 24px; color: black; font-weight: bold;");
    layout->addWidget(m_orderNumberLabel);

    // 出行人
    QLabel *passengerLabel = new QLabel(QString("出行人: %1").arg(order.m_passenger), this);
    passengerLabel->setStyleSheet("font-size: 20px; color: black;");
    layout->addWidget(passengerLabel);

    // 航空公司和航班号
    QLabel *airlineLabel = new QLabel(QString("航空公司: %1  航班号: %2")
                                          .arg(order.m_airline).arg(order.m_flightNumber), this);
    airlineLabel->setStyleSheet("font-size: 20px; color: black;");
    layout->addWidget(airlineLabel);

    // 飞机型号和仓位等级
    QLabel *aircraftLabel = new QLabel(QString("飞机型号: %1  舱位等级: %2")
                                           .arg(order.m_aircraftType).arg(order.m_seatClass), this);
    aircraftLabel->setStyleSheet("font-size: 20px; color: black;");
    layout->addWidget(aircraftLabel);

    // 起飞时间和到达时间
    QLabel *timeLabel = new QLabel(QString("起飞时间: %1  到达时间: %2")
                                       .arg(order.m_departureTime).arg(order.m_arrivalTime), this);
    timeLabel->setStyleSheet("font-size: 20px; color: black;");
    layout->addWidget(timeLabel);

    // 出发地和目的地
    QLabel *routeLabel = new QLabel(QString("出发地: %1  目的地: %2")
                                        .arg(order.m_departure).arg(order.m_destination), this);
    routeLabel->setStyleSheet("font-size: 20px; color: black;");
    layout->addWidget(routeLabel);

    // 出发机场和到达机场
    QLabel *airportLabel = new QLabel(QString("出发机场: %1  到达机场: %2")
                                          .arg(order.m_departureAirport).arg(order.m_arrivalAirport), this);
    airportLabel->setStyleSheet("font-size: 20px; color: black;");
    layout->addWidget(airportLabel);

    // 金额
    QLabel *amountLabel = new QLabel(QString("金额: %1").arg(order.m_amount), this);
    amountLabel->setStyleSheet("font-size: 20px; color: green; font-weight: bold;");
    layout->addWidget(amountLabel);

    // 状态
    QString statusString=order.m_status;

    QLabel *statusLabel = new QLabel(QString("订单状态: %1").arg(statusString), this);
    statusLabel->setStyleSheet("font-size: 20px; color: red;");
    layout->addWidget(statusLabel);

    // 分隔符（横线）
    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget(line);
}

const Order &OrderWidget::getOrder() const
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
void OrderWidget::enterEvent(QEvent *event)
{
    if (event->type() == QEvent::Enter) {
        m_isHovered = true;

        m_orderNumberLabel->setStyleSheet("font-size: 24px; color: blue; font-weight: bold;");

        this->setStyleSheet(
            "#OrderWidget {"
            "   background-color: rgba(173, 216, 230, 200);"
            "   border: 2px solid #cccccc; "
            "   border-radius: 8px;"
            "   margin: 10px;"
            "   padding: 8px;"
            "}"
            );
    }

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
        "   border: 2px solid #cccccc; "
        "   border-radius: 8px;"
        "   margin: 10px;"
        "   padding: 8px;"
        "}"
        );

    QWidget::leaveEvent(event);
}
