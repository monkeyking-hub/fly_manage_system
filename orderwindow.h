#ifndef ORDERWINDOW_H
#define ORDERWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTabBar> // 引入 QTabBar 类
#include <QVBoxLayout>
#include "orderdetailwindow.h"
#include "order.h"
#include <QJsonObject>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QMessageBox>
#include <QComboBox>

QT_BEGIN_NAMESPACE
namespace Ui { class order_2; }
QT_END_NAMESPACE

class orderwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit orderwindow(QWidget *parent = nullptr);
    ~orderwindow();

    // 创建订单页面
    QWidget* createOrderPage(const QString &type);

    // Tab 切换时处理
    void onTabChanged(int index);

    // 显示订单详情
    void showOrderDetails(const Order &order);

    // 获取订单 ID
    void fetchOrderIds(int userId);

    // 获取订单详情
    void fetchOrderDetails(const QList<QString> &flightNumbers);
    //创建每个订单小窗口
    void createOrderWidget(const Order &order, QVBoxLayout *containerLayout);

    void updateOrderPage(const QString &type, QVBoxLayout *pageLayout, QComboBox *departureComboBox, QComboBox *destinationComboBox, QComboBox *airlineComboBox);



private:
    Ui::order_2 *ui;
};

#endif // ORDERWINDOW_H

