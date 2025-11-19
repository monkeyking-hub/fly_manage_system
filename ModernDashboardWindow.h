#ifndef MODERNDASHBOARDWINDOW_H
#define MODERNDASHBOARDWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPropertyAnimation>

class ModernDashboardWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ModernDashboardWindow(QWidget *parent = nullptr);
    ~ModernDashboardWindow();

private slots:
    void onSearchFlightsClicked();
    void onMyBookingsClicked();
    void onProfileClicked();
    void onLogoutClicked();
    void onFeaturedFlightsLoaded();
    void onRecentSearchesLoaded();

private:
    void setupUI();
    void connectSignals();
    void setupNavigationBar();
    void setupSidebar();
    void setupMainContent();
    void setupFeaturedFlights();
    void setupRecentSearches();
    void setupQuickActions();
    void loadFeaturedFlights();
    void loadRecentSearches();
    void applyTheme();
    void createFlightCard(const QString &flightNumber, const QString &from, 
                        const QString &to, const QString &price, const QString &time);
    void animateCard(QWidget *card);

    // UI Components
    QWidget *centralWidget;
    QFrame *navigationBar;
    QFrame *sidebar;
    QFrame *mainContent;
    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    
    // Navigation Bar
    QLabel *logoLabel;
    QPushButton *searchButton;
    QPushButton *bookingsButton;
    QPushButton *profileButton;
    QPushButton *logoutButton;
    
    // Sidebar
    QLabel *userAvatar;
    QLabel *userName;
    QLabel *userEmail;
    QPushButton *dashboardBtn;
    QPushButton *flightsBtn;
    QPushButton *bookingsBtn;
    QPushButton *settingsBtn;
    QPushButton *helpBtn;
    
    // Main Content
    QLabel *welcomeLabel;
    QLabel *subtitleLabel;
    QFrame *searchSection;
    QLineEdit *searchFromInput;
    QLineEdit *searchToInput;
    QDateEdit *departureDate;
    QPushButton *searchFlightsBtn;
    
    QFrame *featuredSection;
    QLabel *featuredTitle;
    QWidget *featuredFlightsWidget;
    
    QFrame *recentSection;
    QLabel *recentTitle;
    QWidget *recentSearchesWidget;
    
    QFrame *quickActionsSection;
    QPushButton *quickSearchBtn;
    QPushButton *quickBookingBtn;
    QPushButton *quickSupportBtn;

    QNetworkAccessManager *networkManager;
    QPropertyAnimation *slideAnimation;
};

#endif // MODERNDASHBOARDWINDOW_H