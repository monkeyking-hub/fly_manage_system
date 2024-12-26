#include "homewindow.h"
#include <QApplication>
#include <QDebug>
#include <QFrame>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
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
    titleLabel->setFixedSize(this->width() - 100, 150);
    QPixmap pixmap(":/plane.png");
    QPixmap scaledPixMap = pixmap.scaled(titleLabel->size(),
                                         Qt::KeepAspectRatio,
                                         Qt::SmoothTransformation);
    titleLabel->setPixmap(scaledPixMap);
    mainLayout->addWidget(titleLabel);

    QHBoxLayout *searchLayout = new QHBoxLayout();

    // 设置搜索框部分的控件
    QLabel *searchLabel = new QLabel("低价速报", this);
    searchLabel->setFixedSize(100, 50);
    searchLabel->setStyleSheet("font: bold 20px; color: black;");

    QLabel *tipLabel = new QLabel("出发地：", this);
    tipLabel->setFixedHeight(50);                                    // 固定高度
    tipLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed); // 固定大小策略
    tipLabel->setStyleSheet("margin: 0px; padding: 0px;");

    //输入框
    departureInput = new QLineEdit(this);
    departureInput->setPlaceholderText("请输入您的出发城市");
    departureInput->setFixedSize(150, 50);                                 // 固定尺寸
    departureInput->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed); // 固定大小策略
    departureInput->setStyleSheet("margin: 0px; padding: 0px;");

    //搜索按钮
    QPushButton *searchButton = new QPushButton(this);
    connect(searchButton, &QPushButton::clicked, this, &homeWindow::onSearchButtonClicked);
    searchButton->setText("搜索");
    searchButton->setFixedSize(50, 30);                                  // 固定尺寸
    searchButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed); // 固定大小策略
    searchButton->setStyleSheet("QPushButton {"
                                "    background: white;"
                                "    color: black;"
                                "    border-radius: 20px;"
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

    // 调整布局：去掉控件间距和边距
    searchLayout->setSpacing(0);                  // 控件之间无间距
    searchLayout->setContentsMargins(0, 0, 0, 0); // 布局边缘无间距
    searchLayout->setAlignment(Qt::AlignLeft);    // 控件左对齐

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
                                              {"广州→宁波", "广州→杭州", "广州->重庆", "广州->贵阳"},
                                              {"¥460起", "¥480起", "¥2000起", "¥1000起"},
                                              this));
    scrollLayout->addWidget(
        new CategoryBlock("爱上大草原",
                          {"广州→鄂尔多斯", "广州→呼和浩特", "广州->乌鲁木齐", "广州->通辽"},
                          {"¥300起", "¥300起", "¥1800起", "¥1100起"},
                          this));
    scrollLayout->addWidget(new CategoryBlock("海边浪一浪",
                                              {"广州→北海", "广州→厦门", "广州->青岛", "广州->舟山"},
                                              {"¥267起", "¥289起", "¥1000起", "¥1000起"},
                                              this));

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

void homeWindow::onSearchButtonClicked()
{
    QString departure = departureInput->text();
    if (departure.isEmpty()) {
        QMessageBox::critical(nullptr, "错误", "请先输入出发城市！");
        return;
    }

    clearCategoryBlocks();

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    // 设置请求 URL
    QUrl url("http://localhost:8080/api/flights/search");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QString array[3][4] = {{"宁波", "杭州", "重庆", "贵阳"},
                           {"鄂尔多斯", "呼和浩特", "乌鲁木齐", "通辽"},
                           {"北海", "厦门", "青岛", "舟山"}};

    bool hasFlights = false; // 记录是否找到航班
    for (int i = 0; i < 3; i++) {
        int prices[4] = {0, 0, 0, 0}; // 每个分类卡片中的四条航班价格

        for (int j = 0; j < 4; j++) {
            QJsonObject json;
            json["departure"] = departure;
            json["destination"] = array[i][j];

            // 创建事件循环用于等待异步请求完成
            QEventLoop loop;
            QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());

            connect(reply, &QNetworkReply::finished, [&]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QByteArray responseData = reply->readAll();
                    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
                    QJsonObject responseObject = jsonResponse.object();

                    if (responseObject["code"].toInt() == 200) {
                        QJsonArray flightsArray = responseObject["data"].toArray();
                        if (!flightsArray.isEmpty()) {
                            double minPrice = std::numeric_limits<int>::max();
                            // 遍历航班数组
                            for (const QJsonValue &value : flightsArray) {
                                QJsonObject flight = value.toObject();
                                int price = flight["economyClassPrice"].toInt();
                                if (price < minPrice)
                                    minPrice = price;
                            }
                            prices[j] = minPrice;
                            hasFlights = true;
                        }
                    } else {
                        hasFlights = false;
                    }
                } else {
                    qDebug() << "Error searching flights:" << reply->errorString();
                    QMessageBox::critical(nullptr, "请求失败", "请求失败: " + reply->errorString());
                }
                reply->deleteLater();
                loop.quit(); // 退出事件循环
            });

            loop.exec(); // 等待请求完成
        }

        // 如果没有找到航班，跳过当前分类卡片
        if (!hasFlights) {
            clearCategoryBlocks();
            break;
        }

        // 创建分类卡片
        QString categoryTitle;
        if (i == 0) {
            categoryTitle = "周末省心游";
        } else if (i == 1) {
            categoryTitle = "爱上大草原";
        } else {
            categoryTitle = "海边浪一浪";
        }

        QStringList destinations = {departure + "->" + array[i][0],
                                    departure + "->" + array[i][1],
                                    departure + "->" + array[i][2],
                                    departure + "->" + array[i][3]};
        QStringList priceStrings = {"¥" + QString::number(prices[0]) + "起",
                                    "¥" + QString::number(prices[1]) + "起",
                                    "¥" + QString::number(prices[2]) + "起",
                                    "¥" + QString::number(prices[3]) + "起"};

        scrollLayout->addWidget(new CategoryBlock(categoryTitle, destinations, priceStrings, this));
    }

    if (!hasFlights) {
        QMessageBox::information(nullptr, "提示", "未找到符合条件的航班，请更换出发城市！");
    }
}

homeWindow::~homeWindow()
{
    delete ui;
}
