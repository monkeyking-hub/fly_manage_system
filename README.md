# 重新修改了一下调度类

## 写了一个system类
新写好一个前端就在system的start函数里面注册进去
然后在构造函数里面取消注册
```cpp
void System::start()
{
    // 创建子界面,并且注册进调度器
    BetaWindow * beta       = new BetaWindow();
    GamaWindow * gamawindow = new GamaWindow();
    XitaWindow * xitawindow = new XitaWindow();
    Yipuxilong * yipuxilong = new Yipuxilong();

    InterfaceManager::instance()->registerPage("/MainWindow/Beta", beta);
    //注册gama界面
    InterfaceManager::instance()->registerPage("/MainWindow/Beta/Gama",gamawindow);
    //注册西塔
    InterfaceManager::instance()->registerPage("/MainWindow/Beta/Xita",xitawindow);
    //注册伊普西隆
    InterfaceManager::instance()->registerPage("/MainWindow/Beta/Yipuxilong",yipuxilong);

}
```
可以修改main函数的来使得第一张展示的的界面,可以用来查看自己写的前端界面
```cpp

InterfaceManager::instance()->switchToPage("/MainWindow/Beta");

```


后续也可以通过这个函数来跳转,只要输入当初注册的名字就行

# 新增加了http接口文档,在http.md中
### 2. 用户登录接口

#### 请求
- **方法**: `POST`
- **URL**: `/api/users/login`
- **请求头**:  
  - `Content-Type: application/json`
- **请求体**:
  ```json
  {
    "username": "exampleUser",
    "password": "examplePassword"
  }
  ```

#### 响应
- **状态码**: `200 OK`（登录成功） 或 `401 Unauthorized`（用户名或密码错误）
- **响应体**（成功登录返回）:
  ```json
  {
    "message": "Login successful",
    "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9..."
  }
  ```

---


## 发送登录请求
```cpp
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

// 假设这是在某个方法中实现的
void sendLoginRequest(const QString& username, const QString& password) {
    // 创建网络管理器
    QNetworkAccessManager* manager = new QNetworkAccessManager();

    // 设置请求 URL
    QUrl url("http://localhost:8080/api/users/login"); // 替换为你的 API 地址
    QNetworkRequest request(url);

    // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 构建 JSON 请求体
    QJsonObject json;
    json["username"] = username;
    json["password"] = password;
    QJsonDocument jsonDoc(json);
    QByteArray requestData = jsonDoc.toJson();

    // 发送 POST 请求
    QNetworkReply* reply = manager->post(request, requestData);

    // 连接信号，等待响应
    connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            // 请求成功，读取响应数据
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject responseObject = jsonResponse.object();

            // 解析响应 JSON
            QString message = responseObject["message"].toString();
            QString token = responseObject["token"].toString();

            qDebug() << "Login Successful: " << message;
            qDebug() << "Token: " << token;
        } else {
            // 请求失败
            qDebug() << "Error:" << reply->errorString();
        }
        reply->deleteLater(); // 释放资源
    });

    // 清理管理器（这里可以根据实际需求决定是否释放）
    connect(manager, &QNetworkAccessManager::destroyed, [manager]() {
        qDebug() << "Network manager destroyed.";
    });
}
```

### 2. 使用示例
假设有一个登录按钮，并绑定以下槽函数,即可实现向服务器发送报文并且接收回复报文：

```cpp

void onLoginButtonClicked() {
    QString username = "exampleUser";
    QString password = "examplePassword";

    sendLoginRequest(username, password);
}
```
