# Sky Wings 现代化前端系统 - 快速入门指南

## 🚀 快速开始

### 环境要求
- **Qt**: 5.12 或更高版本
- **C++**: C++17 标准支持
- **CMake**: 3.10 或更高版本
- **操作系统**: Windows 10+, macOS 10.14+, Ubuntu 18.04+

### 编译运行

```bash
# 进入项目目录
cd /home/engine/project

# 创建构建目录
mkdir build && cd build

# 配置项目
cmake ..

# 编译
make

# 运行
./SkyWings
```

---

## 🎨 设计系统概览

### 核心设计理念
- **无图片设计**: 完全使用 CSS/QSS 样式实现视觉效果
- **现代化风格**: 扁平化设计 + 渐变色彩 + 流畅动画
- **高级感体验**: 企业级 UI/UX 标准
- **响应式布局**: 适配不同屏幕尺寸

### 色彩系统
```css
/* 主色调 */
--primary-color: #1E40AF     /* 深蓝色 */
--primary-light: #3B82F6    /* 海洋蓝 */
--accent-color: #0EA5E9     /* 天蓝色 */

/* 功能色彩 */
--success-color: #10B981     /* 成功绿 */
--warning-color: #F59E0B     /* 警告橙 */
--error-color: #EF4444       /* 错误红 */

/* 中性色彩 */
--gray-50: #F8FAFC          /* 极浅灰 */
--gray-100: #F1F5F9         /* 浅灰 */
--gray-800: #1E293B         /* 深灰 */
--gray-900: #0F172A         /* 极深灰 */
```

---

## 🏗️ 架构结构

### 核心组件

```
ModernMainWindow          # 主容器窗口
├── ModernLoginWindow     # 登录窗口
├── ModernDashboardWindow # 仪表盘窗口
└── ModernFlightSearchWindow # 航班搜索窗口
```

### 页面管理
使用 `InterfaceManager` 单例进行页面切换：

```cpp
// 切换到登录页面
InterfaceManager::instance()->switchToPage("modern_loginWindow");

// 切换到仪表盘
InterfaceManager::instance()->switchToPage("modern_dashboard");
```

---

## 💻 使用指南

### 1. 登录系统

**访问方式**: 启动应用后自动显示登录页面

**功能特性**:
- 双列设计：左侧品牌展示 + 右侧登录表单
- 实时表单验证
- 记住登录状态
- 忘记密码功能
- 注册跳转

**API 集成**:
```cpp
// 登录请求端点
POST /api/users/login
{
  "email": "user@example.com",
  "password": "password123",
  "rememberMe": true
}
```

### 2. 仪表盘界面

**布局结构**:
- **顶部导航栏**: Logo + 主要功能按钮
- **左侧边栏**: 用户信息 + 导航菜单
- **主内容区**: 欢迎信息 + 快速搜索 + 特色推荐

**核心功能**:
- 快速航班搜索
- 特色航班推荐
- 最近搜索历史
- 快速操作面板

### 3. 航班搜索

**搜索参数**:
- 出发地和目的地
- 出发日期和返回日期
- 乘客数量和舱位等级
- 高级过滤条件

**过滤选项**:
- 直飞航班
- 可退款航班
- 价格范围
- 航空公司偏好
- 时间段选择

---

## 🎯 组件使用示例

### 现代化按钮

```cpp
QPushButton *button = new QPushButton("搜索航班");
button->setStyleSheet(
    "QPushButton {"
    "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
    "       stop:0 #3B82F6, stop:1 #1E40AF);"
    "   color: white;"
    "   border: none;"
    "   border-radius: 10px;"
    "   padding: 14px 24px;"
    "   font-size: 15px;"
    "   font-weight: 600;"
    "}"
    "QPushButton:hover {"
    "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
    "       stop:0 #60A5FA, stop:1 #2563EB);"
    "}"
);
```

### 现代化输入框

```cpp
QLineEdit *input = new QLineEdit();
input->setPlaceholderText("请输入邮箱地址");
input->setStyleSheet(
    "QLineEdit {"
    "   background-color: #F8FAFC;"
    "   border: 2px solid #E2E8F0;"
    "   border-radius: 10px;"
    "   padding: 16px 20px;"
    "   font-size: 15px;"
    "   color: #1E293B;"
    "}"
    "QLineEdit:focus {"
    "   border-color: #3B82F6;"
    "   background-color: #FFFFFF;"
    "}"
);
```

### 现代化卡片

```cpp
QFrame *card = new QFrame();
card->setStyleSheet(
    "QFrame {"
    "   background: white;"
    "   border: 1px solid #E2E8F0;"
    "   border-radius: 12px;"
    "   padding: 24px;"
    "}"
    "QFrame:hover {"
    "   border-color: #CBD5E1;"
    "   box-shadow: 0 4px 6px -1px rgba(0, 0, 0, 0.1);"
    "}"
);
```

---

## 🔧 自定义样式

### 应用主题

```cpp
void applyTheme() {
    QFile styleFile(":/modern_theme.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        qApp->setStyleSheet(styleFile.readAll());
    }
}
```

### 创建自定义组件

```cpp
class ModernButton : public QPushButton {
    Q_OBJECT
public:
    ModernButton(const QString &text, QWidget *parent = nullptr) 
        : QPushButton(text, parent) {
        setupStyle();
    }
    
private:
    void setupStyle() {
        setStyleSheet(
            "ModernButton {"
            "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
            "       stop:0 #3B82F6, stop:1 #1E40AF);"
            "   color: white;"
            "   border: none;"
            "   border-radius: 8px;"
            "   padding: 12px 24px;"
            "   font-size: 14px;"
            "   font-weight: 600;"
            "}"
        );
    }
};
```

---

## 📡 API 集成

### 网络请求基类

```cpp
class ApiClient : public QObject {
    Q_OBJECT
public:
    explicit ApiClient(QObject *parent = nullptr) 
        : QObject(parent) {
        manager = new QNetworkAccessManager(this);
    }
    
    void post(const QString &endpoint, const QJsonObject &data) {
        QUrl url(QString("http://localhost:8080/api%1").arg(endpoint));
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        
        QJsonDocument doc(data);
        QByteArray postData = doc.toJson();
        
        QNetworkReply *reply = manager->post(request, postData);
        connect(reply, &QNetworkReply::finished, [this, reply]() {
            handleResponse(reply);
        });
    }
    
private:
    QNetworkAccessManager *manager;
    
    void handleResponse(QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument response = QJsonDocument::fromJson(reply->readAll());
            // 处理成功响应
        } else {
            // 处理错误
        }
        reply->deleteLater();
    }
};
```

### 使用示例

```cpp
ApiClient *api = new ApiClient(this);

// 用户登录
QJsonObject loginData;
loginData["email"] = "user@example.com";
loginData["password"] = "password123";
api->post("/users/login", loginData);

// 搜索航班
QJsonObject searchData;
searchData["from"] = "PEK";
searchData["to"] = "SHA";
searchData["departureDate"] = "2024-02-15";
api->post("/flights/search", searchData);
```

---

## 🎨 动画效果

### 按钮点击动画

```cpp
void animateButton(QPushButton *button) {
    QPropertyAnimation *animation = new QPropertyAnimation(button, "geometry", this);
    QRect originalGeometry = button->geometry();
    
    animation->setDuration(150);
    animation->setStartValue(originalGeometry);
    animation->setEndValue(originalGeometry.adjusted(-2, -2, 2, 2));
    animation->setEasingCurve(QEasingCurve::OutCubic);
    
    connect(animation, &QPropertyAnimation::finished, [button, originalGeometry]() {
        QPropertyAnimation *animation2 = new QPropertyAnimation(button, "geometry");
        animation2->setDuration(150);
        animation2->setStartValue(button->geometry());
        animation2->setEndValue(originalGeometry);
        animation2->setEasingCurve(QEasingCurve::OutBounce);
        animation2->start(QPropertyAnimation::DeleteWhenStopped);
    });
    
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}
```

### 页面切换动画

```cpp
void animatePageTransition(QWidget *newPage) {
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(newPage);
    newPage->setGraphicsEffect(effect);
    
    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity", this);
    animation->setDuration(300);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}
```

---

## 🐛 常见问题

### Q1: 如何更改主题颜色？
**A**: 修改 `modern_theme.qss` 文件中的颜色变量，或使用 `setStyleSheet()` 动态更新。

### Q2: 如何添加新的页面？
**A**: 
1. 创建新的窗口类继承自 `QMainWindow`
2. 在 `system.cpp` 中注册页面
3. 使用 `InterfaceManager::switchToPage()` 切换页面

### Q3: 如何处理网络请求错误？
**A**: 检查 `QNetworkReply::error()` 和响应状态码，参考 API 文档的错误处理部分。

### Q4: 如何实现暗色主题？
**A**: 在 `modern_theme.qss` 中添加 `DarkTheme` 样式规则，运行时切换样式。

---

## 📚 相关文档

- **完整 API 文档**: `API_MODERN_REDESIGN.md`
- **项目总结**: `MODERN_REDESIGN_SUMMARY.md`
- **主题样式**: `modern_theme.qss`
- **核心组件**: `Modern*.h/.cpp` 文件

---

## 🚀 下一步

1. **完善航班搜索窗口** - 实现 `ModernFlightSearchWindow.cpp`
2. **添加预订向导** - 创建多步骤预订流程
3. **集成支付系统** - 对接真实支付 API
4. **优化性能** - 减少内存占用和启动时间

---

## 📞 技术支持

如需技术支持，请参考：
- **API 文档**: 详细的接口说明和示例
- **代码注释**: 每个组件都有详细的中文注释
- **示例代码**: 完整的使用案例和最佳实践

---

**祝您使用愉快！** 🎉

© 2024 Sky Wings 航班预订系统 - 现代化前端团队