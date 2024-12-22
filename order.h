#ifndef ORDER_H
#define ORDER_H

#include <QString>

class Order
{
public:
    enum Status { Pending, Confirmed, Completed, Cancelled };

    // 默认构造函数
    Order();

    // 带参数的构造函数
    Order(const QString &orderNumber, const QString &passenger, const QString &amount,
          const QString &departure, const QString &destination, const QString &airline,
          const QString &flightNumber, const QString &departureTime, const QString &arrivalTime,
          const QString &aircraftType, const QString &seatClass, Status status,
          const QString &flightId, const QString &boardingGate, const QString &arrivalAirport,
          const QString &departureAirport, int firstClassSeats, int economyClassSeats,
          int businessClassSeats, double firstClassPrice, double economyClassPrice, double businessClassPrice);

    // Getter 方法
    QString orderNumber() const;
    QString passenger() const;
    QString amount() const;
    QString departure() const;
    QString destination() const;
    QString airline() const;
    QString flightNumber() const;
    QString departureTime() const;
    QString arrivalTime() const;
    QString aircraftType() const;
    QString seatClass() const;
    Status status() const;

    // 新增航班信息相关的 getter 方法
    QString flightId() const;
    QString boardingGate() const;
    QString arrivalAirport() const;
    QString departureAirport() const;
    int firstClassSeats() const;
    int economyClassSeats() const;
    int businessClassSeats() const;
    double firstClassPrice() const;
    double economyClassPrice() const;
    double businessClassPrice() const;

public:
    QString m_orderNumber;
    QString m_passenger;
    QString m_amount;
    QString m_departure;
    QString m_destination;
    QString m_airline;
    QString m_flightNumber;
    QString m_departureTime;
    QString m_arrivalTime;
    QString m_aircraftType;
    QString m_seatClass;
    Status m_status;

    // 新增的航班信息字段
    QString m_flightId;
    QString m_boardingGate;
    QString m_arrivalAirport;
    QString m_departureAirport;
    int m_firstClassSeats;
    int m_economyClassSeats;
    int m_businessClassSeats;
    double m_firstClassPrice;
    double m_economyClassPrice;
    double m_businessClassPrice;
};

#endif // ORDER_H
