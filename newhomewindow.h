#ifndef NEWHOMEWINDOW_H
#define NEWHOMEWINDOW_H

#include <QMainWindow>

namespace Ui {
class newHomeWindow;
}

class newHomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit newHomeWindow(QWidget *parent = nullptr);
    void onCommandButton3Clicked(); //点击按钮后跳转到编辑个人信息界面
    void onCommandButton4Clicked(); //点击按钮后跳转到航班动态界面
    void onCommandButton5Clicked(); //点击按钮后跳转到订单界面
    void onCommandButton6Clicked(); //点击按钮后跳转到homewindow界面
    ~newHomeWindow();

private:
    Ui::newHomeWindow *ui;
};

#endif // NEWHOMEWINDOW_H
