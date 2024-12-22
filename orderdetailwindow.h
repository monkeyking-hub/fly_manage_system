#ifndef ORDERDETAILWINDOW_H
#define ORDERDETAILWINDOW_H

#include <QWidget>
#include "order.h"

class OrderDetailWindow : public QWidget
{
    Q_OBJECT

public:
    explicit OrderDetailWindow(const Order &order, QWidget *parent = nullptr);

signals:
    void complaintButtonClicked();

private:
    void setupUI(const Order &order); // 初始化界面
    void showBaggageTable();
};

#endif // ORDERDETAILWINDOW_H
