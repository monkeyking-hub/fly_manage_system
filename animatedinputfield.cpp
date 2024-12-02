#include "animatedInputField.h"

AnimatedInputField::AnimatedInputField(const QString &placeholderText, bool isPassword, QWidget *parent)
    : QWidget(parent), label(new QLabel(this)), input(new QLineEdit(this)), animation(new QPropertyAnimation(label, "pos", this)) {

    QVBoxLayout *mainLayout = new QVBoxLayout(this);  // 创建主布局
    mainLayout->setContentsMargins(0, 0, 0, 0);  // 去除边距
    mainLayout->setSpacing(0);  // 去除间距

    // 设置标签（初始位置）
    label->setText(placeholderText);  // 设置标签文本
    label->setStyleSheet("background:transparent; color: black; font-size: 16px;");  // 设置标签样式
    label->move(5, 25);  // 设置标签的初始位置
    label->resize(200, 20);  // 设置标签的大小

    // 设置输入框
    input->setStyleSheet(
        "background: transparent; border: none; border-bottom: 2px solid white; "
        "font-size: 16px; color: white; padding: 5px;"
        );
    if (isPassword) {
        input->setEchoMode(QLineEdit::Password);  // 如果是密码框，设置为密码输入模式
    }
    mainLayout->addWidget(input);  // 将输入框添加到布局中

    // 设置动画效果
    animation->setDuration(300);  // 设置动画持续时间
    animation->setEasingCurve(QEasingCurve::InOutQuad);  // 设置动画的缓动曲线

    // 安装事件过滤器以捕获焦点事件
    input->installEventFilter(this);
}

QString AnimatedInputField::text() const { return input->text(); }  // 获取输入框的文本

bool AnimatedInputField::eventFilter(QObject *obj, QEvent *event){
    if (obj == input) {
        if (event->type() == QEvent::FocusIn) {
            onFocusIn(); // 如果输入框获得焦点，执行动画
        } else if (event->type() == QEvent::FocusOut) {
            onFocusOut(); // 如果输入框失去焦点，执行动画
        }
    }
    return QWidget::eventFilter(obj, event); // 调用基类的事件过滤器
}
