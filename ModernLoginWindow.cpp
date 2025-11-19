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

ModernLoginWindow::ModernLoginWindow(QWidget *parent)
    : QMainWindow(parent), networkManager(new QNetworkAccessManager(this)) {
    setupUI();
    connectSignals();
}

ModernLoginWindow::~ModernLoginWindow() {
}

void ModernLoginWindow::setupUI() {
    setWindowTitle("Sky Wings - Modern Airline Booking");
    
    // 设置窗口大小和居中
    resize(1200, 700);
    QRect screen = QApplication::primaryScreen()->geometry();
    move((screen.width() - width()) / 2, (screen.height() - height()) / 2);
    
    setStyleSheet("QMainWindow { background-color: #F9FAFB; }");

    // 创建主要中央部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建主布局 - 两列布局
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ==================== 左侧品牌面板 ====================
    QFrame *leftPanel = new QFrame();
    leftPanel->setObjectName("leftPanel");
    leftPanel->setStyleSheet(
        "QFrame#leftPanel {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "       stop:0 #0052CC, stop:1 #1E88E5);"
        "   border-radius: 12px;"
        "}"
    );
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(60, 80, 60, 80);
    leftLayout->setSpacing(30);
    leftLayout->setAlignment(Qt::AlignCenter | Qt::AlignTop);

    // 品牌标题
    brandLabel = new QLabel("Sky Wings");
    brandLabel->setStyleSheet(
        "QLabel {"
        "   color: white;"
        "   font-size: 48px;"
        "   font-weight: 700;"
        "   letter-spacing: 2px;"
        "}"
    );
    leftLayout->addWidget(brandLabel, 0, Qt::AlignHCenter);

    // 品牌描述
    QLabel *brandDesc = new QLabel(
        "Your gateway to the world's skies\n\n"
        "Experience seamless flight booking with\n"
        "real-time availability, competitive pricing,\n"
        "and 24/7 customer support."
    );
    brandDesc->setStyleSheet(
        "QLabel {"
        "   color: rgba(255, 255, 255, 0.9);"
        "   font-size: 16px;"
        "   line-height: 1.6;"
        "   padding: 20px;"
        "}"
    );
    brandDesc->setAlignment(Qt::AlignCenter | Qt::AlignTop);
    leftLayout->addWidget(brandDesc, 1, Qt::AlignTop);

    // 特性列表
    QLabel *features = new QLabel(
        "✓ Real-time flight search\n"
        "✓ Best price guarantee\n"
        "✓ Flexible cancellation\n"
        "✓ 24/7 support"
    );
    features->setStyleSheet(
        "QLabel {"
        "   color: rgba(255, 255, 255, 0.85);"
        "   font-size: 14px;"
        "   line-height: 2.0;"
        "   padding: 20px;"
        "}"
    );
    leftLayout->addWidget(features, 0, Qt::AlignBottom | Qt::AlignLeft);

    // ==================== 右侧登录面板 ====================
    QFrame *rightPanel = new QFrame();
    rightPanel->setStyleSheet("QFrame { background-color: white; }");
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(60, 60, 60, 60);
    rightLayout->setSpacing(25);

    // 欢迎文本
    welcomeLabel = new QLabel("Welcome Back");
    welcomeLabel->setStyleSheet(
        "QLabel {"
        "   color: #111827;"
        "   font-size: 32px;"
        "   font-weight: 700;"
        "}"
    );
    rightLayout->addWidget(welcomeLabel);

    QLabel *subtext = new QLabel("Sign in to your account to continue");
    subtext->setStyleSheet(
        "QLabel {"
        "   color: #6B7280;"
        "   font-size: 14px;"
        "   margin-bottom: 20px;"
        "}"
    );
    rightLayout->addWidget(subtext);

    // 错误标签
    errorLabel = new QLabel();
    errorLabel->setStyleSheet(
        "QLabel {"
        "   color: #EF4444;"
        "   background-color: #FEE2E2;"
        "   border-radius: 6px;"
        "   padding: 12px;"
        "   font-size: 12px;"
        "}"
    );
    errorLabel->setVisible(false);
    errorLabel->setWordWrap(true);
    rightLayout->addWidget(errorLabel);

    // 邮箱输入
    QLabel *emailLabel = new QLabel("Email Address");
    emailLabel->setStyleSheet("QLabel { color: #374151; font-weight: 600; font-size: 13px; }");
    rightLayout->addWidget(emailLabel);

    emailInput = new QLineEdit();
    emailInput->setPlaceholderText("you@example.com");
    emailInput->setStyleSheet(
        "QLineEdit {"
        "   background-color: white;"
        "   color: #111827;"
        "   border: 2px solid #E5E7EB;"
        "   border-radius: 8px;"
        "   padding: 12px 15px;"
        "   font-size: 14px;"
        "}"
        "QLineEdit:focus {"
        "   border: 2px solid #0052CC;"
        "   background-color: #F0F4FF;"
        "}"
    );
    emailInput->setFixedHeight(45);
    rightLayout->addWidget(emailInput);

    // 密码输入
    QLabel *passwordLabel = new QLabel("Password");
    passwordLabel->setStyleSheet("QLabel { color: #374151; font-weight: 600; font-size: 13px; }");
    rightLayout->addWidget(passwordLabel);

    passwordInput = new QLineEdit();
    passwordInput->setPlaceholderText("Enter your password");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setStyleSheet(
        "QLineEdit {"
        "   background-color: white;"
        "   color: #111827;"
        "   border: 2px solid #E5E7EB;"
        "   border-radius: 8px;"
        "   padding: 12px 15px;"
        "   font-size: 14px;"
        "}"
        "QLineEdit:focus {"
        "   border: 2px solid #0052CC;"
        "   background-color: #F0F4FF;"
        "}"
    );
    passwordInput->setFixedHeight(45);
    rightLayout->addWidget(passwordInput);

    // 记住密码复选框
    rememberCheckBox = new QCheckBox("Remember me");
    rememberCheckBox->setStyleSheet(
        "QCheckBox {"
        "   color: #6B7280;"
        "   font-size: 13px;"
        "}"
        "QCheckBox::indicator {"
        "   width: 18px;"
        "   height: 18px;"
        "}"
        "QCheckBox::indicator:unchecked {"
        "   background-color: white;"
        "   border: 2px solid #E5E7EB;"
        "   border-radius: 4px;"
        "}"
        "QCheckBox::indicator:checked {"
        "   background-color: #0052CC;"
        "   border: 2px solid #0052CC;"
        "   border-radius: 4px;"
        "   image: url(:/icons/check.svg);"
        "}"
    );
    rightLayout->addWidget(rememberCheckBox);

    // 登录按钮
    loginButton = new QPushButton("Sign In");
    loginButton->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #0052CC, stop:1 #1E88E5);"
        "   color: white;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 14px 24px;"
        "   font-weight: 700;"
        "   font-size: 15px;"
        "   letter-spacing: 0.5px;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #0048B8, stop:1 #1976D2);"
        "}"
        "QPushButton:pressed {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #003FA3, stop:1 #155DB3);"
        "}"
    );
    loginButton->setFixedHeight(50);
    loginButton->setCursor(Qt::PointingHandCursor);
    rightLayout->addWidget(loginButton);

    // 分隔符
    QFrame *divider = new QFrame();
    divider->setStyleSheet("QFrame { background-color: #E5E7EB; }");
    divider->setFixedHeight(1);
    rightLayout->addWidget(divider);

    // 注册链接
    QHBoxLayout *registerLayout = new QHBoxLayout();
    QLabel *noAccountLabel = new QLabel("Don't have an account?");
    noAccountLabel->setStyleSheet("QLabel { color: #6B7280; font-size: 13px; }");
    registerLayout->addWidget(noAccountLabel);
    registerLayout->addSpacing(5);

    registerButton = new QPushButton("Create one");
    registerButton->setFlat(true);
    registerButton->setStyleSheet(
        "QPushButton {"
        "   color: #0052CC;"
        "   border: none;"
        "   background: transparent;"
        "   font-weight: 600;"
        "   font-size: 13px;"
        "   padding: 0px;"
        "}"
        "QPushButton:hover {"
        "   color: #0048B8;"
        "}"
    );
    registerButton->setCursor(Qt::PointingHandCursor);
    registerLayout->addWidget(registerButton);
    registerLayout->addStretch();

    rightLayout->addLayout(registerLayout);
    rightLayout->addStretch();

    // 添加左右面板到主布局
    mainLayout->addWidget(leftPanel, 1);
    mainLayout->addWidget(rightPanel, 1);
}

void ModernLoginWindow::connectSignals() {
    connect(loginButton, &QPushButton::clicked, this, &ModernLoginWindow::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &ModernLoginWindow::onRegisterClicked);
    connect(rememberCheckBox, &QCheckBox::toggled, this, &ModernLoginWindow::onCheckBoxToggled);
}

void ModernLoginWindow::onLoginClicked() {
    QString email = emailInput->text().trimmed();
    QString password = passwordInput->text();

    // 验证输入
    if (email.isEmpty() || password.isEmpty()) {
        showErrorMessage("Please fill in all fields");
        return;
    }

    if (!email.contains("@")) {
        showErrorMessage("Please enter a valid email address");
        return;
    }

    // 禁用登录按钮并显示加载状态
    loginButton->setEnabled(false);
    loginButton->setText("Signing in...");
    errorLabel->setVisible(false);

    sendLoginRequest(email, password);
}

void ModernLoginWindow::onRegisterClicked() {
    qDebug() << "Register button clicked - Navigate to registration page";
    // 将在InterfaceManager中进行页面切换
}

void ModernLoginWindow::onCheckBoxToggled(bool checked) {
    qDebug() << "Remember me:" << checked;
}

void ModernLoginWindow::sendLoginRequest(const QString &email, const QString &password) {
    QUrl url("http://localhost:8080/api/users/login");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jsonData;
    jsonData["email"] = email;
    jsonData["password"] = password;

    QJsonDocument doc(jsonData);
    QNetworkReply *reply = networkManager->post(request, doc.toJson());

    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, [this, reply](QNetworkReply::NetworkError error) {
        Q_UNUSED(error);
        loginButton->setEnabled(true);
        loginButton->setText("Sign In");
        showErrorMessage("Network error: " + reply->errorString());
    });

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        loginButton->setEnabled(true);
        loginButton->setText("Sign In");

        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject responseObject = jsonResponse.object();

            if (responseObject["code"].toInt() == 200) {
                onLoginSuccess(responseObject["data"].toObject());
            } else {
                onLoginFailed(responseObject["message"].toString());
            }
        } else {
            onLoginFailed("Request failed: " + reply->errorString());
        }
        reply->deleteLater();
    });
}

void ModernLoginWindow::onLoginSuccess(const QJsonObject &userData) {
    showSuccessMessage("Login successful! Redirecting...");
    qDebug() << "User logged in:" << userData;
    // 这里可以保存用户数据到UserManager单例
}

void ModernLoginWindow::onLoginFailed(const QString &error) {
    showErrorMessage(error);
}

void ModernLoginWindow::showErrorMessage(const QString &message) {
    errorLabel->setText("✕ " + message);
    errorLabel->setVisible(true);
}

void ModernLoginWindow::showSuccessMessage(const QString &message) {
    QMessageBox::information(this, "Success", message);
}
