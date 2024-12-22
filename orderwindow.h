#ifndef ORDERWINDOW_H
#define ORDERWINDOW_H

#include <QComboBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QMainWindow>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTabBar> // 引入 QTabBar 类
#include <QVBoxLayout>
#include <QWidget>
#include "order.h"
#include "orderdetailwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class order_2;
}
QT_END_NAMESPACE

class orderwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit orderwindow(QWidget *parent = nullptr);
    ~orderwindow();

    // 创建订单页面
    QWidget *createOrderPage(const QString &type);

    // Tab 切换时处理
    void onTabChanged(int index);

    // 显示订单详情
    void showOrderDetails(const Order &order);

    void fetchOrders(int userId);
    //创建每个订单小窗口
    void createOrderWidget(const Order &order, QVBoxLayout *containerLayout);

    void updateOrderPage(const QString &type,
                         QVBoxLayout *pageLayout,
                         QComboBox *departureComboBox,
                         QComboBox *destinationComboBox,
                         QComboBox *airlineComboBox);

private:
    Ui::order_2 *ui;
};

#endif // ORDERWINDOW_H
