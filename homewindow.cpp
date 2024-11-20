#include "homewindow.h"
#include "ui_homewindow.h"
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>
#include <QScrollArea>

homeWindow::homeWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::homeWindow)
{
    ui->setupUi(this);

    // 设置主控件
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // 创建 QLabel 用于显示标题和副标题
    QLabel *titleLabel = new QLabel(this);
    titleLabel->setFixedSize(800, 150); // 固定 QLabel 尺寸

    // 设置 QLabel 的大小策略，使其能占满父控件的空间
    titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // 使用 HTML 设置主标题和副标题
    titleLabel->setText(
        "<html>"
        "<head/>"
        "<body>"
        "<div style='text-align:center;'>"
        "<p style='font-size:32px; font-weight:bold; color:rgba(255, 255, 234,0.9); margin:10px 0;'>机票6重服务保障</p>"
        "<p style='font-size:16px; color:rgba(255, 255, 234,0.9); margin:0;'>保障新升级，价格放心，出行安心</p>"
        "</div>"
        "</body>"
        "</html>"
        );

    // 设置 QLabel 样式
    titleLabel->setStyleSheet(
        "background-image: url(:/plane.png);"  // 设置背景图片
        "background-position: center;"          // 背景图片居中
        "background-repeat: no-repeat;"         // 不重复背景图片
        "background-size: cover;"               // 背景图片自适应填充
        "color: white;"                         // 设置文本颜色
        "padding: 0;"                           // 移除所有内边距
        );

    // 设置对齐方式
    titleLabel->setAlignment(Qt::AlignCenter);

    // 添加到布局
    mainLayout->addWidget(titleLabel);

    // 搜索框部分
    QHBoxLayout *searchLayout = new QHBoxLayout();
    QLabel *searchLabel = new QLabel("低价速报", this);
    searchLabel->setStyleSheet("font: bold 20px; color: black;");

    searchLayout->addWidget(searchLabel);
    searchLayout->addSpacing(20);
    mainLayout->addLayout(searchLayout);

    // 滚动区域 - 分类部分
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    QWidget *scrollWidget = new QWidget(scrollArea);
    QHBoxLayout *scrollLayout = new QHBoxLayout(scrollWidget);

    // 分类卡片
    scrollLayout->addWidget(createCategoryBlock("周末省心游", {"广州→北海", "广州→长沙"}, {"¥460起", "¥480起"}));
    scrollLayout->addWidget(createCategoryBlock("爱上大草原", {"广州→鄂尔多斯", "广州→呼和浩特"}, {"¥300起", "¥300起"}));
    scrollLayout->addWidget(createCategoryBlock("海边浪一浪", {"广州→福州", "广州→宁波"}, {"¥267起", "¥289起"}));

    scrollWidget->setLayout(scrollLayout);
    scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(scrollArea);

    // 设置主控件
    setCentralWidget(centralWidget);
}

QWidget *homeWindow::createCategoryBlock(const QString &title, const QStringList &routes, const QStringList &prices)
{
    // 分类卡片
    QFrame *block = new QFrame(this);
    block->setFixedSize(300, 300);
    block->setStyleSheet("background-color: white; border-radius: 10px; padding: 10px;");

    QVBoxLayout *layout = new QVBoxLayout(block);

    // 分类标题
    QLabel *titleLabel = new QLabel(title, block);
    titleLabel->setStyleSheet("font: bold 18px; color: white; background-color: #78a0f7; padding: 5px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    // 列表内容
    for (int i = 0; i < routes.size(); ++i) {
        QHBoxLayout *itemLayout = new QHBoxLayout();

        // 创建图片标签
        QLabel *imageLabel = new QLabel(block);
        QPixmap imagePixmap(":/place.png");
        imageLabel->setPixmap(imagePixmap.scaled(30, 50, Qt::KeepAspectRatio));
        imageLabel->setStyleSheet("border-radius: 5px;");

        // 路线和价格标签
        QLabel *routeLabel = new QLabel(routes[i], block);
        QLabel *priceLabel = new QLabel(prices[i], block);

        routeLabel->setStyleSheet("font: 14px; color: black;");
        priceLabel->setStyleSheet("font: bold 14px; color: #ff5722;");

        itemLayout->addWidget(imageLabel);
        itemLayout->addWidget(routeLabel);
        itemLayout->addWidget(priceLabel, 1, Qt::AlignRight);
        layout->addLayout(itemLayout);
    }

    return block;
}

homeWindow::~homeWindow()
{
    delete ui;
}
