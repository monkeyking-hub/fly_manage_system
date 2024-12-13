#include "chatwindow.h"
#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QDebug>
#include <QPushButton>
#include <QLineEdit>
#include <QTimer>

ChatWindow::ChatWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // 设置窗口标题和尺寸
    setWindowTitle("ChatWindow");
    setFixedSize(500, 600); // 增加窗口宽度

    // 创建一个 QWidget 作为主窗口的中心控件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建一个垂直布局，用于存放聊天记录
    chatLayout = new QVBoxLayout();
    chatLayout->setSpacing(10);  // 设置垂直布局中的间距

    // 创建一个滚动区域，用于展示聊天记录
    chatScrollArea = new QScrollArea(this);
    chatScrollArea->setWidgetResizable(true);

    // 创建一个 widget，用于在滚动区域中显示聊天记录
    QWidget *chatContentWidget = new QWidget(chatScrollArea);
    chatScrollArea->setWidget(chatContentWidget);

    // 将垂直布局设置为聊天内容的布局
    chatContentWidget->setLayout(chatLayout);

    // 创建输入框和发送按钮
    messageInput = new QLineEdit(this);
    messageInput->setPlaceholderText("请输入消息...");
    messageInput->setFixedHeight(40);

    sendButton = new QPushButton("发送", this);
    sendButton->setFixedHeight(40);

    // 创建一个布局，包含文本框和按钮
    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(messageInput);
    inputLayout->addWidget(sendButton);

    // 创建主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(chatScrollArea);
    mainLayout->addLayout(inputLayout);

    // 设置主布局间距
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0); // 移除外部边距

    // 设置布局
    centralWidget->setLayout(mainLayout);

    // 加载历史聊天记录
    fetchChatHistory();

    // 定时器每秒钟检查新消息
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ChatWindow::fetchChatHistory);
    timer->start(3000); // 每1000毫秒（1秒）调用一次

    // 发送消息按钮点击事件
    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::onSendMessage);
}

ChatWindow::~ChatWindow()
{
    // 析构时无需手动删除 ui，因为是通过指针创建的
}

void ChatWindow::fetchChatHistory()
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QUrl url("http://127.0.0.1:8080/api/chat/history");  // 后端接口URL
    QNetworkRequest request(url);

    QNetworkReply* reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject responseObject = jsonResponse.object();

            if (responseObject["code"].toInt() == 200) {
                qDebug() << "Chat history retrieved successfully";
                QJsonArray chatHistory = responseObject["data"].toArray();

                // 清空聊天区域
                qDeleteAll(chatLayout->children());

                // 处理返回的聊天记录数据
                for (const QJsonValue& value : chatHistory) {
                    QJsonObject message = value.toObject();
                    int userId = message["userId"].toInt();
                    QString messageText = message["message"].toString();
                    qint64 timestamp = message["timestamp"].toInt();
                    QString time = QDateTime::fromSecsSinceEpoch(timestamp).toString("hh:mm:ss");

                    // 创建消息标签
                    QLabel* msgLabel = new QLabel;
                    msgLabel->setText(time + " - " + messageText);
                    msgLabel->setWordWrap(true);  // 启用自动换行
                    msgLabel->setAlignment(Qt::AlignTop);

                    // 设置消息对齐方式和样式
                    if (userId == 1) { // 假设1是当前用户的ID
                        msgLabel->setAlignment(Qt::AlignRight);
                        msgLabel->setStyleSheet("background-color: #d3f8e2; padding: 15px; border-radius: 10px; max-width: 95%; width: 100%; margin-bottom: 15px;");
                    } else {
                        msgLabel->setAlignment(Qt::AlignLeft);
                        msgLabel->setStyleSheet("background-color: #f0f0f0; padding: 15px; border-radius: 10px; max-width: 95%; width: 100%; margin-bottom: 15px;");
                    }

                    // 将消息标签添加到垂直布局中
                    chatLayout->addWidget(msgLabel);
                }

                // 滚动到底部
                QScrollBar *scrollBar = chatScrollArea->verticalScrollBar();
                scrollBar->setValue(scrollBar->maximum());
            } else {
                qDebug() << "Failed to retrieve chat history:" << responseObject["message"].toString();
                QMessageBox::critical(nullptr, "获取历史记录失败", responseObject["message"].toString());
            }
        } else {
            qDebug() << "Error fetching chat history:" << reply->errorString();
            QMessageBox::critical(nullptr, "获取历史记录失败", "请求失败: " + reply->errorString());
        }
        reply->deleteLater();
    });
}

void ChatWindow::sendMessage(int userId, const QString& message)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QUrl url("http://127.0.0.1:8080/api/chat/send");  // 后端接口URL
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["userId"] = userId;
    json["message"] = message;
    json["timestamp"] = QDateTime::currentSecsSinceEpoch();

    QNetworkReply* reply = manager->post(request, QJsonDocument(json).toJson());

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject responseObject = jsonResponse.object();

            if (responseObject["code"].toInt() == 200) {
                qDebug() << "Message sent successfully";

                // 处理返回的数据（例如更新聊天窗口）
                QJsonObject message = responseObject["data"].toObject();
                int userId = message["userId"].toInt();
                QString messageText = message["message"].toString();
                qint64 timestamp = message["timestamp"].toInt();
                QString time = QDateTime::fromSecsSinceEpoch(timestamp).toString("hh:mm:ss");

                // 创建消息标签并显示在聊天窗口
                QLabel* msgLabel = new QLabel;
                msgLabel->setText(time + " - " + messageText);

                // 设置消息对齐方式
                if (userId == 1) { // 假设1是当前用户的ID
                    msgLabel->setAlignment(Qt::AlignRight);
                    msgLabel->setStyleSheet("background-color: #d3f8e2; padding: 5px; border-radius: 10px; max-width: 70%;");
                } else {
                    msgLabel->setAlignment(Qt::AlignLeft);
                    msgLabel->setStyleSheet("background-color: #f0f0f0; padding: 5px; border-radius: 10px; max-width: 70%;");
                }

                // 将消息标签添加到垂直布局中
                chatLayout->addWidget(msgLabel);

                // 滚动到底部
                QScrollBar *scrollBar = chatScrollArea->verticalScrollBar();
                scrollBar->setValue(scrollBar->maximum());
            } else {
                qDebug() << "Message sending failed:" << responseObject["message"].toString();
                QMessageBox::critical(nullptr, "发送失败", responseObject["message"].toString());
            }
        } else {
            qDebug() << "Error sending message:" << reply->errorString();
            QMessageBox::critical(nullptr, "发送失败", "请求失败: " + reply->errorString());
        }
        reply->deleteLater();
    });
}

// 按钮点击事件
void ChatWindow::onSendMessage()
{
    QString message = messageInput->text();
    if (message.isEmpty()) {
        QMessageBox::warning(this, "发送失败", "消息不能为空！");
        return;
    }

    int userId = 1;  // 假设当前用户的ID为1
    sendMessage(userId, message);

    // 清空文本框
    messageInput->clear();
}
