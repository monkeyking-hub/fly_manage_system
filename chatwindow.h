#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QScrollBar>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTimer>

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

private:
    QVBoxLayout *chatLayout;           // 用于显示聊天消息的垂直布局
    QWidget *chatWidget;               // 包含消息的容器
    QScrollArea *chatScrollArea;       // 显示聊天记录的滚动区域

    QLineEdit *messageInput;           // 用于输入消息的文本框
    QPushButton *sendButton;           // 发送按钮

    // 加载历史记录
    void fetchChatHistory();

    // 发送消息
    void sendMessage(int userId, const QString& message);

private slots:
    // 发送消息槽函数
    void onSendMessage();
};

#endif // CHATWINDOW_H
