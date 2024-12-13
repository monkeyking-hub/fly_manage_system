#include "orderdetailwindow.h"
#include <QVBoxLayout>
#include <QLabel>

OrderDetailWindow::OrderDetailWindow(const Order &order, QWidget *parent)
    : QWidget(parent)
{
    setupUI(order);
}

void OrderDetailWindow::setupUI(const Order &order)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 使用 QLabel 显示订单详细信息
    layout->addWidget(new QLabel(QString("订单号: %1").arg(order.orderNumber()), this));
    layout->addWidget(new QLabel(QString("乘客: %1").arg(order.passenger()), this));
    layout->addWidget(new QLabel(QString("预订日期: %1").arg(order.bookingDate()), this));
    layout->addWidget(new QLabel(QString("金额: %1").arg(order.amount()), this));
    layout->addWidget(new QLabel(QString("行程: %1").arg(order.route()), this));

    setWindowTitle("订单详情");
    setLayout(layout);
}
