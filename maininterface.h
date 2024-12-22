#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <QListWidget>
#include <QMainWindow>
#include <QStackedWidget>
#include "orderdetailwindow.h"
#include "user.h"

namespace Ui {
class maininterface;
}

class maininterface : public QMainWindow
{
    Q_OBJECT
public:
    QStackedWidget *stackedWidget;
    QListWidget *menuList;

public:
    explicit maininterface(QWidget *parent = nullptr);
    void editUserInfo();               //修改个人资料
    void switchLogin();                //切换登录
    void switchtoFlightStatusWindow(); //转到航班动态界面
    void onKefuButtonClicked();
    void updateAvatar(const User &user);
    ~maininterface();

private:
    Ui::maininterface *ui;
};

#endif // MAININTERFACE_H
