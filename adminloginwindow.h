#ifndef ADMINLOGINWINDOW_H
#define ADMINLOGINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QPropertyAnimation>
#include <QMessageBox>
#include <QSpacerItem>
#include <QApplication>
#include "animatedInputField.h"

namespace Ui {
class adminLoginWindow;
}

class adminLoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    AnimatedInputField *usrnameField;
    AnimatedInputField *passwordField;

public:
    explicit adminLoginWindow(QWidget *parent = nullptr);
    ~adminLoginWindow();

signals:
    void adminLoginRequested(const QString &usrname,const QString &password);

public slots:
    void onAdminLogInButtonClicked(); //登录按钮点击的槽函数
    void onAdminReturnButtonClicked(); //返回按钮点击的槽函数
private:
    Ui::adminLoginWindow *ui;
};

class adminLoginHandler:public QObject
{
    Q_OBJECT;

public slots:
    void handleAdminLogin(const QString& email, const QString& password);
};


#endif // ADMINLOGINWINDOW_H
