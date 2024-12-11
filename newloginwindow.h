#ifndef NEWLOGINWINDOW_H
#define NEWLOGINWINDOW_H

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
class newLoginWindow;
}

class newLoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    AnimatedInputField *emailField;
    AnimatedInputField *passwordField;

public:
    explicit newLoginWindow(QWidget *parent = nullptr);
    ~newLoginWindow();

signals:
    void loginRequested(const QString &usrname,const QString &password);

public slots:
    void onSignUpButtonClicked(); //注册按钮点击的槽函数
    void onLogInButtonClicked(); //登录按钮点击的槽函数

private:
    Ui::newLoginWindow *ui;
};

class loginHandler:public QObject
{
    Q_OBJECT;

public slots:
    void handleLogin(const QString& email, const QString& password);
};

#endif // NEWLOGINWINDOW_H
