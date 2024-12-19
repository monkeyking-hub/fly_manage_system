#ifndef ORDERWIDGET_H
#define ORDERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include "order.h"
#include <QEvent>  // 确保包含了 QEvent 头文件



class QLabel;

class OrderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OrderWidget(const Order &order, QWidget *parent = nullptr);

    // 获取当前订单数据
    const Order& getOrder() const;

signals:
    // 当订单被点击时发出信号
    void orderClicked(const Order &order);

protected:
    // 鼠标点击事件处理
    void mousePressEvent(QMouseEvent *event) override;

    // 鼠标进入和离开事件处理
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    // 订单数据
    Order m_order;

    // UI 显示的标签
    QLabel *m_orderNumberLabel;  // 订单号标签
    QLabel *m_passengerLabel;    // 出行人标签
    QLabel *m_airlineLabel;      // 航空公司和航班号标签
    QLabel *m_aircraftLabel;     // 飞机型号和仓位等级标签
    QLabel *m_timeLabel;         // 起飞和到达时间标签
    QLabel *m_routeLabel;        // 出发地和目的地标签
    QLabel *m_amountLabel;       // 金额标签
    QLabel *m_statusLabel;       // 订单状态标签

    // 鼠标悬停状态
    bool m_isHovered = false;
};

#endif // ORDERWIDGET_H

