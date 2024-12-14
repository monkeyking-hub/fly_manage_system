#ifndef adminMainInterface_H
#define adminMainInterface_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QTreeWidgetItem>
#include <QLabel>
#include <QStackedWidget>

namespace Ui {
class adminMainInterface;
}

class adminMainInterface : public QMainWindow
{
    Q_OBJECT
public:
    QTreeWidget *menuTree;
    //一级菜单
    QTreeWidgetItem *homeItem; //首页
    QTreeWidgetItem *kefuItem; //客服中心
    QTreeWidgetItem *usersItem; //用户管理
    QTreeWidgetItem *ordersItem; //订单管理
    QTreeWidgetItem *flightsItem; //航班管理
    //二级菜单
    QTreeWidgetItem *userSearchItem; //查找用户信息
    QTreeWidgetItem *userRemoveItem; //删除用户信息
    QTreeWidgetItem *userUpdateItem; //修改用户信息
    QTreeWidgetItem *orderSearchItem; //查找订单信息
    QTreeWidgetItem *orderRemoveItem; //删除订单信息
    QTreeWidgetItem *orderUpdateItem; //修改订单信息
    QTreeWidgetItem *flightSearchItem; //查找航班信息
    QTreeWidgetItem *flightRemoveItem; //删除航班信息
    QTreeWidgetItem *flightUpdateItem; //修改航班信息
    QTreeWidgetItem *flightAddItem; //添加航班信息
    QStackedWidget *stackedWidget;

public:
    explicit adminMainInterface(QWidget *parent = nullptr);
    ~adminMainInterface();

public slots:
    void onItemClicked(QTreeWidgetItem *item, int column);
    void onReturnButtonClicked();

private:
    Ui::adminMainInterface *ui;
};

#endif // adminMainInterface_H
