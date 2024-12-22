#include "newregisterwindow.h"
#include "interfacemanager.h"
#include "ui_newregisterwindow.h"

#include <QApplication>
#include <QCheckBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QRandomGenerator>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QWidget>

newRegisterWindow::newRegisterWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::newRegisterWindow)
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
    outerContainer->setFixedSize(400, 600);
    outerContainer->setStyleSheet("#outerContainer {"
                                  "  background-color: rgba(203, 229, 254, 0.8);" // 半透明背景
                                  "  border: 5px solid #fbf6fd;"
                                  "  border-radius: 20px;"
                                  "  padding: 40px;"
                                  "}");

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

    // 注册标题
    QLabel *titleLabel = new QLabel("注 册", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; text-align: center;");
    titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    innerLayout->addWidget(titleLabel);

    //添加空白
    QSpacerItem *spacer1 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);
    innerLayout->addItem(spacer1);

    // 用户名输入框
    usernameField = new AnimatedInputField("用户名", false, this);
    usernameField->setMinimumHeight(60);
    usernameField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    innerLayout->addWidget(usernameField);

    //添加空白
    QSpacerItem *spacer6 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    innerLayout->addItem(spacer6);

    //邮箱输入框
    emailField = new AnimatedInputField("邮箱", false, this);
    emailField->setMinimumHeight(60);
    emailField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    innerLayout->addWidget(emailField);

    //添加空白
    QSpacerItem *spacer2 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    innerLayout->addItem(spacer2);

    // 密码输入框
    passwordField = new AnimatedInputField("密码", true, this);
    passwordField->setMinimumHeight(60);
    passwordField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    innerLayout->addWidget(passwordField);

    QCheckBox *toggleCheckBox = new QCheckBox("显示密码");
    connect(toggleCheckBox, &QCheckBox::toggled, passwordField, [this](bool checked) {
        passwordField->lineEdit()->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
    });
    innerLayout->addWidget(toggleCheckBox);

    //添加空白
    QSpacerItem *spacer3 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    innerLayout->addItem(spacer3);

    // 手机号输入框
    phoneField = new AnimatedInputField("手机号", false, this);
    phoneField->setMinimumHeight(60);
    phoneField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    innerLayout->addWidget(phoneField);

    //添加空白
    QSpacerItem *spacer4 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    innerLayout->addItem(spacer4);

    //身份证号输入框
    idField = new AnimatedInputField("身份证号", false, this);
    idField->setMinimumHeight(60);
    idField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    innerLayout->addWidget(idField);

    //邮箱验证码输入框
    emailCodeField = new AnimatedInputField("邮箱验证码", false, this);
    emailCodeField->setMinimumHeight(60);
    emailCodeField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    innerLayout->addWidget(emailCodeField);

    // 发送验证码按钮
    QPushButton *sendEmailCodeBtn = new QPushButton("发送验证码", this);
    sendEmailCodeBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sendEmailCodeBtn->setStyleSheet("QPushButton {"
                                    "    border: none;"
                                    "    background-color: transparent;"
                                    "    text-align: right;"
                                    "    color: blue;"
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
    connect(sendEmailCodeBtn,
            &QPushButton::clicked,
            this,
            &newRegisterWindow::onSendEmailCodeBtnClicked);
    innerLayout->addWidget(sendEmailCodeBtn);

    // 注册按钮
    QPushButton *registerBtn = new QPushButton("注册", this);
    registerBtn->setStyleSheet("QPushButton {"
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
    registerBtn->setCursor(Qt::PointingHandCursor);
    registerBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(registerBtn, &QPushButton::clicked, this, &newRegisterWindow::onRegisterButtonClicked);
    innerLayout->addWidget(registerBtn);

    //返回按钮
    QPushButton *returnBtn = new QPushButton("返回", this);
    returnBtn->setStyleSheet("QPushButton {"
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
    returnBtn->setCursor(Qt::PointingHandCursor);
    returnBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(returnBtn, &QPushButton::clicked, this, &newRegisterWindow::onReturnButtonClicked);
    innerLayout->addWidget(returnBtn);

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

    // 窗口配置
    setWindowTitle("云程 注册");
    resize(1300, 700); // 设置窗口初始大小
}

void newRegisterWindow::onRegisterButtonClicked() //点击注册按钮触发事件
{
    QString email = emailField->text();
    QString password = passwordField->text();
    QString usrName = usernameField->text();
    QString phone = phoneField->text();
    QString idNumber = idField->text();
    QString emailCodeInput = emailCodeField->text();

    //检查是否有空格
    if (email.contains(" ") || password.contains(" ") || usrName.contains(" ")
        || phone.contains(" ") || idNumber.contains(" ") || emailCodeInput.contains(" ")) {
        QMessageBox::critical(nullptr,
                              "注册失败",
                              "注册失败: 输入内容不能含有空格！",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }

    //检查是否有全部填写
    if (email == "" || password == "" || usrName == "" || phone == "" || idNumber == ""
        || emailCodeInput == "") {
        QMessageBox::critical(nullptr,
                              "注册失败",
                              "必须要填写全部信息才能注册！",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }

    if (!email.contains("@") || !email.contains(".com")) {
        QMessageBox::critical(nullptr,
                              "注册失败",
                              "邮箱格式不正确！",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }

    //检查验证码是否正确
    if (emailCodeInput != emailCode) {
        QMessageBox::critical(nullptr,
                              "注册失败",
                              "验证码不正确！",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }

    //将输入框内容清空
    QLineEdit *emailLineEdit = emailField->lineEdit();
    QLineEdit *passwordLineEdit = passwordField->lineEdit();
    QLineEdit *usrNameLineEdit = usernameField->lineEdit();
    QLineEdit *phoneLineEdit = phoneField->lineEdit();
    QLineEdit *idLineEdit = idField->lineEdit();
    QLineEdit *emailCodeLineEdit = emailCodeField->lineEdit();
    emailLineEdit->setText("");
    passwordLineEdit->setText("");
    usrNameLineEdit->setText("");
    phoneLineEdit->setText("");
    idLineEdit->setText("");
    emailCodeLineEdit->setText("");

    emit registerRequested(usrName, password, email, phone, idNumber, emailCodeInput);
}

void newRegisterWindow::onReturnButtonClicked()
{
    //将输入框内容清空
    QLineEdit *emailLineEdit = emailField->lineEdit();
    QLineEdit *passwordLineEdit = passwordField->lineEdit();
    QLineEdit *usrNameLineEdit = usernameField->lineEdit();
    QLineEdit *phoneLineEdit = phoneField->lineEdit();
    QLineEdit *idLineEdit = idField->lineEdit();
    QLineEdit *emailCodeLineEdit = emailCodeField->lineEdit();
    emailLineEdit->setText("");
    passwordLineEdit->setText("");
    usrNameLineEdit->setText("");
    phoneLineEdit->setText("");
    idLineEdit->setText("");
    emailCodeLineEdit->setText("");
    InterfaceManager::instance()->switchToPage("lxt_newLoginWindow");
}

void newRegisterWindow::onSendEmailCodeBtnClicked()
{
    // // 创建网络管理器
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    // // 设置请求 URL
    QUrl url("http://localhost:8080/api/send");
    QNetworkRequest request(url);

    // // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //生成随机6位数字作为验证码
    int randNum = QRandomGenerator::global()->bounded(100000, 1000000);
    QString randomString = QString::number(randNum);
    emailCode = randomString;

    // // 构建 JSON 请求体
    QJsonObject json;
    json["to"] = emailField->text();
    json["subject"] = "云程账号注册验证码";
    json["body"] = "您的邮箱验证码是：" + randomString;

    QJsonDocument jsonDoc(json);
    QByteArray requestData = jsonDoc.toJson();

    // // 发送 POST 请求
    QNetworkReply *reply = manager->post(request, requestData);
    // 连接信号，等待响应
    connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            // 请求成功，读取响应数据
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject responseObject = jsonResponse.object();

            // 解析响应 JSON
            int code = responseObject["code"].toInt(); // 获取返回的 code
            QString message = responseObject["message"].toString();
            QJsonObject data = responseObject["data"].toObject();

            if (code == 200) {
                //发送成功
                QMessageBox::information(nullptr, "成功", "验证码发送成功");
            } else {
                // 发送失败，弹出错误提示框
                qDebug() << "Sending failed, code: " << code;
                QMessageBox::critical(nullptr,
                                      "Sending Error",
                                      "Sending failed: " + message,
                                      QMessageBox::Ok,
                                      QMessageBox::Ok);
            }
        } else {
            // 请求失败，弹出错误提示框
            QString errorString = reply->errorString();
            qDebug() << "Error:" << errorString;

            // 创建 QMessageBox 来显示错误信息
            QMessageBox::critical(nullptr,
                                  "Sending Error",
                                  "Sending failed: " + errorString,
                                  QMessageBox::Ok,
                                  QMessageBox::Ok);
        }

        reply->deleteLater(); // 释放资源
    });
}

newRegisterWindow::~newRegisterWindow()
{
    delete ui;
}

void registerHandler::handleRegister(const QString &usrname,
                                     const QString &password,
                                     const QString &email,
                                     const QString &phone,
                                     const QString &idNumber,
                                     const QString &emailCodeInput,
                                     const int &age,
                                     const QString &gender,
                                     const int &usrLevel)
{
    // // 创建网络管理器
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    // // 设置请求 URL
    QUrl url("http://localhost:8080/api/users/register");
    QNetworkRequest request(url);

    // // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // // 构建 JSON 请求体
    QJsonObject json;
    json["username"] = usrname;
    json["password"] = password;
    json["email"] = email;
    json["phone"] = phone;
    json["age"] = age;
    json["gender"] = gender;
    json["idNumber"] = idNumber;
    json["userLevel"] = usrLevel;

    QJsonDocument jsonDoc(json);
    QByteArray requestData = jsonDoc.toJson();

    // // 发送 POST 请求
    QNetworkReply *reply = manager->post(request, requestData);
    // 连接信号，等待响应
    connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            // 请求成功，读取响应数据
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject responseObject = jsonResponse.object();

            // 解析响应 JSON
            int code = responseObject["code"].toInt(); // 获取返回的 code
            QString message = responseObject["message"].toString();
            QJsonObject data = responseObject["data"].toObject();

            if (code == 200) {
                // 注册成功,弹出信息框
                QString username = data["username"].toString();

                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Information);      // 设置图标类型
                msgBox.setWindowTitle("注册成功");             // 设置窗口标题
                msgBox.setText(username + "，您已成功注册！"); // 设置提示文本
                msgBox.setStandardButtons(QMessageBox::Ok); // 设置标准按钮（此处只有“确定”按钮）

                // 显示消息框
                msgBox.exec();

                // 页面切换
                InterfaceManager::instance()->switchToPage("lxt_newLoginWindow");
            } else {
                // 注册失败，弹出错误提示框
                qDebug() << "Register failed, code: " << code;
                QMessageBox::critical(nullptr,
                                      "Register Error",
                                      "Register failed: " + message,
                                      QMessageBox::Ok,
                                      QMessageBox::Ok);
            }
        } else {
            // 请求失败，弹出错误提示框
            QString errorString = reply->errorString();
            qDebug() << "Error:" << errorString;

            // 创建 QMessageBox 来显示错误信息
            QMessageBox::critical(nullptr,
                                  "Register Error",
                                  "Request failed: " + errorString,
                                  QMessageBox::Ok,
                                  QMessageBox::Ok);
        }

        reply->deleteLater(); // 释放资源
    });
}
