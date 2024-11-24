#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <QMainWindow>

namespace Ui {
class maininterface;
}

class maininterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit maininterface(QWidget *parent = nullptr);
    void editUserInfo(); //修改个人资料
    void switchLogin();  //切换登录
    ~maininterface();

private:
    Ui::maininterface *ui;
};

#endif // MAININTERFACE_H
