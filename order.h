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
          const QString &aircraftType, const QString &seatClass, Status status);

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

private:
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
};

#endif // ORDER_H
