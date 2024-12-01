#ifndef ORDERWINDOW_H
#define ORDERWINDOW_H

#include <QMainWindow>

namespace Ui {
class order_2;
}

class orderwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit orderwindow(QWidget *parent = nullptr);
    ~orderwindow();

private:
    Ui::order_2 *ui;
     void openInOrder(int tabIndex);  // 打开 inorder 界面并设置初始页签
};

#endif // ORDERWINDOW_H
