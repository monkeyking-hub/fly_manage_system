#ifndef MODERNFLIGHTSEARCHWINDOW_H
#define MODERNFLIGHTSEARCHWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QDateEdit>
#include <QSlider>
#include <QCheckBox>
#include <QScrollArea>
#include <QFrame>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>

class ModernFlightSearchWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ModernFlightSearchWindow(const QString &departure, const QString &destination,
                                     const QDate &departDate, QWidget *parent = nullptr);
    ~ModernFlightSearchWindow();

private slots:
    void onFilterChanged();
    void onSortChanged(int index);
    void onFlightSelected(int flightId);
    void loadSearchResults();
    void onSearchCompleted(const QJsonArray &flights);

private:
    void setupUI();
    void setupFilterPanel();
    void createFlightResultCard(const QJsonObject &flight);
    void createSortControls();
    void applyFilters();

    // 搜索参数
    QString m_departure;
    QString m_destination;
    QDate m_departDate;
    QJsonArray m_allFlights;

    // UI Components
    QFrame *filterPanel;
    QScrollArea *resultsScroll;
    QFrame *resultsContainer;
    
    // 过滤器
    QSlider *priceSlider;
    QLabel *priceLabel;
    QCheckBox *directOnly;
    QCheckBox *oneStopOnly;
    QComboBox *sortCombo;
    QCheckBox *airlineFilter[10];
    QSlider *departTimeSlider;
    QSlider *arriveTimeSlider;

    QNetworkAccessManager *networkManager;
};

#endif // MODERNFLIGHTSEARCHWINDOW_H
