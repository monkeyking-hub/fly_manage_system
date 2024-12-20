#include "order.h"

// 默认构造函数
Order::Order()
    : m_orderNumber(""), m_passenger(""), m_amount(""),
    m_departure(""), m_destination(""), m_airline(""),
    m_flightNumber(""), m_departureTime(""), m_arrivalTime(""),
    m_aircraftType(""), m_seatClass(""), m_status(Pending) {}

// 带参数的构造函数
Order::Order(const QString &orderNumber, const QString &passenger, const QString &amount,
             const QString &departure, const QString &destination, const QString &airline,
             const QString &flightNumber, const QString &departureTime, const QString &arrivalTime,
             const QString &aircraftType, const QString &seatClass, Status status)
    : m_orderNumber(orderNumber), m_passenger(passenger), m_amount(amount),
    m_departure(departure), m_destination(destination), m_airline(airline),
    m_flightNumber(flightNumber), m_departureTime(departureTime), m_arrivalTime(arrivalTime),
    m_aircraftType(aircraftType), m_seatClass(seatClass), m_status(status) {}

// Getter 方法
QString Order::orderNumber() const {
    return m_orderNumber;
}

QString Order::passenger() const {
    return m_passenger;
}

QString Order::amount() const {
    return m_amount;
}

QString Order::departure() const {
    return m_departure;
}

QString Order::destination() const {
    return m_destination;
}

QString Order::airline() const {
    return m_airline;
}

QString Order::flightNumber() const {
    return m_flightNumber;
}

QString Order::departureTime() const {
    return m_departureTime;
}

QString Order::arrivalTime() const {
    return m_arrivalTime;
}

QString Order::aircraftType() const {
    return m_aircraftType;
}

QString Order::seatClass() const {
    return m_seatClass;
}

Order::Status Order::status() const {
    return m_status;
}
