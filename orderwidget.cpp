// orderwidget.cpp
#include "orderwidget.h"

OrderWidget::OrderWidget(const Order &order, QWidget *parent)
    : QWidget(parent)
{
    // 设置这个订单块的样式
    this->setStyleSheet(
        "QWidget {"
        "   background-color: rgba(255, 255, 255, 200);"
        "   border-radius: 8px;"
        "   margin: 5px;"
        "   padding: 8px;"
        "}"
        );

    // 创建布局
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 订单号
    m_orderNumberLabel = new QLabel(QString("订单号: %1").arg(order.orderNumber()), this);
    m_orderNumberLabel->setStyleSheet("font-size: 12px; color: black; font-weight: bold;");
    layout->addWidget(m_orderNumberLabel);

    // 出行人
    m_passengerLabel = new QLabel(QString("出行人: %1").arg(order.passenger()), this);
    m_passengerLabel->setStyleSheet("font-size: 12px; color: black;");
    layout->addWidget(m_passengerLabel);

    // 预定日期
    m_bookingDateLabel = new QLabel(QString("预定日期: %1").arg(order.bookingDate()), this);
    m_bookingDateLabel->setStyleSheet("font-size: 12px; color: black;");
    layout->addWidget(m_bookingDateLabel);

    // 金额
    m_amountLabel = new QLabel(QString("金额: %1").arg(order.amount()), this);
    m_amountLabel->setStyleSheet("font-size: 12px; color: black;");
    layout->addWidget(m_amountLabel);

    // 路线
    m_routeLabel = new QLabel(QString("出发地 - 目的地: %1").arg(order.route()), this);
    m_routeLabel->setStyleSheet("font-size: 12px; color: black;");
    layout->addWidget(m_routeLabel);
}
