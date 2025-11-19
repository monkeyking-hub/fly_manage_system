#ifndef MODERNDASHBOARDWINDOW_H
#define MODERNDASHBOARDWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QDateEdit>
#include <QSpinBox>
#include <QScrollArea>
#include <QFrame>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class ModernDashboardWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ModernDashboardWindow(QWidget *parent = nullptr);
    ~ModernDashboardWindow();

private slots:
    void onSearchClicked();
    void onFlightCardClicked(int flightId);
    void loadFeaturedFlights();
    void onFeaturedFlightsLoaded(const QJsonArray &flights);

private:
    void setupUI();
    void connectSignals();
    void createFlightCard(const QJsonObject &flight);
    void createQuickAccessCard(const QString &title, const QString &icon, const QString &description);

    // UI Components
    QLineEdit *departureInput;
    QLineEdit *destinationInput;
    QDateEdit *departureDateEdit;
    QDateEdit *returnDateEdit;
    QSpinBox *passengersSpinBox;
    QPushButton *searchButton;
    QScrollArea *featuredFlightsScroll;
    QFrame *featuredFlightsContainer;
    
    QNetworkAccessManager *networkManager;
};

#endif // MODERNDASHBOARDWINDOW_H
