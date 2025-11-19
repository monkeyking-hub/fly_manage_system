#include "ModernLoginWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QMessageBox>
#include <QDebug>
#include <QApplication>
#include <QScreen>
#include <QFont>
#include <QFontDatabase>

ModernLoginWindow::ModernLoginWindow(QWidget *parent)
    : QMainWindow(parent), networkManager(new QNetworkAccessManager(this)) {
    setupUI();
    connectSignals();
    applyTheme();
    setupAnimations();
}

ModernLoginWindow::~ModernLoginWindow() {
}

void ModernLoginWindow::setupUI() {
    setWindowTitle("Sky Wings - 现代化航班预订系统");
    setFixedSize(1200, 700);
    
    // 居中窗口
    QRect screen = QApplication::primaryScreen()->geometry();
    move((screen.width() - width()) / 2, (screen.height() - height()) / 2);
    
    // 创建中央部件
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // 主布局 - 两列设计
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // ==================== 左侧品牌面板 ====================
    leftPanel = new QFrame();
    leftPanel->setFixedWidth(480);
    leftPanel->setStyleSheet(
        "QFrame {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "       stop:0 #1E40AF, stop:0.5 #3B82F6, stop:1 #0EA5E9);"
        "   border-top-left-radius: 12px;"
        "   border-bottom-left-radius: 12px;"
        "}"
    );
    
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(60, 80, 60, 80);
    leftLayout->setSpacing(30);
    leftLayout->setAlignment(Qt::AlignCenter);
    
    // 品牌图标 - 使用Unicode字符代替图片
    brandIcon = new QLabel("✈");
    brandIcon->setStyleSheet(
        "QLabel {"
        "   color: white;"
        "   font-size: 72px;"
        "   font-weight: 300;"
        "   margin-bottom: 20px;"
        "}"
    );
    brandIcon->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(brandIcon, 0, Qt::AlignCenter);
    
    // 品牌名称
    brandLabel = new QLabel("Sky Wings");
    brandLabel->setStyleSheet(
        "QLabel {"
        "   color: white;"
        "   font-size: 42px;"
        "   font-weight: 700;"
        "   letter-spacing: 3px;"
        "   margin-bottom: 20px;"
        "}"
    );
    brandLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(brandLabel, 0, Qt::AlignCenter);
    
    // 品牌描述
    brandDescription = new QLabel(
        "您的空中门户，连接世界的每一片蓝天\n\n"
        "体验无缝的航班预订服务\n"
        "实时可用性查询，最具竞争力的价格\n"
        "全天候客户支持"
    );
    brandDescription->setStyleSheet(
        "QLabel {"
        "   color: rgba(255, 255, 255, 0.9);"
        "   font-size: 16px;"
        "   line-height: 1.8;"
        "   text-align: center;"
        "   margin-bottom: 40px;"
        "}"
    );
    brandDescription->setAlignment(Qt::AlignCenter);
    brandDescription->setWordWrap(true);
    leftLayout->addWidget(brandDescription, 0, Qt::AlignCenter);
    
    // 特性列表
    featuresLabel = new QLabel(
        "✨ 实时航班搜索\n"
        "🏆 最佳价格保证\n"
        "🔄 灵活退改政策\n"
        "🎧 24/7 客服支持"
    );
    featuresLabel->setStyleSheet(
        "QLabel {"
        "   color: rgba(255, 255, 255, 0.85);"
        "   font-size: 15px;"
        "   line-height: 2.0;"
        "   text-align: center;"
        "}"
    );
    featuresLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(featuresLabel, 1, Qt::AlignCenter);
    
    // ==================== 右侧登录面板 ====================
    rightPanel = new QFrame();
    rightPanel->setStyleSheet(
        "QFrame {"
        "   background-color: #FFFFFF;"
        "   border-top-right-radius: 12px;"
        "   border-bottom-right-radius: 12px;"
        "}"
    );
    
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(80, 80, 80, 80);
    rightLayout->setSpacing(25);
    
    // 欢迎标题
    welcomeLabel = new QLabel("欢迎回来");
    welcomeLabel->setStyleSheet(
        "QLabel {"
        "   color: #1E293B;"
        "   font-size: 32px;"
        "   font-weight: 700;"
        "   margin-bottom: 8px;"
        "}"
    );
    rightLayout->addWidget(welcomeLabel);
    
    // 副标题
    subtitleLabel = new QLabel("登录您的账户，继续您的旅程");
    subtitleLabel->setStyleSheet(
        "QLabel {"
        "   color: #64748B;"
        "   font-size: 16px;"
        "   font-weight: 400;"
        "   margin-bottom: 40px;"
        "}"
    );
    rightLayout->addWidget(subtitleLabel);
    
    // 邮箱输入框
    emailInput = new QLineEdit();
    emailInput->setPlaceholderText("请输入邮箱地址");
    emailInput->setStyleSheet(
        "QLineEdit {"
        "   background-color: #F8FAFC;"
        "   border: 2px solid #E2E8F0;"
        "   border-radius: 10px;"
        "   padding: 16px 20px;"
        "   font-size: 15px;"
        "   color: #1E293B;"
        "   margin-bottom: 5px;"
        "}"
        "QLineEdit:focus {"
        "   border-color: #3B82F6;"
        "   background-color: #FFFFFF;"
        "}"
        "QLineEdit:hover {"
        "   border-color: #CBD5E1;"
        "}"
    );
    rightLayout->addWidget(emailInput);
    
    // 密码输入框
    passwordInput = new QLineEdit();
    passwordInput->setPlaceholderText("请输入密码");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setStyleSheet(
        "QLineEdit {"
        "   background-color: #F8FAFC;"
        "   border: 2px solid #E2E8F0;"
        "   border-radius: 10px;"
        "   padding: 16px 20px;"
        "   font-size: 15px;"
        "   color: #1E293B;"
        "   margin-bottom: 20px;"
        "}"
        "QLineEdit:focus {"
        "   border-color: #3B82F6;"
        "   background-color: #FFFFFF;"
        "}"
        "QLineEdit:hover {"
        "   border-color: #CBD5E1;"
        "}"
    );
    rightLayout->addWidget(passwordInput);
    
    // 记住我选项
    QHBoxLayout *rememberLayout = new QHBoxLayout();
    rememberLayout->setContentsMargins(0, 0, 0, 0);
    
    rememberMeCheckBox = new QCheckBox("记住我");
    rememberMeCheckBox->setStyleSheet(
        "QCheckBox {"
        "   color: #64748B;"
        "   font-size: 14px;"
        "   font-weight: 500;"
        "}"
        "QCheckBox::indicator {"
        "   width: 18px;"
        "   height: 18px;"
        "   border-radius: 4px;"
        "   border: 2px solid #CBD5E1;"
        "   background-color: #FFFFFF;"
        "}"
        "QCheckBox::indicator:checked {"
        "   background-color: #3B82F6;"
        "   border-color: #3B82F6;"
        "   image: url(data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMTAiIGhlaWdodD0iOCIgdmlld0JveD0iMCAwIDEwIDgiIGZpbGw9Im5vbmUiIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyI+PHBhdGggZD0iTTEgNEwzLjUgNi41TDkgMSIgc3Ryb2tlPSJ3aGl0ZSIgc3Ryb2tlLXdpZHRoPSIyIiBzdHJva2UtbGluZWNhcD0icm91bmQiIHN0cm9rZS1saW5lam9pbj0icm91bmQiLz48L3N2Zz4=);"
        "}"
    );
    rememberLayout->addWidget(rememberMeCheckBox);
    rememberLayout->addStretch();
    
    forgotPasswordButton = new QPushButton("忘记密码？");
    forgotPasswordButton->setStyleSheet(
        "QPushButton {"
        "   background: transparent;"
        "   color: #3B82F6;"
        "   border: none;"
        "   font-size: 14px;"
        "   font-weight: 500;"
        "   text-decoration: underline;"
        "}"
        "QPushButton:hover {"
        "   color: #1E40AF;"
        "}"
    );
    rememberLayout->addWidget(forgotPasswordButton);
    
    rightLayout->addLayout(rememberLayout);
    
    // 错误/成功消息标签
    errorLabel = new QLabel();
    errorLabel->setStyleSheet(
        "QLabel {"
        "   color: #EF4444;"
        "   font-size: 14px;"
        "   padding: 8px 12px;"
        "   background-color: #FEE2E2;"
        "   border-radius: 6px;"
        "   margin: 10px 0;"
        "}"
    );
    errorLabel->hide();
    rightLayout->addWidget(errorLabel);
    
    successLabel = new QLabel();
    successLabel->setStyleSheet(
        "QLabel {"
        "   color: #10B981;"
        "   font-size: 14px;"
        "   padding: 8px 12px;"
        "   background-color: #D1FAE5;"
        "   border-radius: 6px;"
        "   margin: 10px 0;"
        "}"
    );
    successLabel->hide();
    rightLayout->addWidget(successLabel);
    
    // 登录按钮
    loginButton = new QPushButton("登 录");
    loginButton->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #3B82F6, stop:1 #1E40AF);"
        "   color: white;"
        "   border: none;"
        "   border-radius: 10px;"
        "   padding: 16px 0;"
        "   font-size: 16px;"
        "   font-weight: 600;"
        "   letter-spacing: 1px;"
        "   margin: 20px 0;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #60A5FA, stop:1 #2563EB);"
        "}"
        "QPushButton:pressed {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #2563EB, stop:1 #1D4ED8);"
        "}"
    );
    rightLayout->addWidget(loginButton);
    
    // 分隔线和注册链接
    QHBoxLayout *dividerLayout = new QHBoxLayout();
    dividerLayout->setContentsMargins(0, 20, 0, 0);
    
    QFrame *leftLine = new QFrame();
    leftLine->setFrameShape(QFrame::HLine);
    leftLine->setStyleSheet(
        "QFrame {"
        "   background-color: #E2E8F0;"
        "   height: 1px;"
        "}"
    );
    dividerLayout->addWidget(leftLine);
    
    dividerLabel = new QLabel("还没有账户？");
    dividerLabel->setStyleSheet(
        "QLabel {"
        "   color: #64748B;"
        "   font-size: 14px;"
        "   margin: 0 10px;"
        "}"
    );
    dividerLayout->addWidget(dividerLabel);
    
    QFrame *rightLine = new QFrame();
    rightLine->setFrameShape(QFrame::HLine);
    rightLine->setStyleSheet(
        "QFrame {"
        "   background-color: #E2E8F0;"
        "   height: 1px;"
        "}"
    );
    dividerLayout->addWidget(rightLine);
    
    rightLayout->addLayout(dividerLayout);
    
    // 注册按钮
    registerButton = new QPushButton("立即注册");
    registerButton->setStyleSheet(
        "QPushButton {"
        "   background: transparent;"
        "   color: #3B82F6;"
        "   border: 2px solid #3B82F6;"
        "   border-radius: 10px;"
        "   padding: 14px 0;"
        "   font-size: 15px;"
        "   font-weight: 600;"
        "   margin-top: 20px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #EFF6FF;"
        "   color: #1E40AF;"
        "}"
    );
    rightLayout->addWidget(registerButton);
    
    rightLayout->addStretch();
    
    // 添加面板到主布局
    mainLayout->addWidget(leftPanel);
    mainLayout->addWidget(rightPanel);
}

void ModernLoginWindow::connectSignals() {
    connect(loginButton, &QPushButton::clicked, this, &ModernLoginWindow::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &ModernLoginWindow::onRegisterClicked);
    connect(forgotPasswordButton, &QPushButton::clicked, this, &ModernLoginWindow::onForgotPasswordClicked);
    connect(rememberMeCheckBox, &QCheckBox::toggled, this, &ModernLoginWindow::onRememberMeToggled);
    connect(emailInput, &QLineEdit::textChanged, this, &ModernLoginWindow::onInputChanged);
    connect(passwordInput, &QLineEdit::textChanged, this, &ModernLoginWindow::onInputChanged);
}

void ModernLoginWindow::applyTheme() {
    // 应用现代化主题
    QFile styleFile(":/modern_theme.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        setStyleSheet(styleFile.readAll());
    }
}

void ModernLoginWindow::setupAnimations() {
    // 设置淡入动画
    opacityEffect = new QGraphicsOpacityEffect(this);
    centralWidget->setGraphicsEffect(opacityEffect);
    
    fadeInAnimation = new QPropertyAnimation(opacityEffect, "opacity", this);
    fadeInAnimation->setDuration(800);
    fadeInAnimation->setStartValue(0.0);
    fadeInAnimation->setEndValue(1.0);
    fadeInAnimation->setEasingCurve(QEasingCurve::OutCubic);
    fadeInAnimation->start();
}

void ModernLoginWindow::onLoginClicked() {
    QString email = emailInput->text().trimmed();
    QString password = passwordInput->text();
    
    // 基本验证
    if (email.isEmpty()) {
        showErrorMessage("请输入邮箱地址");
        return;
    }
    
    if (password.isEmpty()) {
        showErrorMessage("请输入密码");
        return;
    }
    
    if (!email.contains("@") || !email.contains(".")) {
        showErrorMessage("请输入有效的邮箱地址");
        return;
    }
    
    // 动画效果
    animateButton(loginButton);
    
    // 发送登录请求
    sendLoginRequest(email, password);
}

void ModernLoginWindow::onRegisterClicked() {
    // 切换到注册页面
    animateButton(registerButton);
    
    // 这里应该切换到注册窗口
    // InterfaceManager::instance()->switchToPage("modern_registerWindow");
}

void ModernLoginWindow::onForgotPasswordClicked() {
    // 处理忘记密码
    animateButton(forgotPasswordButton);
    
    QMessageBox::information(this, "重置密码", 
        "密码重置链接已发送到您的邮箱地址。\n"
        "请检查您的邮箱并按照说明操作。");
}

void ModernLoginWindow::onLoginSuccess(const QJsonObject &userData) {
    showSuccessMessage("登录成功！正在跳转...");
    
    // 保存用户数据
    // UserSession::instance()->setUser(userData);
    
    // 延迟跳转
    QTimer::singleShot(1500, [this]() {
        // InterfaceManager::instance()->switchToPage("modern_dashboard");
        close();
    });
}

void ModernLoginWindow::onLoginFailed(const QString &error) {
    showErrorMessage(error);
}

void ModernLoginWindow::onRememberMeToggled(bool checked) {
    Q_UNUSED(checked);
    // 保存记住我状态
}

void ModernLoginWindow::onInputChanged() {
    // 清除错误消息
    errorLabel->hide();
    successLabel->hide();
}

void ModernLoginWindow::sendLoginRequest(const QString &email, const QString &password) {
    QJsonObject requestData;
    requestData["email"] = email;
    requestData["password"] = password;
    requestData["rememberMe"] = rememberMeCheckBox->isChecked();
    
    QJsonDocument doc(requestData);
    QByteArray data = doc.toJson();
    
    QUrl url("http://localhost:8080/api/users/login");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    QNetworkReply *reply = networkManager->post(request, data);
    
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument response = QJsonDocument::fromJson(reply->readAll());
            QJsonObject obj = response.object();
            
            if (obj["code"].toInt() == 200) {
                onLoginSuccess(obj["data"].toObject());
            } else {
                onLoginFailed(obj["message"].toString());
            }
        } else {
            onLoginFailed("网络连接失败，请检查网络设置");
        }
        reply->deleteLater();
    });
}

void ModernLoginWindow::showErrorMessage(const QString &message) {
    errorLabel->setText(message);
    errorLabel->show();
    successLabel->hide();
}

void ModernLoginWindow::showSuccessMessage(const QString &message) {
    successLabel->setText(message);
    successLabel->show();
    errorLabel->hide();
}

void ModernLoginWindow::animateButton(QPushButton *button) {
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