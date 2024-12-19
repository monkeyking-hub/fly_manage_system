#ifndef ORDERWINDOW_H
#define ORDERWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTabBar> // 引入 QTabBar 类
#include "orderdetailwindow.h"
#include <QVBoxLayout>


QT_BEGIN_NAMESPACE
namespace Ui { class order_2; }
QT_END_NAMESPACE

class orderwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit orderwindow(QWidget *parent = nullptr);
    ~orderwindow();
    void setupTabButtons(); // 声明 setupTabButtons 函数

private:
    Ui::order_2 *ui;

    QWidget* createOrderPage(const QString &type); // 创建订单页面
    void onTabChanged(int index);                 // 处理 Tab 切换
    void showOrderDetails(const Order &order);
    void fetchOrderIds(int userId, QVBoxLayout *containerLayout, const QString &type, QWidget *container);  // 获取订单 ID
    void fetchOrderDetails(const QList<int> &orderIds, QVBoxLayout *containerLayout, const QString &type, QWidget *container);  // 获取订单详情

    Order parseOrderDetail(const QJsonObject &orderDetail);  // 解析订单详细信息
};

#endif // ORDERWINDOW_H
