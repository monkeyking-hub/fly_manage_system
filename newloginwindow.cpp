#include "newloginwindow.h"
#include "ui_newloginwindow.h"
#include "interfacemanager.h"

AnimatedInputField::AnimatedInputField(const QString &placeholderText, bool isPassword, QWidget *parent)
    : QWidget(parent), label(new QLabel(this)), input(new QLineEdit(this)), animation(new QPropertyAnimation(label, "pos", this)) {

    QVBoxLayout *mainLayout = new QVBoxLayout(this);  // 创建主布局
    mainLayout->setContentsMargins(0, 0, 0, 0);  // 去除边距
    mainLayout->setSpacing(0);  // 去除间距

    // 设置标签（初始位置）
    label->setText(placeholderText);  // 设置标签文本
    label->setStyleSheet("background:transparent; color: black; font-size: 16px;");  // 设置标签样式
    label->move(5, 25);  // 设置标签的初始位置
    label->resize(200, 20);  // 设置标签的大小

    // 设置输入框
    input->setStyleSheet(
        "background: transparent; border: none; border-bottom: 2px solid white; "
        "font-size: 16px; color: white; padding: 5px;"
        );
    if (isPassword) {
        input->setEchoMode(QLineEdit::Password);  // 如果是密码框，设置为密码输入模式
    }
    mainLayout->addWidget(input);  // 将输入框添加到布局中

    // 设置动画效果
    animation->setDuration(300);  // 设置动画持续时间
    animation->setEasingCurve(QEasingCurve::InOutQuad);  // 设置动画的缓动曲线

    // 安装事件过滤器以捕获焦点事件
    input->installEventFilter(this);
}

QString AnimatedInputField::text() const { return input->text(); }  // 获取输入框的文本

bool AnimatedInputField::eventFilter(QObject *obj, QEvent *event){
    if (obj == input) {
        if (event->type() == QEvent::FocusIn) {
            onFocusIn(); // 如果输入框获得焦点，执行动画
        } else if (event->type() == QEvent::FocusOut) {
            onFocusOut(); // 如果输入框失去焦点，执行动画
        }
    }
    return QWidget::eventFilter(obj, event); // 调用基类的事件过滤器
}

newLoginWindow::newLoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::newLoginWindow)
{
    ui->setupUi(this);

    // 创建中央窗口部件
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setFixedSize(1300,700);
    centralWidget->setObjectName("centralwidget");
    setCentralWidget(centralWidget);


    // 设置窗口背景颜色
    centralWidget->setStyleSheet(
        "#centralwidget{"
        "background-color: #282a37; color: #fff; font-family: 'Poppins';"
        "background-image: url(':/blue.png');"
        "background-position: center; background-repeat: no-repeat; background-size: cover;"
        "}"
        );

    // 创建外层容器，用于添加边框和圆角
    QWidget *outerContainer = new QWidget(this);
    outerContainer->setObjectName("outerContainer"); // 设置对象名称
    outerContainer->setFixedSize(400,300);
    outerContainer->setStyleSheet(
        "#outerContainer {"
        "  background-color: rgba(203, 229, 254, 0.8);" // 半透明背景
        "  border: 5px solid #fbf6fd;"
        "  border-radius: 20px;"
        "  padding: 40px;"
        "}"
        );

    QLabel *lbl_welcome=new QLabel(this);
    lbl_welcome->setText("欢迎使用 云程 ");
    lbl_welcome->setStyleSheet("font: 45pt '千图笔锋手写体'; color: black; text-align: center;");
    lbl_welcome->setGeometry(480,600,400,300);
    QPropertyAnimation *animation = new QPropertyAnimation(lbl_welcome, "geometry");
    animation->setDuration(1500);
    animation->setStartValue(QRect(480, 600, 400, 300));
    animation->setEndValue(QRect(480, -40, 400, 300));
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    // 从左向右的飞机
    QLabel *airplaneLabel1 = new QLabel(this);
    airplaneLabel1->setPixmap(QPixmap(":/plane_right.png").scaled(100, 50, Qt::KeepAspectRatio));  // 设置飞机图片
    airplaneLabel1->setGeometry(-100, 300, 100, 50);  // 初始位置在屏幕外面，靠左上

    QPropertyAnimation *airplaneAnimation1 = new QPropertyAnimation(airplaneLabel1, "geometry");
    airplaneAnimation1->setLoopCount(-1);  // 无限循环
    airplaneAnimation1->setDuration(6000);  // 动画持续时间，单位为毫秒
    airplaneAnimation1->setEasingCurve(QEasingCurve::Linear);  // 线性过渡
    airplaneAnimation1->setStartValue(QRect(-100, 300, 100, 50));  // 飞机起始位置（左侧）
    airplaneAnimation1->setEndValue(QRect(centralWidget->width(), 300, 100, 50));  // 飞机结束位置（右侧）

    airplaneAnimation1->start();  // 启动飞机动画

    //从右向左的飞机
    QLabel *airplaneLabel2 = new QLabel(this);
    airplaneLabel2->setPixmap(QPixmap(":/plane_left.png").scaled(100, 50, Qt::KeepAspectRatio));  // 设置飞机图片
    airplaneLabel2->setGeometry(centralWidget->width(), 150, 100, 50);  // 初始位置在屏幕外面，靠右

    QPropertyAnimation *airplaneAnimation2 = new QPropertyAnimation(airplaneLabel2, "geometry");
    airplaneAnimation2->setLoopCount(-1);  // 无限循环
    airplaneAnimation2->setDuration(6000);  // 动画持续时间，单位为毫秒
    airplaneAnimation2->setEasingCurve(QEasingCurve::Linear);  // 线性过渡
    airplaneAnimation2->setStartValue(QRect(centralWidget->width(),150, 100, 50));  // 飞机起始位置（右侧）
    airplaneAnimation2->setEndValue(QRect(-100, 150, 100, 50));  // 飞机结束位置（左侧）

    airplaneAnimation2->start();  // 启动飞机动画

    // 创建内部布局（原来的布局逻辑）
    QVBoxLayout *innerLayout = new QVBoxLayout();

    // 登录标题
    QLabel *titleLabel = new QLabel("登 录", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; text-align: center;");
    titleLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    innerLayout->addWidget(titleLabel);

    //添加空白
    QSpacerItem *spacer1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    innerLayout->addItem(spacer1);

    // 用户名输入框
    usernameField = new AnimatedInputField("邮箱", false, this);
    usernameField->setMinimumHeight(60);
    usernameField->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    innerLayout->addWidget(usernameField);

    //添加空白
    QSpacerItem *spacer2 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);
    innerLayout->addItem(spacer2);

    // 密码输入框
    passwordField = new AnimatedInputField("密码", true, this);
    passwordField->setMinimumHeight(60);
    passwordField->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    innerLayout->addWidget(passwordField);

    //添加空白
    QSpacerItem *spacer3 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    innerLayout->addItem(spacer3);

    // 忘记密码按钮
    QPushButton *forgotPasswordBtn = new QPushButton("忘记密码?", this);
    forgotPasswordBtn->setStyleSheet(
        "background: transparent; border: none; color: black; font-size: 14px; text-align: right;"
        );
    forgotPasswordBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    innerLayout->addWidget(forgotPasswordBtn);

    //添加空白
    QSpacerItem *spacer4 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);
    innerLayout->addItem(spacer4);

    // 登录按钮
    QPushButton *loginBtn = new QPushButton("登录", this);
    loginBtn->setStyleSheet(
        "QPushButton {"
        "    background: white;"
        "    color: black;"
        "    border-radius: 20px;"
        "    font-size: 16px;"
        "    padding: 10px;"
        "    font-weight: bold;"
        "    border: 2px solid #1d7bff;"  // 添加边框颜色
        "    box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);"  // 添加初始阴影
        "    transition: all 0.3s ease;"  // 平滑过渡
        "}"
        "QPushButton:hover {"
        "    background-color: rgb(29, 123, 255);"  // 悬浮时的背景色
        "    color: white;"  // 悬浮时字体颜色变白
        "    box-shadow: 0 8px 12px rgba(0, 0, 0, 0.2);"  // 悬浮时的阴影效果
        "    transform: scale(1.1);"  // 悬浮时按钮略微放大
        "}"
        "QPushButton:pressed {"
        "    background-color: rgb(29, 123, 255);"  // 点击时的背景色
        "    box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);"  // 点击时的阴影效果
        "    transform: scale(0.95);"  // 点击时按钮略微缩小
        "}"
        );
    loginBtn->setCursor(Qt::PointingHandCursor);
    loginBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    connect(loginBtn,&QPushButton::clicked,this,&newLoginWindow::onLogInButtonClicked);
    innerLayout->addWidget(loginBtn);

    //添加空白
    QSpacerItem *spacer5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    innerLayout->addItem(spacer5);

    // 注册提示
    QPushButton *signupBtn = new QPushButton("没有账号? 去注册.", this);
    signupBtn->setStyleSheet(
        "QPushButton {"
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
        "    padding: 2px;" /* 模拟按钮按下的轻微位移 */
        "}"
        );
    signupBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    innerLayout->addWidget(signupBtn);
    connect(signupBtn,&QPushButton::clicked,this,&newLoginWindow::onSignUpButtonClicked);

    // 将内部布局添加到外部容器
    outerContainer->setLayout(innerLayout);

    // 设置主布局，使外部容器居中
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding)); // 顶部间距
    mainLayout->addWidget(outerContainer, 0, Qt::AlignCenter);
    mainLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding)); // 底部间距

    // 将主布局设置为中央部件的布局
    centralWidget->setLayout(mainLayout);

    // 连接信号与槽
    connect(forgotPasswordBtn, &QPushButton::clicked, this, [this]() {
        QMessageBox::information(this, "Forgot Password", "Please email support@example.com to request a password reset.");
    });

    // 窗口配置
    setWindowTitle("云程");
    resize(1300, 700); // 设置窗口初始大小
}

void newLoginWindow::onSignUpButtonClicked()
{
    InterfaceManager::instance()->switchToPage("lxt_registerWindow");
}

void newLoginWindow::onLogInButtonClicked() //点击登录按钮触发事件
{
    InterfaceManager::instance()->switchToPage("lxt_mainInterface");
    QString usrname = usernameField->text();
    QString password = passwordField->text();
    emit loginRequested(usrname,password);
}

newLoginWindow::~newLoginWindow()
{
    delete ui;
}

void loginHandler::handleLogin(const QString& username, const QString& password)
{
    qDebug()<<"username:"<<username;
    qDebug()<<"pwd:"<<password;

    // // 创建网络管理器
    // QNetworkAccessManager* manager = new QNetworkAccessManager();

    // // 设置请求 URL
    // QUrl url("http://localhost:8080/api/users/login"); // 替换为你的 API 地址
    // QNetworkRequest request(url);

    // // 设置请求头
    // request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // // 构建 JSON 请求体
    // QJsonObject json;
    // json["username"] = username;
    // json["password"] = password;
    // QJsonDocument jsonDoc(json);
    // QByteArray requestData = jsonDoc.toJson();

    // // 发送 POST 请求
    // QNetworkReply* reply = manager->post(request, requestData);

    // // 连接信号，等待响应
    // connect(reply, &QNetworkReply::finished, [reply]() {
    //     if (reply->error() == QNetworkReply::NoError) {
    //         // 请求成功，读取响应数据
    //         QByteArray responseData = reply->readAll();
    //         QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    //         QJsonObject responseObject = jsonResponse.object();

    //         // 解析响应 JSON
    //         QString message = responseObject["message"].toString();
    //         QString token = responseObject["token"].toString();

    //         qDebug() << "Login Successful: " << message;
    //         qDebug() << "Token: " << token;
    //     } else {
    //         // 请求失败
    //         qDebug() << "Error:" << reply->errorString();
    //     }
    //     reply->deleteLater(); // 释放资源
    // });

    // 清理管理器（这里可以根据实际需求决定是否释放）
}
