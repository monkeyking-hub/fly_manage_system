# Sky Wings 现代前端 - 快速入门指南

## 🚀 快速开始

### 项目概览

这是对原有Qt航班预订系统的全面前端重设计，采用现代化的无图片设计方案，具有高级感和专业外观。

### 文件清单

新增加的现代化前端文件:

```
核心文件:
├── modern_theme.qss                    # 全局主题样式
├── ModernLoginWindow.h/cpp             # 登录窗口
├── ModernDashboardWindow.h/cpp         # 主仪表盘
├── ModernFlightSearchWindow.h/cpp      # 搜索结果页
├── ModernBookingWizard.h/cpp           # 预订向导
└── ModernMainWindow.h/cpp              # 主容器窗口

文档文件:
├── API_MODERN_REDESIGN.md              # 完整API文档 (1000+行)
├── FRONTEND_REDESIGN_PLAN.md           # 设计计划
├── REDESIGN_SUMMARY.md                 # 详细总结
└── MODERN_FRONTEND_QUICKSTART.md       # 本文件
```

## 🎯 主要功能

### 1. 现代登录窗口 (ModernLoginWindow)

**特点**:
- 双列布局 - 左侧品牌叙述, 右侧登录表单
- 渐变色背景 - 蓝色渐变
- 实时表单验证
- 记住密码功能
- 平滑的错误/成功提示

**使用**:
```cpp
ModernLoginWindow *login = new ModernLoginWindow();
login->show();
```

### 2. 现代仪表盘 (ModernDashboardWindow)

**特点**:
- 粘性导航栏 - 用户菜单和登出按钮
- 大型搜索栏 - 出发地、目的地、日期等
- 快速访问卡片 - 常用功能快捷方式
- 特色航班列表 - 实时从API加载

**使用**:
```cpp
ModernDashboardWindow *dashboard = new ModernDashboardWindow();
dashboard->show();
```

### 3. 高级搜索页面 (ModernFlightSearchWindow)

**特点**:
- 左侧过滤面板
  - 价格范围滑块
  - 航班类型选择
  - 出发时间过滤
  - 航空公司多选
  
- 右侧结果展示
  - 排序控件
  - 卡片式航班列表
  - 快速预订按钮

**使用**:
```cpp
ModernFlightSearchWindow *search = 
    new ModernFlightSearchWindow("New York", "Los Angeles", QDate(2024, 2, 15));
search->show();
```

### 4. 预订向导 (ModernBookingWizard)

**特点**:
- 4步多步骤向导
  1. 乘客信息
  2. 座位选择
  3. 预订审查
  4. 支付信息

- 进度条可视化
- 前进/返回导航

**使用**:
```cpp
QJsonObject flight;
flight["airline"] = "Sky Airways";
// ... 填充航班数据

ModernBookingWizard *wizard = new ModernBookingWizard(flight);
wizard->show();
```

## 🎨 设计系统

### 色彩方案

```
主色调:        #0052CC  (深蓝)
辅助色:        #1E88E5  (海洋蓝)
强调色:        #42A5F5  (天空蓝)
成功色:        #10B981  (翠绿)
错误色:        #EF4444  (红)
背景色:        #F9FAFB  (极浅灰)
边框色:        #E5E7EB  (浅灰)
文本色:        #111827  (深灰)
```

### 常见组件样式

#### 按钮
```qss
QPushButton {
    background-color: #0052CC;
    color: white;
    border: none;
    border-radius: 6px;
    padding: 10px 24px;
    font-weight: 600;
}

QPushButton:hover {
    background-color: #0048B8;
}
```

#### 输入框
```qss
QLineEdit {
    background-color: white;
    border: 1px solid #E5E7EB;
    border-radius: 6px;
    padding: 10px 12px;
}

QLineEdit:focus {
    border: 2px solid #0052CC;
}
```

#### 卡片
```qss
QFrame#card {
    background-color: white;
    border: 1px solid #E5E7EB;
    border-radius: 12px;
}
```

## 🔌 API集成

### 登录流程

```cpp
// 1. 用户在登录窗口输入邮箱和密码
// 2. 点击"Sign In"按钮
// 3. 发送POST请求到 /api/users/login

POST /api/users/login
Content-Type: application/json

{
  "email": "user@example.com",
  "password": "SecurePass123"
}

// 4. 响应 (成功)
{
  "code": 200,
  "message": "Login successful",
  "data": {
    "token": "eyJhbGciOiJIUzI1NiIs...",
    "userId": 1,
    "username": "john_doe"
  }
}
```

### 搜索航班

```cpp
// 1. 用户在仪表盘输入搜索条件
// 2. 点击"Search"按钮
// 3. 导航到搜索结果页

POST /api/flights/search
Content-Type: application/json

{
  "departure": "New York",
  "destination": "Los Angeles",
  "departureDate": "2024-02-15",
  "passengers": 1,
  "tripType": "one-way"
}

// 4. 响应 (成功)
{
  "code": 200,
  "data": [
    {
      "id": 101,
      "airline": "Sky Airways",
      "flightNumber": "SA123",
      "departureTime": "08:00",
      "arrivalTime": "11:30",
      "pricing": {"economy": 249.99},
      ...
    }
  ]
}
```

### 创建预订

```cpp
// 1. 用户在预订向导中输入乘客信息
// 2. 选择座位
// 3. 审查信息
// 4. 输入支付信息
// 5. 点击"Confirm Booking"

POST /api/bookings/create
Authorization: Bearer <token>
Content-Type: application/json

{
  "flightId": 101,
  "passengers": [{
    "firstName": "John",
    "lastName": "Doe",
    "email": "john@example.com",
    "passportNumber": "AB123456"
  }],
  "seats": ["12A"],
  "seatClass": "economy"
}

// 4. 响应 (成功)
{
  "code": 201,
  "message": "Booking created successfully",
  "data": {
    "bookingId": "BK202402150001",
    "bookingReference": "SA7HK2",
    "totalPrice": 249.99
  }
}
```

## 📋 完整API列表

参考 `API_MODERN_REDESIGN.md` 获取所有API端点的详细说明:

| 端点 | 方法 | 说明 |
|------|------|------|
| `/api/users/register` | POST | 用户注册 |
| `/api/users/login` | POST | 用户登录 |
| `/api/users/profile` | GET | 获取个人资料 |
| `/api/flights/search` | POST | 搜索航班 |
| `/api/flights/featured` | GET | 获取特色航班 |
| `/api/bookings/create` | POST | 创建预订 |
| `/api/orders` | GET | 获取订单列表 |
| `/api/payments/initiate` | POST | 发起支付 |
| `/api/chat/send` | POST | 发送聊天消息 |

## 🛠️ 编译和运行

### 项目配置 (.pro文件)

确保在Qt项目文件中包含:

```pro
CONFIG += c++17

SOURCES += \
    ModernLoginWindow.cpp \
    ModernDashboardWindow.cpp \
    ModernFlightSearchWindow.cpp \
    ModernBookingWizard.cpp \
    ModernMainWindow.cpp \
    ... (其他文件)

HEADERS += \
    ModernLoginWindow.h \
    ModernDashboardWindow.h \
    ModernFlightSearchWindow.h \
    ModernBookingWizard.h \
    ModernMainWindow.h \
    ... (其他文件)

RESOURCES += modern_theme.qss
```

### 编译

```bash
# Linux/macOS
mkdir build && cd build
cmake ..
make

# 或使用Qt Creator
# File -> Open Project -> Select .pro file
# Build -> Build Project
```

### 运行

```bash
./SkyWings
# 或从Qt Creator直接运行
```

## 🎬 用户流程

```
开始
  ↓
[登录窗口] (ModernLoginWindow)
  ↓ 登录成功
[仪表盘] (ModernDashboardWindow)
  ├─ 快速访问 → 相应功能
  └─ 搜索航班
    ↓
[搜索结果] (ModernFlightSearchWindow)
  ├─ 过滤和排序
  └─ 选择航班
    ↓
[预订向导] (ModernBookingWizard)
  ├─ 第1步: 乘客信息
  ├─ 第2步: 座位选择
  ├─ 第3步: 审查
  ├─ 第4步: 支付
  └─ 确认
    ↓
[预订完成]
  ↓
返回仪表盘
```

## 🔍 调试技巧

### 查看网络请求

```cpp
// 在ModernLoginWindow::sendLoginRequest()中
QNetworkReply *reply = networkManager->post(request, doc.toJson());

connect(reply, &QNetworkReply::errorOccurred, this, [reply]() {
    qDebug() << "Error:" << reply->errorString();
    qDebug() << "Status Code:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
});
```

### 查看API响应

```cpp
// 在任何网络请求的响应处理中
QByteArray responseData = reply->readAll();
qDebug() << "Response:" << QString::fromUtf8(responseData);

QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
qDebug() << "JSON:" << jsonResponse.toJson(QJsonDocument::Indented);
```

### 启用样式调试

```cpp
// 在main.cpp中
QApplication app(argc, argv);

// 加载主题
QFile styleFile(":/styles/modern_theme.qss");
if (styleFile.open(QFile::ReadOnly)) {
    app.setStyleSheet(QLatin1String(styleFile.readAll()));
    styleFile.close();
}
```

## ⚙️ 配置和自定义

### 更改主色

编辑 `modern_theme.qss`:

```qss
/* 更改主色从 #0052CC 到你需要的颜色 */
@primary-color: #0052CC;
@primary-hover: #0048B8;
@primary-active: #003FA3;
```

### 修改字体大小

```qss
QLabel#titleLabel {
    font-size: 32px;  /* 改这里 */
    font-weight: 700;
}
```

### 调整间距

```cpp
// 在各个窗口中
QVBoxLayout *layout = new QVBoxLayout();
layout->setContentsMargins(40, 40, 40, 40);  // 改变边距
layout->setSpacing(20);  // 改变间距
```

## 📱 响应式设计

虽然是桌面应用，但设计考虑了灵活性:

```cpp
// 使用弹性布局而不是固定大小
QHBoxLayout *layout = new QHBoxLayout();
layout->addWidget(leftPanel, 0);  // 固定宽度
layout->addWidget(rightPanel, 1); // 可伸缩

// 使用setMinimumWidth而不是setFixedWidth
component->setMinimumWidth(300);
component->setMaximumWidth(600);
```

## 🚀 部署

### 打包应用

```bash
# macOS
macdeployqt SkyWings.app

# Windows (需要安装 windeployqt)
windeployqt.exe SkyWings.exe

# Linux
# 将可执行文件和必要的库打包
```

### 配置API端点

创建配置文件 `config.json`:

```json
{
  "api_base_url": "https://api.skywings.com",
  "api_timeout": 30000,
  "theme": "modern"
}
```

## 🎓 扩展指南

### 添加新窗口

1. 创建新的Header和Source文件
2. 继承QMainWindow或QWidget
3. 在setupUI()中设计界面
4. 在system.cpp中注册
5. 使用InterfaceManager::instance()->registerPage()

### 添加新API调用

```cpp
// 在任何窗口中
QUrl url("http://localhost:8080/api/endpoint");
QNetworkRequest request(url);
request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

// 添加认证token (如果需要)
QString token = "eyJhbGc..."; // 从UserManager获取
request.setRawHeader("Authorization", "Bearer " + token.toUtf8());

QJsonObject data;
data["key"] = "value";
QJsonDocument doc(data);

QNetworkReply *reply = networkManager->post(request, doc.toJson());
```

## ⚡ 性能优化

1. **异步加载**: 在单独的线程中加载数据
2. **缓存**: 缓存航班搜索结果
3. **延迟加载**: 只加载可见的列表项
4. **内存管理**: 使用deleteLater()正确释放

## 🐛 故障排除

### 问题: 登录后没有跳转到仪表盘

**解决**:
```cpp
// 在onLoginSuccess()中确保有页面切换
InterfaceManager::instance()->switchToPage("modern_dashboard");
```

### 问题: 搜索返回空结果

**解决**:
1. 检查API服务器是否运行
2. 验证搜索参数格式
3. 查看网络请求日志

### 问题: 样式不生效

**解决**:
```cpp
// 确保在创建QApplication后立即加载QSS
QFile styleFile(":/modern_theme.qss");
if (styleFile.open(QFile::ReadOnly)) {
    app.setStyleSheet(styleFile.readAll());
}
```

## 📞 获取帮助

- 查看 `API_MODERN_REDESIGN.md` 了解所有API
- 查看 `REDESIGN_SUMMARY.md` 了解完整设计说明
- 查看源代码注释

## 📄 相关文件

- `FRONTEND_REDESIGN_PLAN.md` - 设计方案详解
- `API_MODERN_REDESIGN.md` - API完整文档 (1000+行)
- `REDESIGN_SUMMARY.md` - 项目总结

---

**版本**: 1.0  
**更新**: 2024年2月  
**状态**: 生产就绪 ✅
