#ifndef ORDERWIDGET_H
#define ORDERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include "order.h"
#include <QEvent>  // 确保包含了 QEvent 头文件

class OrderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OrderWidget(const Order &order, QWidget *parent = nullptr);

    const Order& getOrder() const;

signals:
    void orderClicked(const Order &order);

protected:
    void mousePressEvent(QMouseEvent *event) override;   // 鼠标点击事件
    void enterEvent(QEnterEvent *event) override;   // 正确签名：QEnterEvent
    void leaveEvent(QEvent *event) override;

private:
    QLabel *m_orderNumberLabel;
    QLabel *m_passengerLabel;
    QLabel *m_bookingDateLabel;
    QLabel *m_amountLabel;
    QLabel *m_routeLabel;
<<<<<<< Updated upstream
    Order m_order;
    bool m_isHovered;  // 标识是否悬停
=======
    QLabel *m_statusLabel;      // 显示状态
>>>>>>> Stashed changes
};

#endif // ORDERWIDGET_H
