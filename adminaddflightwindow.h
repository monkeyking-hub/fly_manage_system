#ifndef ADMINADDFLIGHTWINDOW_H
#define ADMINADDFLIGHTWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDateTimeEdit>
#include <QSpinBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QCalendarWidget>

namespace Ui {
class adminAddFlightWindow;
}

class adminAddFlightWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit adminAddFlightWindow(QWidget *parent = nullptr);
    ~adminAddFlightWindow();

public:
    QPushButton *selectedBtn;

signals:
    void addFlightRequested(const QString &flightNumber,const QString& aircraftModel,const QString& departure,
                           const QString &destination,qint64 departureTime,qint64 arrivalTime,
                           const int &firstClssSeats,const int &economyClassSeats,const int &firstClassPrice,
                           const int &economyClassPrice,const QString &company);

public slots:
    void showCalendar(QPushButton *button);

    void selectDate(const QDate &date);

    void onAddFlightBtnClicked();

private:
    Ui::adminAddFlightWindow *ui;
};

class addFlightHandler:public QObject
{
    Q_OBJECT;

public slots:
    void handleAddFlight(const QString &flightNumber,const QString& aircraftModel,const QString& departure,
                         const QString &destination,qint64 departureTime,const qint64 arrivalTime,
                         const int &firstClassSeats,const int &economyClassSeats,const int &firstClassPrice,
                         const int &economyClassPrice,const QString &company);
};

#endif // ADMINADDFLIGHTWINDOW_H
