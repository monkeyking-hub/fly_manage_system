// orderwidget.h
#ifndef ORDERWIDGET_H
#define ORDERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "order.h"

class OrderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OrderWidget(const Order &order, QWidget *parent = nullptr);

private:
    QLabel *m_orderNumberLabel;
    QLabel *m_passengerLabel;
    QLabel *m_bookingDateLabel;
    QLabel *m_amountLabel;
    QLabel *m_routeLabel;
};

#endif // ORDERWIDGET_H
