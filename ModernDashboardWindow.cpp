#include "ModernDashboardWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QApplication>
#include <QScreen>
#include <QMessageBox>
#include <QTimer>
#include <QSpacerItem>

ModernDashboardWindow::ModernDashboardWindow(QWidget *parent)
    : QMainWindow(parent), networkManager(new QNetworkAccessManager(this)) {
    setupUI();
    connectSignals();
    applyTheme();
    loadFeaturedFlights();
    loadRecentSearches();
}

ModernDashboardWindow::~ModernDashboardWindow() {
}

void ModernDashboardWindow::setupUI() {
    setWindowTitle("Sky Wings - 仪表盘");
    setMinimumSize(1400, 900);
    
    // 居中窗口
    QRect screen = QApplication::primaryScreen()->geometry();
    resize(1400, 900);
    move((screen.width() - width()) / 2, (screen.height() - height()) / 2);
    
    // 创建中央部件
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // 设置导航栏
    setupNavigationBar();
    mainLayout->addWidget(navigationBar);
    
    // 创建内容区域
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    
    // 设置侧边栏
    setupSidebar();
    contentLayout->addWidget(sidebar);
    
    // 设置主内容区
    setupMainContent();
    contentLayout->addWidget(mainContent);
    
    mainLayout->addLayout(contentLayout);
}

void ModernDashboardWindow::setupNavigationBar() {
    navigationBar = new QFrame();
    navigationBar->setFixedHeight(70);
    navigationBar->setStyleSheet(
        "QFrame {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #1E40AF, stop:1 #3B82F6);"
        "   border: none;"
        "}"
    );
    
    QHBoxLayout *navLayout = new QHBoxLayout(navigationBar);
    navLayout->setContentsMargins(24, 0, 24, 0);
    
    // Logo
    logoLabel = new QLabel("✈ Sky Wings");
    logoLabel->setStyleSheet(
        "QLabel {"
        "   color: white;"
        "   font-size: 24px;"
        "   font-weight: 700;"
        "   letter-spacing: 1px;"
        "}"
    );
    navLayout->addWidget(logoLabel);
    
    navLayout->addStretch();
    
    // 导航按钮
    searchButton = new QPushButton("🔍 搜索航班");
    searchButton->setStyleSheet(
        "QPushButton {"
        "   background: transparent;"
        "   color: rgba(255, 255, 255, 0.9);"
        "   border: none;"
        "   padding: 10px 20px;"
        "   border-radius: 8px;"
        "   font-size: 15px;"
        "   font-weight: 500;"
        "}"
        "QPushButton:hover {"
        "   background: rgba(255, 255, 255, 0.1);"
        "   color: white;"
        "}"
    );
    navLayout->addWidget(searchButton);
    
    bookingsButton = new QPushButton("📋 我的预订");
    bookingsButton->setStyleSheet(
        "QPushButton {"
        "   background: transparent;"
        "   color: rgba(255, 255, 255, 0.9);"
        "   border: none;"
        "   padding: 10px 20px;"
        "   border-radius: 8px;"
        "   font-size: 15px;"
        "   font-weight: 500;"
        "}"
        "QPushButton:hover {"
        "   background: rgba(255, 255, 255, 0.1);"
        "   color: white;"
        "}"
    );
    navLayout->addWidget(bookingsButton);
    
    profileButton = new QPushButton("👤 个人资料");
    profileButton->setStyleSheet(
        "QPushButton {"
        "   background: transparent;"
        "   color: rgba(255, 255, 255, 0.9);"
        "   border: none;"
        "   padding: 10px 20px;"
        "   border-radius: 8px;"
        "   font-size: 15px;"
        "   font-weight: 500;"
        "}"
        "QPushButton:hover {"
        "   background: rgba(255, 255, 255, 0.1);"
        "   color: white;"
        "}"
    );
    navLayout->addWidget(profileButton);
    
    logoutButton = new QPushButton("🚪 退出");
    logoutButton->setStyleSheet(
        "QPushButton {"
        "   background: rgba(239, 68, 68, 0.2);"
        "   color: white;"
        "   border: 1px solid rgba(239, 68, 68, 0.5);"
        "   padding: 8px 16px;"
        "   border-radius: 6px;"
        "   font-size: 14px;"
        "   font-weight: 500;"
        "}"
        "QPushButton:hover {"
        "   background: rgba(239, 68, 68, 0.3);"
        "}"
    );
    navLayout->addWidget(logoutButton);
}

void ModernDashboardWindow::setupSidebar() {
    sidebar = new QFrame();
    sidebar->setFixedWidth(280);
    sidebar->setStyleSheet(
        "QFrame {"
        "   background-color: #F8FAFC;"
        "   border-right: 1px solid #E2E8F0;"
        "}"
    );
    
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(24, 24, 24, 24);
    sidebarLayout->setSpacing(20);
    
    // 用户信息区域
    QFrame *userInfoFrame = new QFrame();
    userInfoFrame->setStyleSheet(
        "QFrame {"
        "   background: white;"
        "   border: 1px solid #E2E8F0;"
        "   border-radius: 12px;"
        "   padding: 20px;"
        "}"
    );
    
    QVBoxLayout *userInfoLayout = new QVBoxLayout(userInfoFrame);
    userInfoLayout->setAlignment(Qt::AlignCenter);
    
    // 用户头像 (使用Unicode字符)
    userAvatar = new QLabel("👤");
    userAvatar->setStyleSheet(
        "QLabel {"
        "   font-size: 48px;"
        "   color: #3B82F6;"
        "   background-color: #EFF6FF;"
        "   border-radius: 24px;"
        "   padding: 12px;"
        "   margin-bottom: 12px;"
        "}"
    );
    userAvatar->setAlignment(Qt::AlignCenter);
    userInfoLayout->addWidget(userAvatar);
    
    userName = new QLabel("张三");
    userName->setStyleSheet(
        "QLabel {"
        "   color: #1E293B;"
        "   font-size: 18px;"
        "   font-weight: 600;"
        "   margin-bottom: 4px;"
        "}"
    );
    userName->setAlignment(Qt::AlignCenter);
    userInfoLayout->addWidget(userName);
    
    userEmail = new QLabel("zhangsan@example.com");
    userEmail->setStyleSheet(
        "QLabel {"
        "   color: #64748B;"
        "   font-size: 14px;"
        "}"
    );
    userEmail->setAlignment(Qt::AlignCenter);
    userInfoLayout->addWidget(userEmail);
    
    sidebarLayout->addWidget(userInfoFrame);
    
    // 导航菜单
    dashboardBtn = new QPushButton("🏠 仪表盘");
    dashboardBtn->setStyleSheet(
        "QPushButton {"
        "   background: #EFF6FF;"
        "   color: #1E40AF;"
        "   border: none;"
        "   padding: 14px 16px;"
        "   border-radius: 10px;"
        "   font-size: 15px;"
        "   font-weight: 500;"
        "   text-align: left;"
        "}"
        "QPushButton:hover {"
        "   background: #DBEAFE;"
        "}"
    );
    sidebarLayout->addWidget(dashboardBtn);
    
    flightsBtn = new QPushButton("✈️ 航班搜索");
    flightsBtn->setStyleSheet(
        "QPushButton {"
        "   background: transparent;"
        "   color: #64748B;"
        "   border: none;"
        "   padding: 14px 16px;"
        "   border-radius: 10px;"
        "   font-size: 15px;"
        "   font-weight: 500;"
        "   text-align: left;"
        "}"
        "QPushButton:hover {"
        "   background: #F1F5F9;"
        "   color: #1E293B;"
        "}"
    );
    sidebarLayout->addWidget(flightsBtn);
    
    bookingsBtn = new QPushButton("📋 我的预订");
    bookingsBtn->setStyleSheet(
        "QPushButton {"
        "   background: transparent;"
        "   color: #64748B;"
        "   border: none;"
        "   padding: 14px 16px;"
        "   border-radius: 10px;"
        "   font-size: 15px;"
        "   font-weight: 500;"
        "   text-align: left;"
        "}"
        "QPushButton:hover {"
        "   background: #F1F5F9;"
        "   color: #1E293B;"
        "}"
    );
    sidebarLayout->addWidget(bookingsBtn);
    
    settingsBtn = new QPushButton("⚙️ 设置");
    settingsBtn->setStyleSheet(
        "QPushButton {"
        "   background: transparent;"
        "   color: #64748B;"
        "   border: none;"
        "   padding: 14px 16px;"
        "   border-radius: 10px;"
        "   font-size: 15px;"
        "   font-weight: 500;"
        "   text-align: left;"
        "}"
        "QPushButton:hover {"
        "   background: #F1F5F9;"
        "   color: #1E293B;"
        "}"
    );
    sidebarLayout->addWidget(settingsBtn);
    
    helpBtn = new QPushButton("❓ 帮助中心");
    helpBtn->setStyleSheet(
        "QPushButton {"
        "   background: transparent;"
        "   color: #64748B;"
        "   border: none;"
        "   padding: 14px 16px;"
        "   border-radius: 10px;"
        "   font-size: 15px;"
        "   font-weight: 500;"
        "   text-align: left;"
        "}"
        "QPushButton:hover {"
        "   background: #F1F5F9;"
        "   color: #1E293B;"
        "}"
    );
    sidebarLayout->addWidget(helpBtn);
    
    sidebarLayout->addStretch();
}

void ModernDashboardWindow::setupMainContent() {
    mainContent = new QFrame();
    mainContent->setStyleSheet(
        "QFrame {"
        "   background-color: #F8FAFC;"
        "}"
    );
    
    QVBoxLayout *contentLayout = new QVBoxLayout(mainContent);
    contentLayout->setContentsMargins(32, 32, 32, 32);
    contentLayout->setSpacing(24);
    
    // 欢迎区域
    welcomeLabel = new QLabel("欢迎回来，张三！");
    welcomeLabel->setStyleSheet(
        "QLabel {"
        "   color: #1E293B;"
        "   font-size: 32px;"
        "   font-weight: 700;"
        "   margin-bottom: 8px;"
        "}"
    );
    contentLayout->addWidget(welcomeLabel);
    
    subtitleLabel = new QLabel("开始您的下一次飞行冒险，探索世界各地的精彩目的地");
    subtitleLabel->setStyleSheet(
        "QLabel {"
        "   color: #64748B;"
        "   font-size: 16px;"
        "   margin-bottom: 32px;"
        "}"
    );
    contentLayout->addWidget(subtitleLabel);
    
    // 快速搜索区域
    searchSection = new QFrame();
    searchSection->setStyleSheet(
        "QFrame {"
        "   background: white;"
        "   border: 1px solid #E2E8F0;"
        "   border-radius: 16px;"
        "   padding: 32px;"
        "}"
    );
    
    QVBoxLayout *searchLayout = new QVBoxLayout(searchSection);
    searchLayout->setSpacing(20);
    
    QLabel *searchTitle = new QLabel("🔍 快速搜索航班");
    searchTitle->setStyleSheet(
        "QLabel {"
        "   color: #1E293B;"
        "   font-size: 20px;"
        "   font-weight: 600;"
        "   margin-bottom: 16px;"
        "}"
    );
    searchLayout->addWidget(searchTitle);
    
    // 搜索表单
    QGridLayout *searchFormLayout = new QGridLayout();
    searchFormLayout->setSpacing(16);
    
    searchFromInput = new QLineEdit();
    searchFromInput->setPlaceholderText("出发城市");
    searchFromInput->setStyleSheet(
        "QLineEdit {"
        "   background: #F8FAFC;"
        "   border: 2px solid #E2E8F0;"
        "   border-radius: 10px;"
        "   padding: 14px 16px;"
        "   font-size: 15px;"
        "   color: #1E293B;"
        "}"
        "QLineEdit:focus {"
        "   border-color: #3B82F6;"
        "   background-color: white;"
        "}"
    );
    searchFormLayout->addWidget(searchFromInput, 0, 0);
    
    searchToInput = new QLineEdit();
    searchToInput->setPlaceholderText("目的地城市");
    searchToInput->setStyleSheet(
        "QLineEdit {"
        "   background: #F8FAFC;"
        "   border: 2px solid #E2E8F0;"
        "   border-radius: 10px;"
        "   padding: 14px 16px;"
        "   font-size: 15px;"
        "   color: #1E293B;"
        "}"
        "QLineEdit:focus {"
        "   border-color: #3B82F6;"
        "   background-color: white;"
        "}"
    );
    searchFormLayout->addWidget(searchToInput, 0, 1);
    
    departureDate = new QDateEdit();
    departureDate->setDate(QDate::currentDate().addDays(7));
    departureDate->setCalendarPopup(true);
    departureDate->setStyleSheet(
        "QDateEdit {"
        "   background: #F8FAFC;"
        "   border: 2px solid #E2E8F0;"
        "   border-radius: 10px;"
        "   padding: 14px 16px;"
        "   font-size: 15px;"
        "   color: #1E293B;"
        "}"
        "QDateEdit:focus {"
        "   border-color: #3B82F6;"
        "   background-color: white;"
        "}"
    );
    searchFormLayout->addWidget(departureDate, 0, 2);
    
    searchFlightsBtn = new QPushButton("搜索航班");
    searchFlightsBtn->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #3B82F6, stop:1 #1E40AF);"
        "   color: white;"
        "   border: none;"
        "   border-radius: 10px;"
        "   padding: 14px 24px;"
        "   font-size: 15px;"
        "   font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #60A5FA, stop:1 #2563EB);"
        "}"
    );
    searchFormLayout->addWidget(searchFlightsBtn, 0, 3);
    
    searchLayout->addLayout(searchFormLayout);
    contentLayout->addWidget(searchSection);
    
    // 特色航班区域
    setupFeaturedFlights();
    contentLayout->addWidget(featuredSection);
    
    // 快速操作区域
    setupQuickActions();
    contentLayout->addWidget(quickActionsSection);
    
    contentLayout->addStretch();
}

void ModernDashboardWindow::setupFeaturedFlights() {
    featuredSection = new QFrame();
    featuredSection->setStyleSheet(
        "QFrame {"
        "   background: white;"
        "   border: 1px solid #E2E8F0;"
        "   border-radius: 16px;"
        "   padding: 32px;"
        "}"
    );
    
    QVBoxLayout *featuredLayout = new QVBoxLayout(featuredSection);
    featuredLayout->setSpacing(20);
    
    featuredTitle = new QLabel("🌟 特色航班推荐");
    featuredTitle->setStyleSheet(
        "QLabel {"
        "   color: #1E293B;"
        "   font-size: 20px;"
        "   font-weight: 600;"
        "   margin-bottom: 16px;"
        "}"
    );
    featuredLayout->addWidget(featuredTitle);
    
    featuredFlightsWidget = new QWidget();
    featuredFlightsWidget->setStyleSheet(
        "QWidget {"
        "   background: transparent;"
        "}"
    );
    
    QHBoxLayout *flightsLayout = new QHBoxLayout(featuredFlightsWidget);
    flightsLayout->setSpacing(20);
    
    // 创建示例航班卡片
    createFlightCard("CA1234", "北京", "上海", "¥899", "08:30-10:45");
    createFlightCard("MU5678", "广州", "深圳", "¥459", "14:20-15:30");
    createFlightCard("CZ9012", "成都", "重庆", "¥399", "09:15-10:25");
    
    // 这里会动态添加航班卡片
    featuredLayout->addWidget(featuredFlightsWidget);
}

void ModernDashboardWindow::setupQuickActions() {
    quickActionsSection = new QFrame();
    quickActionsSection->setStyleSheet(
        "QFrame {"
        "   background: white;"
        "   border: 1px solid #E2E8F0;"
        "   border-radius: 16px;"
        "   padding: 32px;"
        "}"
    );
    
    QVBoxLayout *actionsLayout = new QVBoxLayout(quickActionsSection);
    actionsLayout->setSpacing(20);
    
    QLabel *actionsTitle = new QLabel("⚡ 快速操作");
    actionsTitle->setStyleSheet(
        "QLabel {"
        "   color: #1E293B;"
        "   font-size: 20px;"
        "   font-weight: 600;"
        "   margin-bottom: 16px;"
        "}"
    );
    actionsLayout->addWidget(actionsTitle);
    
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSpacing(16);
    
    quickSearchBtn = new QPushButton("🔍 高级搜索");
    quickSearchBtn->setStyleSheet(
        "QPushButton {"
        "   background: #EFF6FF;"
        "   color: #1E40AF;"
        "   border: 2px solid #DBEAFE;"
        "   border-radius: 12px;"
        "   padding: 16px 24px;"
        "   font-size: 15px;"
        "   font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "   background: #DBEAFE;"
        "   border-color: #93C5FD;"
        "}"
    );
    buttonsLayout->addWidget(quickSearchBtn);
    
    quickBookingBtn = new QPushButton("📅 管理预订");
    quickBookingBtn->setStyleSheet(
        "QPushButton {"
        "   background: #F0FDF4;"
        "   color: #166534;"
        "   border: 2px solid #DCFCE7;"
        "   border-radius: 12px;"
        "   padding: 16px 24px;"
        "   font-size: 15px;"
        "   font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "   background: #DCFCE7;"
        "   border-color: #BBF7D0;"
        "}"
    );
    buttonsLayout->addWidget(quickBookingBtn);
    
    quickSupportBtn = new QPushButton("💬 客服支持");
    quickSupportBtn->setStyleSheet(
        "QPushButton {"
        "   background: #FEF3C7;"
        "   color: #92400E;"
        "   border: 2px solid #FDE68A;"
        "   border-radius: 12px;"
        "   padding: 16px 24px;"
        "   font-size: 15px;"
        "   font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "   background: #FDE68A;"
        "   border-color: #FCD34D;"
        "}"
    );
    buttonsLayout->addWidget(quickSupportBtn);
    
    buttonsLayout->addStretch();
    actionsLayout->addLayout(buttonsLayout);
}

void ModernDashboardWindow::createFlightCard(const QString &flightNumber, 
                                           const QString &from, const QString &to, 
                                           const QString &price, const QString &time) {
    QFrame *card = new QFrame();
    card->setFixedSize(280, 140);
    card->setStyleSheet(
        "QFrame {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "       stop:0 #F8FAFC, stop:1 #F1F5F9);"
        "   border: 1px solid #E2E8F0;"
        "   border-radius: 12px;"
        "   padding: 20px;"
        "}"
        "QFrame:hover {"
        "   border-color: #3B82F6;"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "       stop:0 #EFF6FF, stop:1 #DBEAFE);"
        "}"
    );
    
    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(8);
    
    // 航班号
    QLabel *flightLabel = new QLabel(flightNumber);
    flightLabel->setStyleSheet(
        "QLabel {"
        "   color: #3B82F6;"
        "   font-size: 14px;"
        "   font-weight: 600;"
        "}"
    );
    cardLayout->addWidget(flightLabel);
    
    // 航线
    QLabel *routeLabel = new QLabel(QString("%1 → %2").arg(from, to));
    routeLabel->setStyleSheet(
        "QLabel {"
        "   color: #1E293B;"
        "   font-size: 16px;"
        "   font-weight: 600;"
        "}"
    );
    cardLayout->addWidget(routeLabel);
    
    // 时间
    QLabel *timeLabel = new QLabel(time);
    timeLabel->setStyleSheet(
        "QLabel {"
        "   color: #64748B;"
        "   font-size: 13px;"
        "}"
    );
    cardLayout->addWidget(timeLabel);
    
    // 价格
    QLabel *priceLabel = new QLabel(price);
    priceLabel->setStyleSheet(
        "QLabel {"
        "   color: #10B981;"
        "   font-size: 18px;"
        "   font-weight: 700;"
        "}"
    );
    cardLayout->addWidget(priceLabel);
    
    // 添加到特色航班区域
    QHBoxLayout *flightsLayout = qobject_cast<QHBoxLayout*>(featuredFlightsWidget->layout());
    if (flightsLayout) {
        flightsLayout->addWidget(card);
        animateCard(card);
    }
}

void ModernDashboardWindow::animateCard(QWidget *card) {
    QPropertyAnimation *animation = new QPropertyAnimation(card, "geometry", this);
    QRect startGeometry = card->geometry();
    QRect endGeometry = startGeometry.adjusted(0, -20, 0, -20);
    
    animation->setDuration(600);
    animation->setStartValue(endGeometry);
    animation->setEndValue(startGeometry);
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void ModernDashboardWindow::connectSignals() {
    connect(searchButton, &QPushButton::clicked, this, &ModernDashboardWindow::onSearchFlightsClicked);
    connect(bookingsButton, &QPushButton::clicked, this, &ModernDashboardWindow::onMyBookingsClicked);
    connect(profileButton, &QPushButton::clicked, this, &ModernDashboardWindow::onProfileClicked);
    connect(logoutButton, &QPushButton::clicked, this, &ModernDashboardWindow::onLogoutClicked);
    connect(searchFlightsBtn, &QPushButton::clicked, this, &ModernDashboardWindow::onSearchFlightsClicked);
    connect(flightsBtn, &QPushButton::clicked, this, &ModernDashboardWindow::onSearchFlightsClicked);
    connect(bookingsBtn, &QPushButton::clicked, this, &ModernDashboardWindow::onMyBookingsClicked);
    connect(quickSearchBtn, &QPushButton::clicked, this, &ModernDashboardWindow::onSearchFlightsClicked);
    connect(quickBookingBtn, &QPushButton::clicked, this, &ModernDashboardWindow::onMyBookingsClicked);
}

void ModernDashboardWindow::applyTheme() {
    QFile styleFile(":/modern_theme.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        setStyleSheet(styleFile.readAll());
    }
}

void ModernDashboardWindow::loadFeaturedFlights() {
    // 模拟加载特色航班数据
    QTimer::singleShot(1000, this, &ModernDashboardWindow::onFeaturedFlightsLoaded);
}

void ModernDashboardWindow::loadRecentSearches() {
    // 模拟加载最近搜索数据
    QTimer::singleShot(1500, this, &ModernDashboardWindow::onRecentSearchesLoaded);
}

void ModernDashboardWindow::onSearchFlightsClicked() {
    // 切换到航班搜索页面
    // InterfaceManager::instance()->switchToPage("modern_flight_search");
}

void ModernDashboardWindow::onMyBookingsClicked() {
    // 切换到我的预订页面
    // InterfaceManager::instance()->switchToPage("modern_bookings");
}

void ModernDashboardWindow::onProfileClicked() {
    // 切换到个人资料页面
    // InterfaceManager::instance()->switchToPage("modern_profile");
}

void ModernDashboardWindow::onLogoutClicked() {
    // 处理退出登录
    QMessageBox::StandardButton reply = QMessageBox::question(this, "确认退出", 
        "您确定要退出登录吗？", 
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        // 清除用户会话
        // UserSession::instance()->clearUser();
        
        // 切换到登录页面
        // InterfaceManager::instance()->switchToPage("modern_login");
        close();
    }
}

void ModernDashboardWindow::onFeaturedFlightsLoaded() {
    // 特色航班加载完成
    qDebug() << "Featured flights loaded successfully";
}

void ModernDashboardWindow::onRecentSearchesLoaded() {
    // 最近搜索加载完成
    qDebug() << "Recent searches loaded successfully";
}