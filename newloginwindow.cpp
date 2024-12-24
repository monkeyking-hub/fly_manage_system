#include "newloginwindow.h"
#include <QCheckBox>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "interfacemanager.h"
#include "ui_newloginwindow.h"
#include "usermanager.h"
#include "userwindow.h"

newLoginWindow::newLoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::newLoginWindow)
{
    ui->setupUi(this);

    // 创建中央窗口部件
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setFixedSize(1300, 700);
    centralWidget->setObjectName("centralwidget");
    setCentralWidget(centralWidget);

    // 设置窗口背景颜色
    centralWidget->setStyleSheet(
        "#centralwidget{"
        "background-color: #282a37; color: #fff; font-family: 'Poppins';"
        "background-image: url(':/blue.png');"
        "background-position: center; background-repeat: no-repeat; background-size: cover;"
        "}");

    // 创建外层容器，用于添加边框和圆角
    QWidget *outerContainer = new QWidget(this);
    outerContainer->setObjectName("outerContainer"); // 设置对象名称
    outerContainer->setFixedSize(400, 300);
    outerContainer->setStyleSheet("#outerContainer {"
                                  "  background-color: rgba(203, 229, 254, 0.8);" // 半透明背景
                                  "  border: 5px solid #fbf6fd;"
                                  "  border-radius: 20px;"
                                  "  padding: 40px;"
                                  "}");

    QLabel *lbl_welcome = new QLabel(this);
    lbl_welcome->setText("欢迎使用 云程 ");
    lbl_welcome->setStyleSheet("font: 45pt '千图笔锋手写体'; color: black; text-align: center;");
    lbl_welcome->setGeometry(480, 600, 400, 300);
    QPropertyAnimation *animation = new QPropertyAnimation(lbl_welcome, "geometry");
    animation->setDuration(1500);
    animation->setStartValue(QRect(480, 600, 400, 300));
    animation->setEndValue(QRect(480, -40, 400, 300));
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    // 从左向右的飞机
    QLabel *airplaneLabel1 = new QLabel(this);
    airplaneLabel1->setPixmap(
        QPixmap(":/plane_right.png").scaled(100, 50, Qt::KeepAspectRatio)); // 设置飞机图片
    airplaneLabel1->setGeometry(-100, 300, 100, 50); // 初始位置在屏幕外面，靠左上

    QPropertyAnimation *airplaneAnimation1 = new QPropertyAnimation(airplaneLabel1, "geometry");
    airplaneAnimation1->setLoopCount(-1);  // 无限循环
    airplaneAnimation1->setDuration(6000); // 动画持续时间，单位为毫秒
    airplaneAnimation1->setEasingCurve(QEasingCurve::Linear);     // 线性过渡
    airplaneAnimation1->setStartValue(QRect(-100, 300, 100, 50)); // 飞机起始位置（左侧）
    airplaneAnimation1->setEndValue(
        QRect(centralWidget->width(), 300, 100, 50)); // 飞机结束位置（右侧）

    airplaneAnimation1->start(); // 启动飞机动画

    //从右向左的飞机
    QLabel *airplaneLabel2 = new QLabel(this);
    airplaneLabel2->setPixmap(
        QPixmap(":/plane_left.png").scaled(100, 50, Qt::KeepAspectRatio)); // 设置飞机图片
    airplaneLabel2->setGeometry(centralWidget->width(), 150, 100, 50); // 初始位置在屏幕外面，靠右

    QPropertyAnimation *airplaneAnimation2 = new QPropertyAnimation(airplaneLabel2, "geometry");
    airplaneAnimation2->setLoopCount(-1);  // 无限循环
    airplaneAnimation2->setDuration(6000); // 动画持续时间，单位为毫秒
    airplaneAnimation2->setEasingCurve(QEasingCurve::Linear); // 线性过渡
    airplaneAnimation2->setStartValue(
        QRect(centralWidget->width(), 150, 100, 50));           // 飞机起始位置（右侧）
    airplaneAnimation2->setEndValue(QRect(-100, 150, 100, 50)); // 飞机结束位置（左侧）

    airplaneAnimation2->start(); // 启动飞机动画

    // 创建内部布局
    QVBoxLayout *innerLayout = new QVBoxLayout();

    // 登录标题
    QLabel *titleLabel = new QLabel("登 录", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; text-align: center;");
    titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    innerLayout->addWidget(titleLabel);

    //添加空白
    QSpacerItem *spacer1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    innerLayout->addItem(spacer1);

    // 邮箱输入框
    emailField = new AnimatedInputField("邮箱", false, this);
    emailField->setMinimumHeight(60);
    emailField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    innerLayout->addWidget(emailField);

    //添加空白
    QSpacerItem *spacer2 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);
    innerLayout->addItem(spacer2);

    // 密码输入框
    passwordField = new AnimatedInputField("密码", true, this);
    passwordField->setMinimumHeight(60);
    passwordField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    innerLayout->addWidget(passwordField);

    //添加空白
    QSpacerItem *spacer3 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    innerLayout->addItem(spacer3);

    QHBoxLayout *checkboxAndadminbtn = new QHBoxLayout();
    QCheckBox *toggleCheckBox = new QCheckBox("显示密码");
    connect(toggleCheckBox, &QCheckBox::toggled, passwordField, [this](bool checked) {
        passwordField->lineEdit()->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
    });

    checkboxAndadminbtn->addWidget(toggleCheckBox);

    // 前往管理员登录界面按钮
    QPushButton *adminLoginBtn = new QPushButton("管理员登录", this);
    adminLoginBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    adminLoginBtn->setStyleSheet("QPushButton {"
                                 "    border: none;"
                                 "    background-color: transparent;"
                                 "    text-align: right;"
                                 "    color: black;"
                                 "}"

                                 "QPushButton:hover {"
                                 "    text-align: right;"
                                 "    color: blue;"
                                 "    text-decoration: underline;"
                                 "}"

                                 "QPushButton:pressed {"
                                 "    text-align: right;"
                                 "    color: darkblue;"
                                 "    padding: 2px;"
                                 "}");

    checkboxAndadminbtn->addWidget(adminLoginBtn);

    innerLayout->addLayout(checkboxAndadminbtn);

    //添加空白
    QSpacerItem *spacer4 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);
    innerLayout->addItem(spacer4);

    // 登录按钮
    QPushButton *loginBtn = new QPushButton("登录", this);
    loginBtn->setStyleSheet("QPushButton {"
                            "    background: white;"
                            "    color: black;"
                            "    border-radius: 20px;"
                            "    font-size: 16px;"
                            "    padding: 10px;"
                            "    font-weight: bold;"
                            "    border: 2px solid #1d7bff;" // 添加边框颜色
                            "}"
                            "QPushButton:hover {"
                            "    background-color: rgb(29, 123, 255);" // 悬浮时的背景色
                            "    color: white;" // 悬浮时字体颜色变白
                            "}"
                            "QPushButton:pressed {"
                            "    background-color: rgb(29, 123, 255);" // 点击时的背景色
                            "}");
    loginBtn->setCursor(Qt::PointingHandCursor);
    loginBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(loginBtn, &QPushButton::clicked, this, &newLoginWindow::onLogInButtonClicked);
    innerLayout->addWidget(loginBtn);

    //添加空白
    QSpacerItem *spacer5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    innerLayout->addItem(spacer5);

    // 注册提示
    QPushButton *signupBtn = new QPushButton("没有账号? 去注册.", this);
    signupBtn->setStyleSheet("QPushButton {"
                             "    border: none;"
                             "    background-color: transparent;"
                             "    color: black;" /* 默认文字颜色 */
                             "}"

                             "QPushButton:hover {"
                             "    color: blue;" /* 鼠标悬停时文字颜色变蓝 */
                             "    text-decoration: underline;" /* 鼠标悬停时添加下划线 */
                             "}"

                             "QPushButton:pressed {"
                             "    color: darkblue;" /* 按下时文字变为深蓝色 */
                             "    padding: 2px;"    /* 模拟按钮按下的轻微位移 */
                             "}");
    signupBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    innerLayout->addWidget(signupBtn);
    connect(signupBtn, &QPushButton::clicked, this, &newLoginWindow::onSignUpButtonClicked);

    // 将内部布局添加到外部容器
    outerContainer->setLayout(innerLayout);

    // 设置主布局，使外部容器居中
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addItem(
        new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding)); // 顶部间距
    mainLayout->addWidget(outerContainer, 0, Qt::AlignCenter);
    mainLayout->addItem(
        new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding)); // 底部间距

    // 将主布局设置为中央部件的布局
    centralWidget->setLayout(mainLayout);

    // 连接信号与槽
    connect(adminLoginBtn, &QPushButton::clicked, this, [this]() {
        QLineEdit *emailLineEdit = emailField->lineEdit();
        QLineEdit *pwdLineEdit = passwordField->lineEdit();
        emailLineEdit->setText("");
        pwdLineEdit->setText("");
        InterfaceManager::instance()->switchToPage("lxt_adminLoginWindow");
    });

    // 窗口配置
    setWindowTitle("云程 登录");
    resize(1300, 700); // 设置窗口初始大小
}

void newLoginWindow::onSignUpButtonClicked()
{
    QLineEdit *emailLineEdit = emailField->lineEdit();
    QLineEdit *pwdLineEdit = passwordField->lineEdit();
    emailLineEdit->setText("");
    pwdLineEdit->setText("");
    InterfaceManager::instance()->switchToPage("lxt_newRegisterWindow");
}

void newLoginWindow::onLogInButtonClicked() //点击登录按钮触发事件
{
    QString email = emailField->text();
    QString password = passwordField->text();

    //检查是否有空格
    if (email.contains(" ") || password.contains(" ")) {
        QMessageBox::critical(nullptr,
                              "登录失败",
                              "登录失败: 邮箱或密码含有空格！",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }

    //检查是否为空
    if (email == "" || password == "") {
        QMessageBox::critical(nullptr,
                              "登录失败",
                              "登录失败: 邮箱或密码为空！",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }

    //将输入框内容清空
    QLineEdit *emailLineEdit = emailField->lineEdit();
    QLineEdit *passwordLineEdit = passwordField->lineEdit();
    emailLineEdit->setText("");
    passwordLineEdit->setText("");

    emit loginRequested(email, password);
}

newLoginWindow::~newLoginWindow()
{
    delete ui;
}

void loginHandler::handleLogin(const QString &email, const QString &password)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QUrl url("http://127.0.0.1:8080/api/users/login");
    //QUrl luUrl("http://192.168.238.11:8080/api/users/get");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["email"] = email;
    json["password"] = password;

    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, [this, reply, email]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject responseObject = jsonResponse.object();

            if (responseObject["code"].toInt() == 200) {
                // 登录成功
                QJsonObject data = responseObject["data"].toObject();

                // 查询用户完整信息
                QNetworkAccessManager *luManager = new QNetworkAccessManager(this);
                QUrl luUrl("http://localhost:8080/api/users/get");
                QNetworkRequest luRequest(luUrl);
                luRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

                QJsonObject luJson;
                luJson["email"] = email;
                QNetworkReply *luReply = luManager->post(luRequest, QJsonDocument(luJson).toJson());

                connect(luReply, &QNetworkReply::finished, [luReply]() {
                    if (luReply->error() == QNetworkReply::NoError) {
                        QByteArray resdata = luReply->readAll();
                        QJsonDocument lujsonResponse = QJsonDocument::fromJson(resdata);
                        QJsonObject luresponseObject = lujsonResponse.object();

                        qDebug() << luresponseObject["code"].toInt();

                        if (luresponseObject["code"].toInt() == 200) {
                            // 完整用户信息获取成功
                            QJsonObject luData = luresponseObject["data"].toObject();
                            User loginUser;
                            loginUser.id=luData["id"].toInt();
                            loginUser.username = luData["username"].toString();
                            loginUser.email = luData["email"].toString();
                            loginUser.phonenumber = luData["phone"].toString();
                            loginUser.age = luData["age"].toInt();
                            loginUser.sex = luData["gender"].toString();
                            UserManager::getInstance()->setCurrentUser(loginUser);

                            // 页面切换到主界面
                            InterfaceManager::instance()->switchToPage("lxt_mainInterface");
                        } else {
                            QMessageBox::critical(nullptr,
                                                  "Error",
                                                  "Failed to fetch user details: "
                                                      + luReply->errorString());
                        }
                    } else {
                        // 请求失败
                        qDebug() << "Error fetching user details:" << luReply->errorString();
                        QMessageBox::critical(nullptr,
                                              "Error",
                                              "Failed to fetch user details: "
                                                  + luReply->errorString());
                    }
                    luReply->deleteLater();
                });
            } else {
                // 登录失败
                qDebug() << "Login failed, code: " << responseObject["code"].toInt();
                QMessageBox::critical(nullptr, "登录失败", responseObject["message"].toString());
            }
        } else {
            // 登录请求失败
            qDebug() << "Error logging in:" << reply->errorString();
            QMessageBox::critical(nullptr, "登录失败", "Request failed: " + reply->errorString());
        }
        reply->deleteLater();
    });
}
