#ifndef MODERNBOOKINGWIZARD_H
#define MODERNBOOKINGWIZARD_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QDateEdit>
#include <QTextEdit>
#include <QStackedWidget>
#include <QJsonObject>
#include <QNetworkAccessManager>

class ModernBookingWizard : public QMainWindow {
    Q_OBJECT

public:
    explicit ModernBookingWizard(const QJsonObject &flightData, QWidget *parent = nullptr);
    ~ModernBookingWizard();

private slots:
    void onNextClicked();
    void onBackClicked();
    void onConfirmClicked();

private:
    void setupUI();
    void createStep1PassengerInfo();
    void createStep2SeatSelection();
    void createStep3Review();
    void createStep4Payment();
    void updateProgressBar(int currentStep);

    // 航班数据
    QJsonObject m_flightData;

    // UI Components
    QStackedWidget *stepsStack;
    QLabel *progressLabel;
    QLabel *progressBar;
    QPushButton *nextBtn;
    QPushButton *backBtn;
    QPushButton *confirmBtn;

    // 乘客信息
    QVector<QMap<QString, QLineEdit*>> passengerFields;

    int m_currentStep = 1;
    QNetworkAccessManager *networkManager;
};

#endif // MODERNBOOKINGWIZARD_H
