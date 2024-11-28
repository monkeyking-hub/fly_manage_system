#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QString>

namespace Ui {
class loginWindow;
}

class loginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit loginWindow(QWidget *parent = nullptr);
    void onSignUpButtonClicked(); //点击注册按钮触发事件
    ~loginWindow();

signals:
    void loginRequested(const QString &usrname,const QString &password);

public slots:
    void onLoginButtonClicked();  //点击登录按钮触发事件

private:

    Ui::loginWindow *ui;
};

class loginHandler:public QObject
{
    Q_OBJECT;

public slots:
    void handleLogin(const QString& username, const QString& password);
};

#endif // LOGINWINDOW_H
