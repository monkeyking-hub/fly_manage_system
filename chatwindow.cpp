#include "chatwindow.h"
#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QDebug>
#include <QTimer>  // 引入 QTimer

ChatWindow::ChatWindow(bool isclient, QWidget *parent) :
    isClient(isclient),
    QMainWindow(parent)
{
    // 设置窗口标题和尺寸
    setWindowTitle("ChatWindow");
    setWindowState(Qt::WindowMaximized);  // 窗口最大化

    // 设置主窗口的背景图片
    setStyleSheet("QMainWindow { background-image: url(:/blue.png); background-position: center; }");

    // 创建一个 QWidget 作为主窗口的中心控件
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 主布局
    mainLayout = new QVBoxLayout(centralWidget);

    // 显示对方名字
    if (isclient) {
        friendNameLabel = new QLabel("人工客服", this);
    } else {
        friendNameLabel = new QLabel("刁难客户", this);
    }
    friendNameLabel->setAlignment(Qt::AlignCenter);
    friendNameLabel->setStyleSheet("font-size: 22px; font-weight: bold; padding: 15px; color: #ffffff;");

    // 创建聊天记录区域（使用 QTextBrowser 来显示消息）
    chatArea = new QTextBrowser(this);
    chatArea->setReadOnly(true);
    chatArea->setStyleSheet("background-color: rgba(255, 255, 255, 0.7); border-radius: 10px; padding: 15px; font-size: 14px; color: #333; border: 1px solid #ddd;");

    // 输入框和发送按钮
    messageInput = new QLineEdit(this);
    messageInput->setPlaceholderText("请输入消息...");
    messageInput->setFixedHeight(40);
    messageInput->setStyleSheet("padding: 5px; font-size: 14px; border: 1px solid #ddd; border-radius: 5px;");

    sendButton = new QPushButton("发送", this);
    sendButton->setFixedHeight(40);
    sendButton->setStyleSheet("background-color: #007bff; color: white; border-radius: 5px; font-size: 14px; transition: background-color 0.3s;");

    // 添加按钮悬浮效果
    sendButton->setStyleSheet(
        "QPushButton { background-color: #007bff; color: white; border-radius: 5px; font-size: 14px; }"
        "QPushButton:hover { background-color: #0056b3; }"
        );

    // 创建输入区域布局
    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(messageInput);
    inputLayout->addWidget(sendButton);

    // 添加控件到主布局
    mainLayout->addWidget(friendNameLabel);
    mainLayout->addWidget(chatArea);
    mainLayout->addLayout(inputLayout);

    // 设置主布局间距和边距
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);  // 给外围添加一些空隙

    // 加载历史聊天记录
    fetchChatHistory();

    // 设置定时器每3秒刷新一次
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ChatWindow::fetchChatHistory);
    timer->start(3000);  // 每3秒刷新一次

    // 发送消息按钮点击事件
    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::onSendMessage);
}

ChatWindow::~ChatWindow()
{
    // 析构时无需手动删除 ui，因为是通过指针创建的
}

void ChatWindow::fetchChatHistory()
{
    // 模拟获取历史消息
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://127.0.0.1:8080/api/chat/history");  // 后端接口URL
    QNetworkRequest request(url);

    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject responseObject = jsonResponse.object();

            if (responseObject["code"].toInt() == 200) {
                qDebug() << "Chat history retrieved successfully";
                QJsonArray chatHistory = responseObject["data"].toArray();

                // 清空聊天区域
                chatArea->clear();

                // 处理返回的聊天记录数据
                for (const QJsonValue& value : chatHistory) {
                    QJsonObject message = value.toObject();
                    int userId = message["userId"].toInt();
                    QString messageText = message["message"].toString();
                    qint64 timestamp = message["timestamp"].toInt();
                    QString time = QDateTime::fromSecsSinceEpoch(timestamp).toString("hh:mm:ss");

                    // 拼接显示的消息内容
                    QString msgContent = "<b>" + time + "</b>: " + messageText + "\n";

                    // 根据用户ID调整对齐方式
                    if (isClient == false) {  // 表示当前为客服
                        if (userId == 1)
                            chatArea->setAlignment(Qt::AlignRight);
                        else
                            chatArea->setAlignment(Qt::AlignLeft);
                    } else {  // 其他用户
                        if (userId == 1)
                            chatArea->setAlignment(Qt::AlignLeft);
                        else
                            chatArea->setAlignment(Qt::AlignRight);
                    }

                    // 将消息内容添加到聊天区域
                    chatArea->append(msgContent);
                }
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
    // 发送消息
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://127.0.0.1:8080/api/chat/send");  // 后端接口URL
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["userId"] = userId;
    json["message"] = message;
    json["timestamp"] = QDateTime::currentSecsSinceEpoch();

    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());

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

                // 拼接显示的消息内容
                QString msgContent = "<b>" + time + "</b>: " + messageText + "\n";
                // 根据用户ID调整对齐方式
                if (isClient==false)
                {  //表示当前为客服
                    // 根据用户ID调整对齐方式
                    if (userId == 1) {  // 假设1是当前用户的ID
                        msgContent.prepend("<div style='text-align:right;'>");
                        msgContent.append("</div>");
                    } else {
                        msgContent.prepend("<div style='text-align:left;'>");
                        msgContent.append("</div>");
                    }
                } else {  // 其他用户
                    // 根据用户ID调整对齐方式
                    if (userId == 1) {  // 假设1是当前用户的ID
                        msgContent.prepend("<div style='text-align:left;'>");
                        msgContent.append("</div>");
                    } else {
                        msgContent.prepend("<div style='text-align:right;'>");
                        msgContent.append("</div>");
                    }
                }

                // // 根据用户ID调整对齐方式
                // if (userId == 1) {  // 假设1是当前用户的ID
                //     msgContent.prepend("<div style='text-align:right;'>");
                //     msgContent.append("</div>");
                // } else {
                //     msgContent.prepend("<div style='text-align:left;'>");
                //     msgContent.append("</div>");
                // }

                // 将消息内容添加到聊天区域
                chatArea->append(msgContent);
            } else {
                qDebug() << "Message sending failed:" << responseObject["message"].toString();
                QMessageBox::critical(nullptr, "发送失败", responseObject["message"].toString());
            }
        } else {
            qDebug() << "Error sending message:" << reply->errorString();
            QMessageBox::critical(nullptr, "发送失败", "请求失败: " + reply->errorString());
        }
    });
}


void ChatWindow::onSendMessage()
{
    QString message = messageInput->text();
    if (message.isEmpty()) {
        QMessageBox::warning(this, "发送失败", "消息不能为空！");
        return;
    }
    int userId = isClient ? 2 : 1;
    sendMessage(userId, message);  // 假设1是当前用户ID
    messageInput->clear();
}
