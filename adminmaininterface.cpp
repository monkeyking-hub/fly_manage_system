#include "adminmaininterface.h"
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QToolBar>
#include "adminaddflightwindow.h"
#include "admindeleteflightwindow.h"
#include "adminhomewindow.h"
#include "adminupdateflightwindow.h"
#include "chatwindow.h"
#include "interfacemanager.h"
#include "mfindorderwindow.h"
#include "ui_adminmaininterface.h"
#include <mreorder.h>

adminMainInterface::adminMainInterface(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::adminMainInterface)
{
    ui->setupUi(this);

    // 设置主窗口
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setStyleSheet("background-color:white");
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    //左侧菜单栏
    menuTree = new QTreeWidget(this);
    menuTree->setStyleSheet("QTreeWidget {"
                            "    background-color: #f5f5f5;"
                            "    border: none;"
                            "    font: 14px 'Arial';"
                            "    outline: 0;"
                            "}"
                            "QTreeWidget::item {"
                            "    padding: 10px 15px;"
                            "    color: #333;"
                            "    background-color: transparent;"
                            "    border-left: 4px solid transparent;"
                            "    transition: all 0.3s;"
                            "}"
                            "QTreeWidget::item:hover {"
                            "    background-color: #e6f7ff;"
                            "    color: #1890ff;"
                            "    border-left: 4px solid #1890ff;"
                            "}"
                            "QTreeWidget::item:selected {"
                            "    background-color: #d9f7be;"
                            "    color: #52c41a;"
                            "    border-left: 4px solid #52c41a;"
                            "}"
                            "QTreeWidget::item:pressed {"
                            "    background-color: #bae7ff;"
                            "    color: #096dd9;"
                            "    border-left: 4px solid #096dd9;"
                            "}");
    menuTree->setHeaderHidden(true); // 隐藏默认的标题

    // 一级菜单项： 首页, 用户管理, 订单管理, 航班管理
    homeItem = new QTreeWidgetItem(menuTree, QStringList() << "首页");
    kefuItem = new QTreeWidgetItem(menuTree, QStringList() << "客服中心");
    usersItem = new QTreeWidgetItem(menuTree, QStringList() << "用户管理");
    ordersItem = new QTreeWidgetItem(menuTree, QStringList() << "订单管理");
    flightsItem = new QTreeWidgetItem(menuTree, QStringList() << "航班管理");

    // 二级菜单项： 用户增删查改, 订单增删查改, 航班增删查改
    userSearchItem = new QTreeWidgetItem(usersItem, QStringList() << "查询用户信息");
    userRemoveItem = new QTreeWidgetItem(usersItem, QStringList() << "删除用户信息");
    userUpdateItem = new QTreeWidgetItem(usersItem, QStringList() << "修改用户信息");
    orderSearchItem = new QTreeWidgetItem(ordersItem, QStringList() << "查找订单信息");
    orderUpdateItem = new QTreeWidgetItem(ordersItem, QStringList() << "修改订单信息");
    flightRemoveItem = new QTreeWidgetItem(flightsItem, QStringList() << "删除航班信息");
    flightUpdateItem = new QTreeWidgetItem(flightsItem, QStringList() << "修改航班信息");
    flightAddItem = new QTreeWidgetItem(flightsItem, QStringList() << "添加航班信息");

    connect(menuTree, &QTreeWidget::itemClicked, this, &adminMainInterface::onItemClicked);

    // 设置菜单宽度
    menuTree->setFixedWidth(200);

    // 默认选中首页
    menuTree->setCurrentItem(homeItem);

    // 将左侧菜单栏添加到主布局
    mainLayout->addWidget(menuTree);

    //右侧内容区
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(new adminHomeWindow());
    stackedWidget->addWidget(new QLabel("用户管理"));
    stackedWidget->addWidget(new QLabel("航班管理"));
    stackedWidget->addWidget(new QLabel("订单管理"));

    stackedWidget->addWidget(new QLabel("查询用户信息"));
    stackedWidget->addWidget(new QLabel("删除用户信息"));
    stackedWidget->addWidget(new QLabel("修改用户信息"));

    stackedWidget->addWidget(new MFindOrderWindow);
    stackedWidget->addWidget(new MReorderWindow);

    stackedWidget->addWidget(new adminDeleteFlightWindow());
    stackedWidget->addWidget(new adminUpdateFlightWindow());

    adminAddFlightWindow *adminAddflight = new adminAddFlightWindow();
    addFlightHandler *addFlightHand = new addFlightHandler();
    stackedWidget->addWidget(adminAddflight); //添加航班界面
    connect(adminAddflight,
            &adminAddFlightWindow::addFlightRequested,
            addFlightHand,
            &addFlightHandler::handleAddFlight);

    ChatWindow *chat = new ChatWindow(false);
    stackedWidget->addWidget(chat);

    //将右侧内容添加到主布局mainLayout
    mainLayout->addWidget(stackedWidget);

    //设置中央部件
    setCentralWidget(centralWidget);

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
    )");

    addToolBar(Qt::TopToolBarArea, toolBar); //将工具栏添加到顶部

    //在工具栏添加label，显示软件logo图片
    QLabel *label_logo = new QLabel(this);
    label_logo->setFixedSize(100, 100);
    label_logo->setStyleSheet("background-color: transparent;");
    QPixmap *pix = new QPixmap(":/logo.png");
    QSize sz = label_logo->size();
    label_logo->setPixmap(pix->scaled(sz));
    toolBar->addWidget(label_logo);

    // 添加隐藏/显示按钮
    QPushButton *toggleButton = new QPushButton(this);
    toggleButton->setCheckable(true); // 设置为可切换按钮
    toggleButton->setChecked(true);   // 初始为选中状态

    // 设置按钮初始图标
    QIcon visibleIcon(":/menu_visible.png");  // 菜单栏显示时的图标
    QIcon hiddenIcon(":/menu_hidden.png");    // 菜单栏隐藏时的图标
    toggleButton->setIcon(visibleIcon);       // 初始显示状态为菜单栏可见
    toggleButton->setIconSize(QSize(24, 24)); // 设置图标大小

    toolBar->addWidget(toggleButton); // 将按钮添加到工具栏

    // 连接按钮的点击信号到切换逻辑
    connect(toggleButton, &QPushButton::clicked, [this, toggleButton, visibleIcon, hiddenIcon]() {
        static bool isMenuVisible = menuTree->isVisible(); // 将 isMenuVisible 定义为静态局部变量
        isMenuVisible = !isMenuVisible;                    // 切换状态
        qDebug() << "Menu visibility toggled to:" << isMenuVisible;      // 调试输出
        menuTree->setVisible(isMenuVisible);                             // 设置菜单栏可见性
        toggleButton->setIcon(isMenuVisible ? visibleIcon : hiddenIcon); // 根据状态切换图标
    });

    //在工具栏里添加label，显示软件名字
    QLabel *label_name = new QLabel("云程 管理员界面", this);
    label_name->setFixedSize(350, 100);
    label_name->setStyleSheet(
        "font-size: 40px; color: rgb(52, 127, 196); font-family: '千图笔锋手写体';");
    toolBar->addWidget(label_name);

    //在工具栏添加标语
    QLabel *slogan = new QLabel("软件定义世界，工程铸就未来", this);
    slogan->setFixedSize(630, 100);
    slogan->setStyleSheet("font-size: 45px; color: green; font-family: '千图笔锋手写体';");
    toolBar->addWidget(slogan);

    //在工具栏里添加返回用户登录界面按钮
    QPushButton *returnButton = new QPushButton("返回用户登录", this);
    returnButton->setStyleSheet("QPushButton {"
                                "    background: white;"
                                "    color: black;"
                                "    border-radius: 20px;"
                                "    font-size: 10px;"
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
    connect(returnButton, &QPushButton::clicked, this, &adminMainInterface::onReturnButtonClicked);
    toolBar->addWidget(returnButton);
}

void adminMainInterface::onItemClicked(QTreeWidgetItem *item, int column)
{
    // 如果当前点击的节点有子节点，切换其展开/折叠状态
    if (item->childCount() > 0) {
        bool expanded = item->isExpanded();
        item->setExpanded(!expanded); // 切换展开状态
    }

    // 判断点击的节点，并切换 QStackedWidget 的页面
    if (item == menuTree->topLevelItem(0)) // "首页"
    {
        stackedWidget->setCurrentIndex(0);
    } else if (item == menuTree->topLevelItem(1)) //客服中心
    {
        stackedWidget->setCurrentIndex(12);
    } else if (item == menuTree->topLevelItem(2)) // "用户管理"
    {
        stackedWidget->setCurrentIndex(1);
    } else if (item == menuTree->topLevelItem(3)) // "订单管理"
    {
        stackedWidget->setCurrentIndex(2);
    } else if (item == menuTree->topLevelItem(4)) // "航班管理"
    {
        stackedWidget->setCurrentIndex(3);
    } else if (item == usersItem->child(0)) //"查询用户信息"
    {
        stackedWidget->setCurrentIndex(4);
    } else if (item == usersItem->child(1)) //"删除用户信息"
    {
        stackedWidget->setCurrentIndex(5);
    } else if (item == usersItem->child(2)) //"修改用户信息"
    {
        stackedWidget->setCurrentIndex(6);
    } else if (item == usersItem->child(0)) //"查询用户信息"
    {
        stackedWidget->setCurrentIndex(6);
    } else if (item == ordersItem->child(0)) {
        stackedWidget->setCurrentIndex(7);
    } else if (item == ordersItem->child(1)) {
        stackedWidget->setCurrentIndex(8);
    } else if (item == flightsItem->child(0)) {
        stackedWidget->setCurrentIndex(9);
    } else if (item == flightsItem->child(1)) {
        stackedWidget->setCurrentIndex(10);
    } else if (item == flightsItem->child(2)) {
        stackedWidget->setCurrentIndex(11);
    }
}

void adminMainInterface::onReturnButtonClicked()
{
    InterfaceManager::instance()->switchToPage("lxt_newLoginWindow");
    stackedWidget->setCurrentIndex(0);
    //选中首页
    menuTree->setCurrentItem(homeItem);
}

adminMainInterface::~adminMainInterface()
{
    delete ui;
}
