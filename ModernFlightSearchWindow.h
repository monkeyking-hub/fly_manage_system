#ifndef MODERNFLIGHTSEARCHWINDOW_H
#define MODERNFLIGHTSEARCHWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPropertyAnimation>
#include <QProgressBar>
#include <QComboBox>
#include <QSpinBox>
#include <QSlider>
#include <QCheckBox>

class ModernFlightSearchWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ModernFlightSearchWindow(QWidget *parent = nullptr);
    ~ModernFlightSearchWindow();

private slots:
    void onSearchClicked();
    void onResetClicked();
    void onBackClicked();
    void onFlightSelected(const QString &flightId);
    void onSearchResultsLoaded();
    void onFiltersChanged();

private:
    void setupUI();
    void connectSignals();
    void setupSearchForm();
    void setupResultsArea();
    void setupFilters();
    void applyTheme();
    void sendSearchRequest();
    void createResultCard(const QString &flightNumber, const QString &airline,
                         const QString &from, const QString &to, const QString &departure,
                         const QString &arrival, const QString &price, const QString &duration);
    void animateResult(QWidget *card);
    void applyFilters();

    // UI Components
    QWidget *centralWidget;
    QFrame *navigationBar;
    QFrame *mainContent;
    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    
    // Navigation
    QLabel *titleLabel;
    QPushButton *backButton;
    QPushButton *homeButton;
    
    // Search Form
    QFrame *searchFormFrame;
    QComboBox *tripTypeCombo;
    QLineEdit *fromInput;
    QLineEdit *toInput;
    QDateEdit *departureDate;
    QDateEdit *returnDate;
    QSpinBox *passengersSpin;
    QComboBox *classCombo;
    QPushButton *searchButton;
    QPushButton *resetButton;
    
    // Filters
    QFrame *filtersFrame;
    QCheckBox *directFlightCheck;
    QCheckBox *refundableCheck;
    QSlider *priceSlider;
    QLabel *priceLabel;
    QComboBox *airlineCombo;
    QComboBox *timeRangeCombo;
    
    // Results
    QFrame *resultsFrame;
    QLabel *resultsTitle;
    QLabel *resultsCount;
    QComboBox *sortByCombo;
    QWidget *resultsWidget;
    QProgressBar *loadingProgress;
    QLabel *loadingLabel;

    QNetworkAccessManager *networkManager;
    QPropertyAnimation *slideAnimation;
    
    // Search parameters
    QString currentFrom;
    QString currentTo;
    QString currentDeparture;
    QString currentReturn;
    int currentPassengers;
    QString currentClass;
    bool isDirectFlight;
    bool isRefundable;
    int maxPrice;
    QString selectedAirline;
    QString selectedTimeRange;
};

#endif // MODERNFLIGHTSEARCHWINDOW_H