#ifndef INORDER_H
#define INORDER_H

#include <QMainWindow>
#include <QTabBar>
#include <QStackedWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class inorder; }
QT_END_NAMESPACE

class inorder : public QMainWindow
{
    Q_OBJECT

public:
    explicit inorder(QWidget *parent = nullptr);
    ~inorder();
    void setInitialTab(int index); // 设置初始页签

private:
    Ui::inorder *ui;

    // 新增组件成员变量
    QTabBar *tabBar;
    QStackedWidget *stackedWidget;


    // 辅助函数
    QWidget *createPage(const QString &title);

};

#endif // INORDER_H
