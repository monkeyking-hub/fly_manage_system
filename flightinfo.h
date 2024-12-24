#ifndef FLIGHTINFO_H
#define FLIGHTINFO_H
#include <QString>
#include <QJsonObject>

class flightInfo
{
public:
    int id;
    QString flightNumber;
    QString departure;
    QString destination;
    qint64 departureTime;  // 使用 qint64 表示 UNIX 时间戳
    qint64 arrivalTime;    // 使用 qint64 表示 UNIX 时间戳
    int firstClassSeats;
    int economyClassSeats;
    double firstClassPrice;
    double economyClassPrice;
    QString airlineCompany;
    QString aircraftModel;
    QString boardingGate;
    QString arrivalAirport;
    QString departureAirport;
    int hasMeal;
    int isInternational;
    int luggageSizeLimit;
    int businessClassSeats;
    double businessClassPrice;
    QString chosenSeat;

    void fromJson(const QJsonObject &jsonObject,QString seat);
public:
    flightInfo();
};

#endif // FLIGHTINFO_H
