#include "maininterface.h"
#include "ui_maininterface.h"
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

maininterface::maininterface(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::maininterface)
{
    ui->setupUi(this);

    //设置主窗口
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    //左侧菜单栏
    QListWidget *menuList = new QListWidget(this);
    menuList->addItem("首页");
    menuList->addItem("航班动态");
    menuList->addItem("我的订单");
    menuList->setFixedWidth(150);

    //自定义菜单栏样式
    menuList->setStyleSheet(
        "QListWidget {"
        "    background-color: #f5f5f5;"
        "    border: none;"
        "    font: 14px 'Arial';"
        "}"
        "QListWidget::item {"
        "    padding: 10px;"
        "    color: #333;"
        "}"
        "QListWidget::item:selected {"
        "    background-color: #d9d9d9;"
        "    color: #0078d7;"
        "    border-left: 4px solid #0078d7;"
        "}"
    );

    //右侧内容区
    QStackedWidget *stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(new QLabel("首页窗口",this)); //仅以label为示例，实际上要实现一个首页窗口类
    stackedWidget->addWidget(new QLabel("航班动态窗口",this)); //仅以label为示例，实际上要实现一个航班动态窗口类
    stackedWidget->addWidget(new QLabel("我的订单窗口",this)); //仅以label为示例，实际上要实现一个我的订单窗口类

    //将左侧菜单和右侧内容添加到主布局mainLayout
    mainLayout->addWidget(menuList);
    mainLayout->addWidget(stackedWidget);

    //设置中央部件
    setCentralWidget(centralWidget);

    //信号槽：菜单项切换页面
    connect(menuList,&QListWidget::currentRowChanged,stackedWidget,&QStackedWidget::setCurrentIndex);

    //添加上方工具栏
    QToolBar *toolBar = new QToolBar(this);
    addToolBar(Qt::TopToolBarArea,toolBar); //将工具栏添加到顶部

    //工具栏按钮：示例按钮
    QAction *actionHome = new QAction(QIcon(":/icons/home.png"), "首页", this);
    QAction *actionSettings = new QAction(QIcon(":/icons/settings.png"), "设置", this);
    QAction *actionHelp = new QAction(QIcon(":/icons/help.png"), "帮助", this);

    // 添加按钮到工具栏
    toolBar->addAction(actionHome);
    toolBar->addAction(actionSettings);
    toolBar->addAction(actionHelp);

    // 工具栏样式
    toolBar->setStyleSheet(
        "QToolBar {"
        "    background-color: #ffffff;"
        "    border: 1px solid #ddd;"
        "}"
        "QToolButton {"
        "    background-color: transparent;"
        "    margin: 5px;"
        "    padding: 5px;"
        "    border-radius: 5px;"
        "}"
        "QToolButton:hover {"
        "    background-color: #f5f5f5;"
        "}"
        );

    // 信号槽：工具栏按钮的功能实现
    connect(actionHome, &QAction::triggered, [=]() {
        menuList->setCurrentRow(0); // 切换到“首页”
    });
    connect(actionSettings, &QAction::triggered, [=]() {
        menuList->setCurrentRow(1); // 切换到“航班动态”窗口
    });
    connect(actionHelp, &QAction::triggered, [=]() {
        stackedWidget->setCurrentIndex(2); // 切换到“我的订单”窗口
    });

}

maininterface::~maininterface()
{
    delete ui;
}
