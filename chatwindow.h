#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QLineEdit>
#include <QTextBrowser>
#include <QLabel>

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(bool isclient,QWidget *parent = nullptr);
    ~ChatWindow();
    bool isClient = false;

private:
    QWidget *centralWidget;            // 中央控件
    QVBoxLayout *mainLayout;           // 主布局
    void keyPressEvent(QKeyEvent *event);
    QLabel *friendNameLabel;           // 显示对方名字
    QTextBrowser *chatArea;            // 聊天记录显示区域
    QLineEdit *messageInput;           // 消息输入框
    QPushButton *sendButton;           // 发送按钮

    void fetchChatHistory();           // 加载历史记录
    void sendMessage(int userId, const QString& message);  // 发送消息

private slots:
    void onSendMessage();  // 发送消息槽函数
};

#endif // CHATWINDOW_H
