#include "maininterface.h"
#include "ui_maininterface.h"
#include "interfacemanager.h"
#include "flightstatus.h"
#include "homewindow.h"
#include "orderwindow.h"
#include "newhomewindow.h"
#include "user.h"
#include "usermanager.h"
#include <QStackedWidget>
#include <QDebug>
#include <QApplication>
#include <QMainWindow>
#include <QListWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QComboBox>
#include <QLineEdit>
#include <QToolButton>

#include <QTreeWidget>           // 树形控件 QTreeWidget
#include <QTreeWidgetItem>       // 树形控件的项 QTreeWidgetItem
#include <QListWidgetItem>       // （可选）如果使用 QListWidgetItem
#include <QStringList>           // 字符串列表，用于树节点名称

maininterface::maininterface(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::maininterface)
{
    ui->setupUi(this);

    //设置主窗口
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setStyleSheet("background-color:white");
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    //左侧菜单栏
    QListWidget *menuList = new QListWidget(this);
    menuList->setStyleSheet("background-color:white");
    menuList->addItem("首页");
    menuList->addItem("航班动态");
    menuList->addItem("我的订单");
    menuList->addItem("特惠机票");
    menuList->setFixedWidth(150);
    menuList->setCurrentRow(0); //默认在首页窗口

    //自定义菜单栏样式
    menuList->setStyleSheet(
        "QListWidget {"
        "    background-color: #f5f5f5;"
        "    border: none;"
        "    font: 14px 'Arial';"
        "    outline: 0;"
        "}"
        "QListWidget::item {"
        "    padding: 10px 15px;"
        "    color: #333;"
        "    background-color: transparent;"
        "    border-left: 4px solid transparent;"
        "    transition: all 0.3s;"
        "}"
        "QListWidget::item:hover {"
        "    background-color: #e6f7ff;"
        "    color: #1890ff;"
        "    border-left: 4px solid #1890ff;"
        "}"
        "QListWidget::item:selected {"
        "    background-color: #d9f7be;"
        "    color: #52c41a;"
        "    border-left: 4px solid #52c41a;"
        "}"
        "QListWidget::item:pressed {"
        "    background-color: #bae7ff;"
        "    color: #096dd9;"
        "    border-left: 4px solid #096dd9;"
        "}"
    );

    //将左侧菜单栏添加到主布局mainLayout
    mainLayout->addWidget(menuList);

    //右侧内容区
    QStackedWidget *stackedWidget = new QStackedWidget(this);
    newHomeWindow *w1 = new newHomeWindow();
    stackedWidget->addWidget(w1); //首页界面
    stackedWidget->addWidget(new flightstatus()); //航班动态界面
    stackedWidget->addWidget(new orderwindow()); //订单界面
    stackedWidget->addWidget(new homeWindow()); //特惠机票界面

    //将右侧内容添加到主布局mainLayout
    mainLayout->addWidget(stackedWidget);

    //设置中央部件
    setCentralWidget(centralWidget);

    //信号槽：菜单项切换页面
    connect(menuList,&QListWidget::currentRowChanged,stackedWidget,&QStackedWidget::setCurrentIndex);

    // 连接 newHomeWindow 信号到槽
    connect(w1, &newHomeWindow::commandLinkButton4Clicked, this, [stackedWidget,menuList]() {
        // 切换到航班动态界面
        stackedWidget->setCurrentIndex(1);
        menuList->setCurrentRow(1);
    });

    connect(w1, &newHomeWindow::commandLinkButton5Clicked, this, [stackedWidget,menuList]() {
        // 切换到订单界面
        stackedWidget->setCurrentIndex(2);
        menuList->setCurrentRow(2);
    });

    connect(w1, &newHomeWindow::commandLinkButton6Clicked, this, [stackedWidget,menuList]() {
        // 切换到特惠机票界面
        stackedWidget->setCurrentIndex(3);
        menuList->setCurrentRow(3);
    });

    //添加上方工具栏
    QToolBar *toolBar = new QToolBar(this);
    toolBar->setIconSize(QSize(75, 75));
    toolBar->setStyleSheet(R"(
    /* 工具栏整体样式 */
    QToolBar {
        background-color: white;       /* 白色背景 */
        border: none;                   /* 去除边框 */
        padding: 2px;                   /* 工具栏内边距 */
        spacing: 8px;                   /* 按钮和控件之间的间距 */
    }

    /* 工具栏按钮样式 */
    QToolButton {
        background-color: transparent;  /* 默认透明背景 */
        border: none;                   /* 去除边框 */
        padding: 4px 6px;               /* 按钮内边距 */
        font: 14px "Arial";             /* 设置字体 */
        color: #333333;                 /* 字体颜色 */
    }

    /* 鼠标悬停效果 */
    QToolButton:hover {
        background-color: #e0e0e0;      /* 悬停背景浅灰 */
        border-radius: 6px;             /* 按钮圆角效果 */
    }

    /* 鼠标按下效果 */
    QToolButton:pressed {
        background-color: #cccccc;      /* 按下背景更深的灰色 */
        border: 1px solid #aaaaaa;      /* 添加浅灰边框 */
    }

    /* 工具栏分隔符样式 */
    QToolBar::separator {
        background-color: #d0d0d0;      /* 分隔符浅灰 */
        width: 1px;                     /* 分隔线宽度 */
        margin: 4px;                    /* 上下间距 */
    }
    )");

    addToolBar(Qt::TopToolBarArea,toolBar); //将工具栏添加到顶部

    //在工具栏添加label，显示软件logo图片
    QLabel *label_logo = new QLabel(this);
    label_logo->setFixedSize(100,100);
    label_logo->setStyleSheet("background-color: transparent;");
    QPixmap *pix = new QPixmap(":/logo.png");
    QSize sz=label_logo->size();
    label_logo->setPixmap(pix->scaled(sz));
    toolBar->addWidget(label_logo);

    //在工具栏里添加label，显示软件名字
    QLabel *label_name = new QLabel("云程",this);
    label_name->setFixedSize(100,100);
    label_name->setStyleSheet(
        "font-size: 40px; color: rgb(52, 127, 196); font-family: '千图笔锋手写体';"
    );
    toolBar->addWidget(label_name);


    QWidget *spacer = new QWidget(this); //添加弹性空间
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    toolBar->addWidget(spacer);
    // 在工具栏添加用户名label
    QLabel *usrNameLabel = new QLabel(this);
    usrNameLabel->setFixedSize(100, 50);
    usrNameLabel->setStyleSheet(
        "font-size: 20px; color: black; font-family: '千图笔锋手写体';");
    toolBar->addWidget(usrNameLabel);

    // 获取当前用户信息并显示昵称
    User currentUser = UserManager::getInstance()->getCurrentUser();
    usrNameLabel->setText(currentUser.username.isEmpty() ? "未登录" : currentUser.username);

    // 监听用户信息更新信号，动态刷新界面上的昵称
    connect(UserManager::getInstance(), &UserManager::currentUserChanged, this, [usrNameLabel](const User &user) {
        usrNameLabel->setText(user.username.isEmpty() ? "未登录" : user.username);
    });


    //添加用户头像按钮
    QToolButton *usrButton = new QToolButton(this);
    usrButton->setIcon(QIcon(":/profilePhoto.png"));
    usrButton->setIconSize(QSize(100,100));
    usrButton->setPopupMode(QToolButton::InstantPopup);
    //创建用户头像下拉菜单
    QMenu *profileMenu = new QMenu(usrButton);
    //设置菜单效果
    profileMenu->setStyleSheet(R"(
    /* 菜单背景和整体样式 */
    QMenu {
        background-color: #ffffff;        /* 背景颜色 */
        border: 1px solid #cccccc;       /* 边框颜色 */
        font-size: 14px;                 /* 字体大小 */
        color: #333333;                  /* 字体颜色 */
        padding: 4px;                    /* 内边距 */
    }

    /* 菜单项默认样式 */
    QMenu::item {
        background-color: transparent;   /* 默认背景透明 */
        padding: 6px 24px;               /* 项目内边距 */
        color: #333333;                  /* 默认字体颜色 */
    }

    /* 鼠标悬停效果 */
    QMenu::item:selected {
        background-color: #f0f0f0;       /* 鼠标悬停时背景颜色 */
        color: #000000;                  /* 鼠标悬停时字体颜色 */
    }

    /* 鼠标按下效果 */
    QMenu::item:pressed {
        background-color: #dcdcdc;       /* 鼠标按下时背景颜色 */
        color: #000000;                  /* 鼠标按下时字体颜色 */
    }
    )");
    QAction *editInfoAction = new QAction("个人信息编辑",this);
    QAction *switchLoginAction = new QAction("切换登录",this);
    QAction *switchFlightStatusWindowAction = new QAction("航班动态",this);
    // 设置图标
    editInfoAction->setIcon(QIcon(":/usr.png"));
    switchLoginAction->setIcon(QIcon(":/switchLogin.png"));
    //将选项添加到菜单
    profileMenu->addAction(editInfoAction);
    profileMenu->addAction(switchLoginAction);
    //将菜单设置为头像按钮的下拉菜单
    usrButton->setMenu(profileMenu);
    //将头像按钮添加到工具栏
    toolBar->addWidget(usrButton);
    //信号槽：连接菜单项
    connect(editInfoAction,&QAction::triggered,this,&maininterface::editUserInfo);
    connect(switchLoginAction,&QAction::triggered,this,&maininterface::switchLogin);
    connect(switchFlightStatusWindowAction,&QAction::triggered,this,&maininterface::switchtoFlightStatusWindow);
}

void maininterface::editUserInfo()
{
    InterfaceManager::instance()->switchToPage("fzj_window");
}

void maininterface::switchLogin()
{
    InterfaceManager::instance()->switchToPage("lxt_newLoginWindow");
}

void maininterface::switchtoFlightStatusWindow()
{
    InterfaceManager::instance()->switchToPage("fzj_flightstatus");
}

maininterface::~maininterface()
{
    delete ui;
}
