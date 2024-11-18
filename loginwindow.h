#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

namespace Ui {
class loginWindow;
}

class loginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit loginWindow(QWidget *parent = nullptr);
    void onLoginButtonClicked(); //点击登录按钮触发事件
    void onSignUpButtonClicked(); //点击注册按钮触发事件
    ~loginWindow();

private:
    Ui::loginWindow *ui;
};

#endif // LOGINWINDOW_H
