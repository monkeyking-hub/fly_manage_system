#ifndef ANIMATEDINPUTFIELD_H
#define ANIMATEDINPUTFIELD_H
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QWidget>

// 创建一个带有动画效果的输入框类
class AnimatedInputField : public QWidget
{
    Q_OBJECT

public:
    // 构造函数，接受占位符文本和是否是密码框的标志
    AnimatedInputField(const QString &placeholderText,
                       bool isPassword = false,
                       QWidget *parent = nullptr);

    QString text() const;  // 获取输入框的文本
    QLineEdit *lineEdit(); //获取输入框的指针
protected:
    // 事件过滤器，用于处理焦点事件
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    // 焦点获得时执行的动画效果
    void onFocusIn()
    {
        animation->stop();                      // 停止当前动画
        animation->setStartValue(label->pos()); // 设置动画的起始位置
        animation->setEndValue(QPoint(5, 0));   // 设置动画的结束位置（向上移动）
        animation->start();                     // 启动动画
        label->setStyleSheet("color: black; font-size: 13px;"); // 改变标签样式
    }

    // 焦点失去时执行的动画效果
    void onFocusOut()
    {
        if (input->text().isEmpty()) {              // 如果输入框为空
            animation->stop();                      // 停止当前动画
            animation->setStartValue(label->pos()); // 设置动画的起始位置
            animation->setEndValue(QPoint(5, 25));  // 设置动画的结束位置（向下移动）
            animation->start();                     // 启动动画
            label->setStyleSheet("color: black; font-size: 16px;"); // 改变标签样式
        }
    }

private:
    QLabel *label;                 // 标签
    QLineEdit *input;              // 输入框
    QPropertyAnimation *animation; // 动画
};

#endif // ANIMATEDINPUTFIELD_H
