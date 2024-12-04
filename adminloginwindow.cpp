#include "adminloginwindow.h"
#include "ui_adminloginwindow.h"

#include "interfacemanager.h"
#include "usermanager.h"
#include "userwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

adminLoginWindow::adminLoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::adminLoginWindow)
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

    // 创建内部布局
    QVBoxLayout *innerLayout = new QVBoxLayout();

    // 登录标题
    QLabel *titleLabel = new QLabel("管理员登录", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; text-align: center;");
    titleLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    innerLayout->addWidget(titleLabel);

    //添加空白
    QSpacerItem *spacer1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    innerLayout->addItem(spacer1);

    // 管理员账号输入框
    usrnameField = new AnimatedInputField("管理员账号", false, this);
    usrnameField->setMinimumHeight(60);
    usrnameField->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    innerLayout->addWidget(usrnameField);

    //添加空白
    QSpacerItem *spacer2 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);
    innerLayout->addItem(spacer2);

    // 密码输入框
    passwordField = new AnimatedInputField("密码", true, this);
    passwordField->setMinimumHeight(60);
    passwordField->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    innerLayout->addWidget(passwordField);

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
    connect(loginBtn,&QPushButton::clicked,this,&adminLoginWindow::onAdminLogInButtonClicked);
    innerLayout->addWidget(loginBtn);

    // 将内部布局添加到外部容器
    outerContainer->setLayout(innerLayout);

    // 设置主布局，使外部容器居中
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding)); // 顶部间距
    mainLayout->addWidget(outerContainer, 0, Qt::AlignCenter);
    mainLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding)); // 底部间距

    // 将主布局设置为中央部件的布局
    centralWidget->setLayout(mainLayout);

    // 窗口配置
    setWindowTitle("云程 登录");
    resize(1300, 700); // 设置窗口初始大小
}

void adminLoginWindow::onAdminLogInButtonClicked() //点击登录按钮触发事件
{
    QString usrname = usrnameField->text();
    QString password = passwordField->text();

    //将输入框内容清空
    QLineEdit *usrnameLineEdit = usrnameField->lineEdit();
    QLineEdit *passwordLineEdit = passwordField->lineEdit();
    usrnameLineEdit->setText("");
    passwordLineEdit->setText("");

    emit adminLoginRequested(usrname,password);
}

adminLoginWindow::~adminLoginWindow()
{
    delete ui;
}

void adminLoginHandler::handleAdminLogin(const QString& usrname, const QString& password)
{
    if(usrname=="sysu" && password=="sse")
    {
        InterfaceManager::instance()->switchToPage("lxt_mainInterface");
    }
    else
    {
        // 创建 QMessageBox 来显示错误信息
        QMessageBox::critical(nullptr, "Login Error",
                              "账号或密码输入错误",
                              QMessageBox::Ok, QMessageBox::Ok);
    }
}
