#include "ModernFlightSearchWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QDebug>
#include <QApplication>
#include <QScreen>
#include <QScrollBar>

ModernFlightSearchWindow::ModernFlightSearchWindow(const QString &departure, 
                                                   const QString &destination,
                                                   const QDate &departDate,
                                                   QWidget *parent)
    : QMainWindow(parent), m_departure(departure), m_destination(destination),
      m_departDate(departDate), networkManager(new QNetworkAccessManager(this)) {
    setupUI();
    loadSearchResults();
}

ModernFlightSearchWindow::~ModernFlightSearchWindow() {
}

void ModernFlightSearchWindow::setupUI() {
    setWindowTitle("Sky Wings - Flight Search Results");
    resize(1400, 900);
    QRect screen = QApplication::primaryScreen()->geometry();
    move((screen.width() - width()) / 2, (screen.height() - height()) / 2);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 主布局 - 左侧过滤面板 + 右侧结果
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ==================== 左侧过滤面板 ====================
    setupFilterPanel();
    mainLayout->addWidget(filterPanel, 0);

    // ==================== 右侧搜索结果 ====================
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setContentsMargins(30, 30, 30, 30);
    rightLayout->setSpacing(20);

    // 搜索结果头部
    QHBoxLayout *headerLayout = new QHBoxLayout();

    QLabel *resultTitle = new QLabel(m_departure + " → " + m_destination);
    resultTitle->setStyleSheet(
        "QLabel {"
        "   color: #0052CC;"
        "   font-size: 24px;"
        "   font-weight: 700;"
        "}"
    );
    headerLayout->addWidget(resultTitle);

    QLabel *resultDate = new QLabel(m_departDate.toString("dddd, MMMM dd, yyyy"));
    resultDate->setStyleSheet(
        "QLabel {"
        "   color: #6B7280;"
        "   font-size: 14px;"
        "}"
    );
    headerLayout->addWidget(resultDate);

    headerLayout->addStretch();

    // 排序控件
    createSortControls();
    headerLayout->addWidget(sortCombo);

    rightLayout->addLayout(headerLayout);

    // 分隔线
    QFrame *divider = new QFrame();
    divider->setStyleSheet("QFrame { background-color: #E5E7EB; }");
    divider->setFixedHeight(1);
    rightLayout->addWidget(divider);

    // 搜索结果容器
    resultsScroll = new QScrollArea();
    resultsScroll->setWidgetResizable(true);
    resultsScroll->setStyleSheet(
        "QScrollArea {"
        "   border: none;"
        "   background-color: #F9FAFB;"
        "}"
        "QScrollBar:vertical {"
        "   width: 8px;"
        "}"
        "QScrollBar::handle:vertical {"
        "   background-color: #D1D5DB;"
        "   border-radius: 4px;"
        "}"
    );

    resultsContainer = new QFrame();
    QVBoxLayout *containerLayout = new QVBoxLayout(resultsContainer);
    containerLayout->setSpacing(15);
    containerLayout->addStretch();
    resultsScroll->setWidget(resultsContainer);

    rightLayout->addWidget(resultsScroll, 1);

    // 将右侧布局添加到主布局
    QWidget *rightPanel = new QWidget();
    rightPanel->setLayout(rightLayout);
    mainLayout->addWidget(rightPanel, 1);
}

void ModernFlightSearchWindow::setupFilterPanel() {
    filterPanel = new QFrame();
    filterPanel->setStyleSheet(
        "QFrame {"
        "   background-color: white;"
        "   border-right: 1px solid #E5E7EB;"
        "}"
    );
    filterPanel->setFixedWidth(300);

    QVBoxLayout *filterLayout = new QVBoxLayout(filterPanel);
    filterLayout->setContentsMargins(20, 20, 20, 20);
    filterLayout->setSpacing(20);

    // ========== 价格过滤 ==========
    QLabel *priceTitle = new QLabel("Price Range");
    priceTitle->setStyleSheet("QLabel { color: #111827; font-weight: 700; font-size: 15px; }");
    filterLayout->addWidget(priceTitle);

    QHBoxLayout *priceLayout = new QHBoxLayout();
    QLabel *minPrice = new QLabel("$0");
    minPrice->setStyleSheet("QLabel { color: #6B7280; font-size: 12px; }");
    priceLayout->addWidget(minPrice);

    priceSlider = new QSlider(Qt::Horizontal);
    priceSlider->setMinimum(0);
    priceSlider->setMaximum(5000);
    priceSlider->setValue(5000);
    priceSlider->setStyleSheet(
        "QSlider::groove:horizontal {"
        "   background-color: #E5E7EB;"
        "   height: 4px;"
        "   border-radius: 2px;"
        "}"
        "QSlider::handle:horizontal {"
        "   background-color: #0052CC;"
        "   width: 16px;"
        "   margin: -6px 0;"
        "   border-radius: 8px;"
        "}"
    );
    priceLayout->addWidget(priceSlider);

    priceLabel = new QLabel("$5000");
    priceLabel->setStyleSheet("QLabel { color: #0052CC; font-weight: 600; font-size: 12px; }");
    priceLayout->addWidget(priceLabel);

    filterLayout->addLayout(priceLayout);

    connect(priceSlider, QOverload<int>::of(&QSlider::valueChanged),
            this, [this](int value) {
        priceLabel->setText("$" + QString::number(value));
        onFilterChanged();
    });

    // 分隔符
    QFrame *div1 = new QFrame();
    div1->setStyleSheet("QFrame { background-color: #E5E7EB; }");
    div1->setFixedHeight(1);
    filterLayout->addWidget(div1);

    // ========== 航班类型过滤 ==========
    QLabel *typeTitle = new QLabel("Flight Type");
    typeTitle->setStyleSheet("QLabel { color: #111827; font-weight: 700; font-size: 15px; }");
    filterLayout->addWidget(typeTitle);

    directOnly = new QCheckBox("Direct Flights Only");
    directOnly->setStyleSheet("QCheckBox { color: #374151; font-size: 13px; }");
    filterLayout->addWidget(directOnly);
    connect(directOnly, &QCheckBox::toggled, this, &ModernFlightSearchWindow::onFilterChanged);

    oneStopOnly = new QCheckBox("1 Stop Maximum");
    oneStopOnly->setStyleSheet("QCheckBox { color: #374151; font-size: 13px; }");
    filterLayout->addWidget(oneStopOnly);
    connect(oneStopOnly, &QCheckBox::toggled, this, &ModernFlightSearchWindow::onFilterChanged);

    // 分隔符
    QFrame *div2 = new QFrame();
    div2->setStyleSheet("QFrame { background-color: #E5E7EB; }");
    div2->setFixedHeight(1);
    filterLayout->addWidget(div2);

    // ========== 出发时间过滤 ==========
    QLabel *timeTitle = new QLabel("Departure Time");
    timeTitle->setStyleSheet("QLabel { color: #111827; font-weight: 700; font-size: 15px; }");
    filterLayout->addWidget(timeTitle);

    departTimeSlider = new QSlider(Qt::Horizontal);
    departTimeSlider->setMinimum(0);
    departTimeSlider->setMaximum(24);
    departTimeSlider->setStyleSheet(
        "QSlider::groove:horizontal {"
        "   background-color: #E5E7EB;"
        "   height: 4px;"
        "   border-radius: 2px;"
        "}"
        "QSlider::handle:horizontal {"
        "   background-color: #1E88E5;"
        "   width: 16px;"
        "   margin: -6px 0;"
        "   border-radius: 8px;"
        "}"
    );
    filterLayout->addWidget(departTimeSlider);
    connect(departTimeSlider, QOverload<int>::of(&QSlider::valueChanged),
            this, &ModernFlightSearchWindow::onFilterChanged);

    // 分隔符
    QFrame *div3 = new QFrame();
    div3->setStyleSheet("QFrame { background-color: #E5E7EB; }");
    div3->setFixedHeight(1);
    filterLayout->addWidget(div3);

    // ========== 航空公司 ==========
    QLabel *airlineTitle = new QLabel("Airlines");
    airlineTitle->setStyleSheet("QLabel { color: #111827; font-weight: 700; font-size: 15px; }");
    filterLayout->addWidget(airlineTitle);

    QStringList airlines = {"Sky Airways", "Global Airlines", "Premium Airways", "Blue Horizon"};
    for (int i = 0; i < airlines.length() && i < 10; ++i) {
        airlineFilter[i] = new QCheckBox(airlines[i]);
        airlineFilter[i]->setStyleSheet("QCheckBox { color: #374151; font-size: 13px; }");
        airlineFilter[i]->setChecked(true);
        filterLayout->addWidget(airlineFilter[i]);
        connect(airlineFilter[i], &QCheckBox::toggled, this, &ModernFlightSearchWindow::onFilterChanged);
    }

    filterLayout->addStretch();

    // 重置按钮
    QPushButton *resetBtn = new QPushButton("Reset Filters");
    resetBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   color: #0052CC;"
        "   border: 2px solid #0052CC;"
        "   border-radius: 6px;"
        "   padding: 8px;"
        "   font-weight: 600;"
        "   font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #F0F4FF;"
        "}"
    );
    filterLayout->addWidget(resetBtn);

    connect(resetBtn, &QPushButton::clicked, this, [this]() {
        priceSlider->setValue(5000);
        directOnly->setChecked(false);
        oneStopOnly->setChecked(false);
        departTimeSlider->setValue(0);
        for (int i = 0; i < 4; ++i) {
            airlineFilter[i]->setChecked(true);
        }
    });
}

void ModernFlightSearchWindow::createSortControls() {
    sortCombo = new QComboBox();
    sortCombo->addItem("Recommended");
    sortCombo->addItem("Price: Low to High");
    sortCombo->addItem("Price: High to Low");
    sortCombo->addItem("Duration: Short to Long");
    sortCombo->addItem("Departure Time: Early");
    sortCombo->addItem("Departure Time: Late");
    sortCombo->setStyleSheet(
        "QComboBox {"
        "   background-color: white;"
        "   color: #111827;"
        "   border: 1px solid #E5E7EB;"
        "   border-radius: 6px;"
        "   padding: 8px 12px;"
        "   font-size: 13px;"
        "   min-width: 150px;"
        "}"
        "QComboBox:focus {"
        "   border: 2px solid #0052CC;"
        "}"
    );

    connect(sortCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ModernFlightSearchWindow::onSortChanged);
}

void ModernFlightSearchWindow::loadSearchResults() {
    QUrl url("http://localhost:8080/api/flights/search");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject searchParams;
    searchParams["departure"] = m_departure;
    searchParams["destination"] = m_destination;
    searchParams["departureDate"] = m_departDate.toString(Qt::ISODate);

    QJsonDocument doc(searchParams);
    QNetworkReply *reply = networkManager->post(request, doc.toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject responseObject = jsonResponse.object();

            if (responseObject["code"].toInt() == 200) {
                m_allFlights = responseObject["data"].toArray();
                onSearchCompleted(m_allFlights);
            }
        }
        reply->deleteLater();
    });
}

void ModernFlightSearchWindow::onSearchCompleted(const QJsonArray &flights) {
    // 清空现有结果
    QLayout *layout = resultsContainer->layout();
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

    // 创建航班卡片
    for (const QJsonValue &value : flights) {
        createFlightResultCard(value.toObject());
    }

    layout->addStretch();
}

void ModernFlightSearchWindow::createFlightResultCard(const QJsonObject &flight) {
    QFrame *card = new QFrame();
    card->setStyleSheet(
        "QFrame {"
        "   background-color: white;"
        "   border: 1px solid #E5E7EB;"
        "   border-radius: 12px;"
        "   padding: 20px;"
        "}"
        "QFrame:hover {"
        "   border: 2px solid #0052CC;"
        "   box-shadow: 0 10px 30px rgba(0, 82, 204, 0.15);"
        "}"
    );

    QHBoxLayout *cardLayout = new QHBoxLayout(card);
    cardLayout->setSpacing(30);

    // 航班信息 - 左侧
    QVBoxLayout *leftLayout = new QVBoxLayout();

    QLabel *airlineLabel = new QLabel(flight["airline"].toString() + " - " + flight["flightNumber"].toString());
    airlineLabel->setStyleSheet("QLabel { color: #0052CC; font-weight: 700; font-size: 15px; }");
    leftLayout->addWidget(airlineLabel);

    QLabel *aircraftLabel = new QLabel(flight["aircraft"].toString());
    aircraftLabel->setStyleSheet("QLabel { color: #6B7280; font-size: 12px; }");
    leftLayout->addWidget(aircraftLabel);

    cardLayout->addLayout(leftLayout);

    // 时间和路线 - 中间
    QVBoxLayout *middleLayout = new QVBoxLayout();
    middleLayout->setAlignment(Qt::AlignCenter);

    QLabel *timeLabel = new QLabel(flight["departureTime"].toString() + " - " + flight["arrivalTime"].toString());
    timeLabel->setStyleSheet("QLabel { color: #111827; font-weight: 600; font-size: 14px; }");
    middleLayout->addWidget(timeLabel);

    QString duration = flight["duration"].toString();
    QString stops = QString::number(flight["stops"].toInt());
    QLabel *durationLabel = new QLabel(duration + " • " + stops + (stops == "0" ? " stops" : " stop"));
    durationLabel->setStyleSheet("QLabel { color: #6B7280; font-size: 13px; }");
    middleLayout->addWidget(durationLabel);

    cardLayout->addLayout(middleLayout);
    cardLayout->addStretch();

    // 座位和价格 - 右侧
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setAlignment(Qt::AlignCenter);

    QJsonObject availSeats = flight["availableSeats"].toObject();
    QLabel *seatsLabel = new QLabel("✓ " + QString::number(availSeats["economy"].toInt()) + " seats");
    seatsLabel->setStyleSheet("QLabel { color: #10B981; font-size: 13px; font-weight: 600; }");
    rightLayout->addWidget(seatsLabel);

    rightLayout->addSpacing(10);

    // 价格
    QJsonObject pricing = flight["pricing"].toObject();
    double price = pricing["economy"].toDouble();
    QLabel *priceLabel = new QLabel("$" + QString::number(price, 'f', 2));
    priceLabel->setStyleSheet(
        "QLabel {"
        "   color: #10B981;"
        "   font-size: 22px;"
        "   font-weight: 700;"
        "}"
    );
    rightLayout->addWidget(priceLabel);

    cardLayout->addLayout(rightLayout);

    // 预订按钮
    QPushButton *bookBtn = new QPushButton("Book");
    bookBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #0052CC;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 6px;"
        "   padding: 10px 24px;"
        "   font-weight: 700;"
        "   font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0048B8;"
        "}"
    );
    bookBtn->setFixedWidth(100);
    bookBtn->setCursor(Qt::PointingHandCursor);
    cardLayout->addWidget(bookBtn);

    dynamic_cast<QVBoxLayout*>(resultsContainer->layout())->insertWidget(
        resultsContainer->layout()->count() - 1, card
    );
}

void ModernFlightSearchWindow::onFilterChanged() {
    applyFilters();
}

void ModernFlightSearchWindow::onSortChanged(int index) {
    qDebug() << "Sort by index:" << index;
    applyFilters();
}

void ModernFlightSearchWindow::applyFilters() {
    // 这里实现过滤逻辑
    qDebug() << "Applying filters...";
}

void ModernFlightSearchWindow::onFlightSelected(int flightId) {
    qDebug() << "Flight selected:" << flightId;
    // 导航到预订页面
}
