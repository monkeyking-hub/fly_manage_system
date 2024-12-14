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
#include <QKeyEvent>  // 引入 QKeyEvent

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
    if (isClient) {
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

// 重载 keyPressEvent 处理回车键事件
void ChatWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        // 调用发送消息的函数
        onSendMessage();
    }
    // 确保事件继续传递给父类
    event->ignore();
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

                    // 这里判断消息是由谁发送的来调整对齐方式
                    QString alignStyle="left";
                    QString userPrefix;
                    QString space=  QString(100,' ');
                    if(isClient)//如果是客户
                    {
                        if (userId == 1)//客户的人工消息
                        {
                            userPrefix =  "-人工客服:" ;
                        }
                        else
                        {
                            userPrefix =  "-客户2:" ;
                        }
                    }
                    else//如果是客服
                    {
                        if (userId == 1) {
                            userPrefix = "-人工客服:" ;
                        }//客服自己的
                        else
                        {
                            userPrefix = "-客户2:" ;
                        }
                    }
                    // 拼接显示的消息内容，使用 HTML 格式化
                    QString msgContent = "<div style='border: 1px solid #ddd; padding: 10px; border-radius: 10px; margin-bottom: 10px; background-color: rgba(255, 255, 255, 0.8);'>";
                    msgContent += "<b style='font-size: 12px; color: #888;'>[" + time + "]</b><br>";
                    msgContent += "<span style='font-size: 14px;'>" + userPrefix + " " + messageText + "</span>";
                    msgContent += "</div>";

                    // 使用 inline-style 的 align 属性来调整每条消息的对齐方式
                    msgContent = "<div style='text-align: " + alignStyle + ";'>" + msgContent + "</div>";

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
    // 先将消息添加到聊天区域（假设消息最终会成功发送）
    qint64 timestamp = QDateTime::currentSecsSinceEpoch();
    QString time = QDateTime::fromSecsSinceEpoch(timestamp).toString("hh:mm:ss");

    QString alignStyle="left";
    QString userPrefix;
    if(isClient)//如果是客户
    {
        if (userId == 1)//客户的人工消息
        {
            userPrefix =  "-人工客服:" ;
        }
        else
        {
            userPrefix =  "-客户2:" ;
        }
    }
    else//如果是客服
    {
        if (userId == 1) {
            userPrefix = "-人工客服:" ;
        }//客服自己的
        else
        {
            userPrefix = "-客户2:" ;
        }
    }

    QString msgContent = "<div style='border: 1px solid #ddd; padding: 10px; border-radius: 10px; margin-bottom: 10px; background-color: rgba(255, 255, 255, 0.8);'>";
    msgContent += "<b style='font-size: 12px; color: #888;'>[" + time + "]</b><br>";
    msgContent += "<span style='font-size: 14px;'>" + userPrefix + " " + message + "</span>";
    msgContent += "</div>";

    // 使用 inline-style 的 align 属性来调整每条消息的对齐方式
    msgContent = "<div style='text-align: " + alignStyle + ";'>" + msgContent + "</div>";

    // 先显示消息，不管服务器是否成功接收
    chatArea->append(msgContent);

    // 发送消息到服务器
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://127.0.0.1:8080/api/chat/send");  // 后端接口URL
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["userId"] = userId;
    json["message"] = message;
    json["timestamp"] = timestamp;

    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());

    connect(reply, &QNetworkReply::finished, [this, reply, msgContent]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject responseObject = jsonResponse.object();

            if (responseObject["code"].toInt() == 200) {
                // 服务器成功返回消息
                qDebug() << "Message sent successfully";
            } else {
                // 服务器返回失败
                qDebug() << "Failed to send message:" << responseObject["message"].toString();
                QMessageBox::critical(nullptr, "发送消息失败", responseObject["message"].toString());

                // 可选：你可以修改显示的消息状态，表示发送失败
                // 比如：修改消息内容为 "发送失败"
                chatArea->append("<div style='color: red;'>" + msgContent + " <b>发送失败</b></div>");
            }
        } else {
            // 网络错误或其他请求错误
            qDebug() << "Error sending message:" << reply->errorString();
            QMessageBox::critical(nullptr, "发送消息失败", "请求失败: " + reply->errorString());

            // 可选：修改显示的消息状态
            chatArea->append("<div style='color: red;'>" + msgContent + " <b>发送失败</b></div>");
        }
        reply->deleteLater();
    });
}



void ChatWindow::onSendMessage()
{
    QString message = messageInput->text();
    if (message.isEmpty()) {
        QMessageBox::warning(this, "输入为空", "请输入要发送的消息");
        return;
    }

    // 清空输入框
    messageInput->clear();

    // 发送消息
    sendMessage(isClient ? 2 : 1, message);
}
