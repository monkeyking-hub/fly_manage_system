# 聊天窗口模拟数据修改说明

## 修改文件
- `chatwindow.cpp` - 第 148-294 行的 `fetchChatHistory()` 函数

## 问题所在
**错误位置**: `/home/engine/project/chatwindow.cpp` 第 224 行
```cpp
qDebug() << "Error fetching chat history:" << reply->errorString();
QMessageBox::critical(nullptr, "获取历史记录失败", "请求失败: " + reply->errorString());
```

**问题原因**: 
- 当后端服务器未启动时，连接到 `http://127.0.0.1:8080/api/chat/history` 会被拒绝（Connection refused）
- 定时器每 3 秒调用一次 `fetchChatHistory()`（第 96 行）
- 因此用户会不断看到弹出的错误对话框

## 修改内容

### 修改前
当连接被拒绝时，直接显示 `QMessageBox::critical()` 弹出框，导致不断弹窗。

### 修改后
在第 223 行添加了连接被拒绝的特殊处理：
```cpp
if (reply->error() == QNetworkReply::ConnectionRefusedError) {
    qDebug() << "Backend not available, using mock data for testing";
    
    // 清空聊天区域
    chatArea->clear();
    
    // 创建模拟数据
    QJsonArray mockChatHistory;
    
    // 模拟消息 1 (客服)
    QJsonObject msg1;
    msg1["userId"] = 1;
    msg1["message"] = "你好！欢迎来到我们的航空订票系统。";
    msg1["timestamp"] = QDateTime::currentSecsSinceEpoch() - 300;
    mockChatHistory.append(msg1);
    
    // 模拟消息 2 (客户)
    QJsonObject msg2;
    msg2["userId"] = 2;
    msg2["message"] = "谢谢！我想查询北京到上海的航班。";
    msg2["timestamp"] = QDateTime::currentSecsSinceEpoch() - 200;
    mockChatHistory.append(msg2);
    
    // 模拟消息 3 (客服)
    QJsonObject msg3;
    msg3["userId"] = 1;
    msg3["message"] = "好的，我为您查询一下。请告诉我您的出行日期。";
    msg3["timestamp"] = QDateTime::currentSecsSinceEpoch() - 100;
    mockChatHistory.append(msg3);
    
    // 模拟消息 4 (客户)
    QJsonObject msg4;
    msg4["userId"] = 2;
    msg4["message"] = "我想要明天的航班。";
    msg4["timestamp"] = QDateTime::currentSecsSinceEpoch();
    mockChatHistory.append(msg4);
    
    // 处理模拟数据并显示
    for (const QJsonValue &value : mockChatHistory) {
        // ... 使用与真实数据相同的渲染逻辑
    }
}
```

## 主要改动点

1. **第 216-218 行**: 移除了错误处理中的 `QMessageBox::critical()` 调用，改为仅打印日志

2. **第 223-290 行**: 添加了对 `ConnectionRefusedError` 的特殊处理
   - 检测到连接被拒绝时，不再弹出错误对话框
   - 而是加载 4 条模拟聊天记录
   - 模拟数据包含客服（userId=1）和客户（userId=2）的对话
   - 使用与真实数据相同的 HTML 渲染逻辑显示消息

## 模拟数据内容
- **消息 1**: 客服欢迎语 (5 分钟前)
- **消息 2**: 客户查询航班请求 (3 分钟前)
- **消息 3**: 客服回应 (1 分钟前)
- **消息 4**: 客户选择日期 (刚刚)

## 测试效果
现在当后端未启动时：
- ✅ 不会弹出错误对话框
- ✅ 聊天窗口会显示 4 条模拟对话
- ✅ 用户可以继续输入消息并测试发送功能
- ✅ 定时刷新不会产生任何干扰

## 恢复真实后端
当后端服务启动后，系统会自动使用真实的 API 数据替代模拟数据。无需修改代码。
