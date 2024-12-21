#include "order.h"

// 默认构造函数
Order::Order()
    : m_orderNumber(""), m_passenger(""), m_amount(""),
    m_departure(""), m_destination(""), m_airline(""),
    m_flightNumber(""), m_departureTime(""), m_arrivalTime(""),
    m_aircraftType(""), m_seatClass(""), m_status(Pending),
    m_flightId(""), m_boardingGate(""), m_arrivalAirport(""),
    m_departureAirport(""), m_firstClassSeats(0), m_economyClassSeats(0),
    m_businessClassSeats(0), m_firstClassPrice(0.0), m_economyClassPrice(0.0), m_businessClassPrice(0.0) {}

// 带参数的构造函数
Order::Order(const QString &orderNumber, const QString &passenger, const QString &amount,
             const QString &departure, const QString &destination, const QString &airline,
             const QString &flightNumber, const QString &departureTime, const QString &arrivalTime,
             const QString &aircraftType, const QString &seatClass, Status status,
             const QString &flightId, const QString &boardingGate, const QString &arrivalAirport,
             const QString &departureAirport, int firstClassSeats, int economyClassSeats,
             int businessClassSeats, double firstClassPrice, double economyClassPrice, double businessClassPrice)
    : m_orderNumber(orderNumber), m_passenger(passenger), m_amount(amount),
    m_departure(departure), m_destination(destination), m_airline(airline),
    m_flightNumber(flightNumber), m_departureTime(departureTime), m_arrivalTime(arrivalTime),
    m_aircraftType(aircraftType), m_seatClass(seatClass), m_status(status),
    m_flightId(flightId), m_boardingGate(boardingGate), m_arrivalAirport(arrivalAirport),
    m_departureAirport(departureAirport), m_firstClassSeats(firstClassSeats),
    m_economyClassSeats(economyClassSeats), m_businessClassSeats(businessClassSeats),
    m_firstClassPrice(firstClassPrice), m_economyClassPrice(economyClassPrice), m_businessClassPrice(businessClassPrice) {}

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

// 新增航班信息相关的 getter 方法
QString Order::flightId() const {
    return m_flightId;
}

QString Order::boardingGate() const {
    return m_boardingGate;
}

QString Order::arrivalAirport() const {
    return m_arrivalAirport;
}

QString Order::departureAirport() const {
    return m_departureAirport;
}

int Order::firstClassSeats() const {
    return m_firstClassSeats;
}

int Order::economyClassSeats() const {
    return m_economyClassSeats;
}

int Order::businessClassSeats() const {
    return m_businessClassSeats;
}

double Order::firstClassPrice() const {
    return m_firstClassPrice;
}

double Order::economyClassPrice() const {
    return m_economyClassPrice;
}

double Order::businessClassPrice() const {
    return m_businessClassPrice;
}
