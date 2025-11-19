#include "ModernDashboardWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QLabel>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QDebug>
#include <QApplication>
#include <QScreen>
#include <QDateTime>

ModernDashboardWindow::ModernDashboardWindow(QWidget *parent)
    : QMainWindow(parent), networkManager(new QNetworkAccessManager(this)) {
    setupUI();
    connectSignals();
    loadFeaturedFlights();
}

ModernDashboardWindow::~ModernDashboardWindow() {
}

void ModernDashboardWindow::setupUI() {
    setWindowTitle("Sky Wings - Dashboard");
    resize(1400, 900);
    QRect screen = QApplication::primaryScreen()->geometry();
    move((screen.width() - width()) / 2, (screen.height() - height()) / 2);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ==================== 顶部导航栏 ====================
    QFrame *navbar = new QFrame();
    navbar->setStyleSheet(
        "QFrame {"
        "   background-color: white;"
        "   border-bottom: 1px solid #E5E7EB;"
        "}"
    );
    navbar->setFixedHeight(70);

    QHBoxLayout *navLayout = new QHBoxLayout(navbar);
    navLayout->setContentsMargins(40, 0, 40, 0);
    navLayout->setSpacing(20);

    QLabel *logo = new QLabel("✈ Sky Wings");
    logo->setStyleSheet(
        "QLabel {"
        "   color: #0052CC;"
        "   font-size: 24px;"
        "   font-weight: 700;"
        "   letter-spacing: 1px;"
        "}"
    );
    navLayout->addWidget(logo);
    navLayout->addStretch();

    QPushButton *profileBtn = new QPushButton("👤 Profile");
    profileBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   color: #0052CC;"
        "   border: none;"
        "   font-size: 13px;"
        "   font-weight: 600;"
        "   padding: 8px 16px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #F0F4FF;"
        "   border-radius: 6px;"
        "}"
    );
    navLayout->addWidget(profileBtn);

    QPushButton *ordersBtn = new QPushButton("📋 Orders");
    ordersBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   color: #0052CC;"
        "   border: none;"
        "   font-size: 13px;"
        "   font-weight: 600;"
        "   padding: 8px 16px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #F0F4FF;"
        "   border-radius: 6px;"
        "}"
    );
    navLayout->addWidget(ordersBtn);

    QPushButton *logoutBtn = new QPushButton("Logout");
    logoutBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #FEE2E2;"
        "   color: #EF4444;"
        "   border: none;"
        "   border-radius: 6px;"
        "   font-size: 13px;"
        "   font-weight: 600;"
        "   padding: 8px 16px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #FECACA;"
        "}"
    );
    navLayout->addWidget(logoutBtn);

    mainLayout->addWidget(navbar);

    // ==================== 搜索面板 ====================
    QFrame *searchPanel = new QFrame();
    searchPanel->setStyleSheet(
        "QFrame {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "       stop:0 #0052CC, stop:1 #1E88E5);"
        "   border-radius: 0px;"
        "}"
    );
    searchPanel->setFixedHeight(280);

    QVBoxLayout *searchLayout = new QVBoxLayout(searchPanel);
    searchLayout->setContentsMargins(40, 40, 40, 40);
    searchLayout->setSpacing(25);

    QLabel *searchTitle = new QLabel("Search & Book Flights");
    searchTitle->setStyleSheet(
        "QLabel {"
        "   color: white;"
        "   font-size: 32px;"
        "   font-weight: 700;"
        "}"
    );
    searchLayout->addWidget(searchTitle);

    QLabel *searchSubtitle = new QLabel("Find the best deals on flights to your favorite destinations");
    searchSubtitle->setStyleSheet(
        "QLabel {"
        "   color: rgba(255, 255, 255, 0.9);"
        "   font-size: 15px;"
        "}"
    );
    searchLayout->addWidget(searchSubtitle);

    // 搜索输入行
    QHBoxLayout *searchInputLayout = new QHBoxLayout();
    searchInputLayout->setSpacing(15);

    // 出发地
    departureInput = new QLineEdit();
    departureInput->setPlaceholderText("Departure City");
    departureInput->setStyleSheet(
        "QLineEdit {"
        "   background-color: white;"
        "   color: #111827;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 12px 15px;"
        "   font-size: 14px;"
        "}"
    );
    departureInput->setFixedHeight(45);
    searchInputLayout->addWidget(departureInput);

    // 目的地
    destinationInput = new QLineEdit();
    destinationInput->setPlaceholderText("Destination City");
    destinationInput->setStyleSheet(
        "QLineEdit {"
        "   background-color: white;"
        "   color: #111827;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 12px 15px;"
        "   font-size: 14px;"
        "}"
    );
    destinationInput->setFixedHeight(45);
    searchInputLayout->addWidget(destinationInput);

    // 出发日期
    departureDateEdit = new QDateEdit();
    departureDateEdit->setDate(QDate::currentDate());
    departureDateEdit->setDisplayFormat("dd/MM/yyyy");
    departureDateEdit->setStyleSheet(
        "QDateEdit {"
        "   background-color: white;"
        "   color: #111827;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 12px 15px;"
        "   font-size: 14px;"
        "}"
    );
    departureDateEdit->setFixedHeight(45);
    searchInputLayout->addWidget(departureDateEdit);

    // 返回日期
    returnDateEdit = new QDateEdit();
    returnDateEdit->setDate(QDate::currentDate().addDays(7));
    returnDateEdit->setDisplayFormat("dd/MM/yyyy");
    returnDateEdit->setStyleSheet(
        "QDateEdit {"
        "   background-color: white;"
        "   color: #111827;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 12px 15px;"
        "   font-size: 14px;"
        "}"
    );
    returnDateEdit->setFixedHeight(45);
    searchInputLayout->addWidget(returnDateEdit);

    // 乘客数
    passengersSpinBox = new QSpinBox();
    passengersSpinBox->setMinimum(1);
    passengersSpinBox->setMaximum(9);
    passengersSpinBox->setValue(1);
    passengersSpinBox->setStyleSheet(
        "QSpinBox {"
        "   background-color: white;"
        "   color: #111827;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 12px 15px;"
        "   font-size: 14px;"
        "}"
    );
    passengersSpinBox->setFixedHeight(45);
    passengersSpinBox->setFixedWidth(80);
    searchInputLayout->addWidget(passengersSpinBox);

    // 搜索按钮
    searchButton = new QPushButton("Search");
    searchButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #10B981;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 12px 30px;"
        "   font-weight: 700;"
        "   font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #059669;"
        "}"
    );
    searchButton->setFixedHeight(45);
    searchButton->setCursor(Qt::PointingHandCursor);
    searchInputLayout->addWidget(searchButton);

    searchLayout->addLayout(searchInputLayout);
    searchLayout->addStretch();

    mainLayout->addWidget(searchPanel);

    // ==================== 内容区域 ====================
    QWidget *contentWidget = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(40, 40, 40, 40);
    contentLayout->setSpacing(30);

    // 快速访问卡片
    QLabel *quickAccessTitle = new QLabel("Quick Access");
    quickAccessTitle->setStyleSheet(
        "QLabel {"
        "   color: #111827;"
        "   font-size: 20px;"
        "   font-weight: 700;"
        "}"
    );
    contentLayout->addWidget(quickAccessTitle);

    QHBoxLayout *quickAccessLayout = new QHBoxLayout();
    quickAccessLayout->setSpacing(20);
    createQuickAccessCard("My Bookings", "📅", "View and manage your reservations");
    createQuickAccessCard("Popular Routes", "🌍", "Discover trending destinations");
    createQuickAccessCard("Best Deals", "💰", "Find special offers and discounts");
    createQuickAccessCard("Support", "💬", "Get help from our team");
    contentLayout->addLayout(quickAccessLayout);

    // 特色航班
    QLabel *featuredTitle = new QLabel("Featured Flights");
    featuredTitle->setStyleSheet(
        "QLabel {"
        "   color: #111827;"
        "   font-size: 20px;"
        "   font-weight: 700;"
        "}"
    );
    contentLayout->addWidget(featuredTitle);

    featuredFlightsScroll = new QScrollArea();
    featuredFlightsScroll->setWidgetResizable(true);
    featuredFlightsScroll->setStyleSheet(
        "QScrollArea { border: none; background-color: transparent; }"
    );

    featuredFlightsContainer = new QFrame();
    QVBoxLayout *flightsContainerLayout = new QVBoxLayout(featuredFlightsContainer);
    flightsContainerLayout->setSpacing(15);
    flightsContainerLayout->addStretch();

    featuredFlightsScroll->setWidget(featuredFlightsContainer);
    contentLayout->addWidget(featuredFlightsScroll);

    mainLayout->addWidget(contentWidget, 1);
}

void ModernDashboardWindow::connectSignals() {
    connect(searchButton, &QPushButton::clicked, this, &ModernDashboardWindow::onSearchClicked);
}

void ModernDashboardWindow::onSearchClicked() {
    QString departure = departureInput->text().trimmed();
    QString destination = destinationInput->text().trimmed();

    if (departure.isEmpty() || destination.isEmpty()) {
        qWarning() << "Please fill in all required fields";
        return;
    }

    qDebug() << "Searching flights from" << departure << "to" << destination;
    // 导航到搜索结果页面
}

void ModernDashboardWindow::loadFeaturedFlights() {
    QUrl url("http://localhost:8080/api/flights/featured");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject responseObject = jsonResponse.object();

            if (responseObject["code"].toInt() == 200) {
                onFeaturedFlightsLoaded(responseObject["data"].toArray());
            }
        }
        reply->deleteLater();
    });
}

void ModernDashboardWindow::onFeaturedFlightsLoaded(const QJsonArray &flights) {
    // 清空现有的航班卡片
    QLayout *layout = featuredFlightsContainer->layout();
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

    // 创建新的航班卡片
    for (const QJsonValue &value : flights) {
        createFlightCard(value.toObject());
    }

    layout->addStretch();
}

void ModernDashboardWindow::createFlightCard(const QJsonObject &flight) {
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
        "   box-shadow: 0 10px 25px rgba(0, 82, 204, 0.1);"
        "}"
    );

    QHBoxLayout *cardLayout = new QHBoxLayout(card);
    cardLayout->setSpacing(30);

    // 航班信息
    QString airline = flight["airline"].toString();
    QString flightNum = flight["flightNumber"].toString();
    QString departure = flight["departure"].toString();
    QString destination = flight["destination"].toString();
    QString departTime = flight["departureTime"].toString();
    QString arrivalTime = flight["arrivalTime"].toString();
    double price = flight["price"].toDouble();

    QVBoxLayout *infoLayout = new QVBoxLayout();

    QLabel *headerLabel = new QLabel(airline + " - " + flightNum);
    headerLabel->setStyleSheet("QLabel { color: #0052CC; font-weight: 700; font-size: 15px; }");
    infoLayout->addWidget(headerLabel);

    QLabel *routeLabel = new QLabel(departure + " → " + destination);
    routeLabel->setStyleSheet("QLabel { color: #111827; font-weight: 600; font-size: 16px; }");
    infoLayout->addWidget(routeLabel);

    QLabel *timeLabel = new QLabel(departTime + " - " + arrivalTime);
    timeLabel->setStyleSheet("QLabel { color: #6B7280; font-size: 13px; }");
    infoLayout->addWidget(timeLabel);

    cardLayout->addLayout(infoLayout, 1);

    // 价格和预订按钮
    QVBoxLayout *actionLayout = new QVBoxLayout();
    actionLayout->setAlignment(Qt::AlignCenter);

    QLabel *priceLabel = new QLabel("$" + QString::number(price, 'f', 2));
    priceLabel->setStyleSheet(
        "QLabel {"
        "   color: #10B981;"
        "   font-size: 20px;"
        "   font-weight: 700;"
        "}"
    );
    actionLayout->addWidget(priceLabel, 0, Qt::AlignCenter);

    QPushButton *bookBtn = new QPushButton("Book Now");
    bookBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #0052CC;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 6px;"
        "   padding: 10px 20px;"
        "   font-weight: 600;"
        "   font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0048B8;"
        "}"
    );
    bookBtn->setFixedWidth(120);
    bookBtn->setCursor(Qt::PointingHandCursor);
    actionLayout->addWidget(bookBtn, 0, Qt::AlignCenter);

    cardLayout->addLayout(actionLayout);

    dynamic_cast<QVBoxLayout*>(featuredFlightsContainer->layout())->insertWidget(
        featuredFlightsContainer->layout()->count() - 1, card
    );
}

void ModernDashboardWindow::createQuickAccessCard(const QString &title, const QString &icon, const QString &description) {
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
        "   background-color: #F0F4FF;"
        "}"
    );
    card->setMinimumWidth(250);
    card->setMaximumWidth(300);

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setSpacing(12);

    QLabel *iconLabel = new QLabel(icon);
    iconLabel->setStyleSheet("QLabel { font-size: 32px; }");
    layout->addWidget(iconLabel);

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet(
        "QLabel {"
        "   color: #111827;"
        "   font-size: 16px;"
        "   font-weight: 700;"
        "}"
    );
    layout->addWidget(titleLabel);

    QLabel *descLabel = new QLabel(description);
    descLabel->setStyleSheet(
        "QLabel {"
        "   color: #6B7280;"
        "   font-size: 13px;"
        "}"
    );
    descLabel->setWordWrap(true);
    layout->addWidget(descLabel);

    layout->addStretch();

    dynamic_cast<QHBoxLayout*>(
        dynamic_cast<QWidget*>(sender())->layout()
    )->addWidget(card);
}
