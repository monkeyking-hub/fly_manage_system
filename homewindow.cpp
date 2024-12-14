#include "homewindow.h"
#include <QApplication>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include "ui_homewindow.h"

homeWindow::homeWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::homeWindow)
{
    ui->setupUi(this);

    // 设置主控件
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // 创建 QLabel 用于显示图片
    QLabel *titleLabel = new QLabel(this);
    titleLabel->setFixedSize(this->width()-100,150);
    QPixmap pixmap(":/plane.png");
    QPixmap scaledPixMap=pixmap.scaled(titleLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    titleLabel->setPixmap(scaledPixMap);
    mainLayout->addWidget(titleLabel);

    QHBoxLayout *searchLayout = new QHBoxLayout();

    // 设置搜索框部分的控件
    QLabel *searchLabel = new QLabel("低价速报", this);
    searchLabel->setFixedSize(100, 50);
    searchLabel->setStyleSheet("font: bold 20px; color: black;");

    QLabel *tipLabel = new QLabel("出发地：", this);
    tipLabel->setFixedHeight(50); // 固定高度
    tipLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed); // 固定大小策略
    tipLabel->setStyleSheet("margin: 0px; padding: 0px;");

    //输入框
    QLineEdit *departureInput = new QLineEdit(this);
    departureInput->setPlaceholderText("请输入您的出发城市");
    departureInput->setFixedSize(150,50); // 固定尺寸
    departureInput->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed); // 固定大小策略
    departureInput->setStyleSheet("margin: 0px; padding: 0px;");

    //搜索按钮
    QPushButton *searchButton = new QPushButton(this);
    searchButton->setText("搜索");
    searchButton->setFixedSize(50,30); // 固定尺寸
    searchButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed); // 固定大小策略
    searchButton->setStyleSheet(
        "QPushButton {"
        "    background: white;"
        "    color: black;"
        "    border-radius: 20px;"
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

    // 调整布局：去掉控件间距和边距
    searchLayout->setSpacing(0);                     // 控件之间无间距
    searchLayout->setContentsMargins(0, 0, 0, 0);    // 布局边缘无间距
    searchLayout->setAlignment(Qt::AlignLeft);       // 控件左对齐

    // 添加控件到布局，按顺序排列
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(tipLabel);
    searchLayout->addWidget(departureInput);
    searchLayout->addWidget(searchButton);

    // 添加到主布局
    mainLayout->addLayout(searchLayout);

    // 滚动区域 - 分类部分
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    QWidget *scrollWidget = new QWidget(scrollArea);
    scrollLayout = new QHBoxLayout(scrollWidget);

    // 添加分类卡片
    scrollLayout->addWidget(new CategoryBlock("周末省心游",
                                              {"广州→北海", "广州→长沙","广州->乌鲁木齐"},
                                              {"¥460起", "¥480起","¥2000起"}, this));
    scrollLayout->addWidget(new CategoryBlock("爱上大草原",
                                              {"广州→鄂尔多斯", "广州→呼和浩特","广州->乌鲁木齐"},
                                              {"¥300起", "¥300起","¥1800起"}, this));
    scrollLayout->addWidget(new CategoryBlock("海边浪一浪",
                                              {"广州→福州", "广州→宁波","广州->三亚"},
                                              {"¥267起", "¥289起","¥1000起"}, this));

    scrollWidget->setLayout(scrollLayout);
    scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(scrollArea);

    // 设置主控件
    setCentralWidget(centralWidget);
}

void homeWindow::clearCategoryBlocks()
{
    while (scrollLayout->count() > 0) {
        QLayoutItem *item = scrollLayout->takeAt(0); // 从布局中移除第一个控件
        if (item) {
            QWidget *widget = item->widget();
            if (widget) {
                delete widget; // 销毁控件
            }
            delete item; // 销毁布局项
        }
    }
}

homeWindow::~homeWindow()
{
    delete ui;
}
