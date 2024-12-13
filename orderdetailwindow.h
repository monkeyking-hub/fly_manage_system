#ifndef ORDERDETAILWINDOW_H
#define ORDERDETAILWINDOW_H

#include <QWidget>
#include "order.h"

class OrderDetailWindow : public QWidget
{
    Q_OBJECT

public:
    explicit OrderDetailWindow(const Order &order, QWidget *parent = nullptr);

private:
    void setupUI(const Order &order); // 初始化界面
};

#endif // ORDERDETAILWINDOW_H
