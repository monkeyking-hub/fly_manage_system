#include "order.h"

Order::Order()
    : m_orderNumber(""), m_passenger(""), m_bookingDate(""),
    m_amount(""), m_departure(""), m_destination(""), m_status(Pending) {}

Order::Order(const QString &orderNumber, const QString &passenger, const QString &bookingDate,
             const QString &amount, const QString &departure, const QString &destination, Status status)
    : m_orderNumber(orderNumber), m_passenger(passenger), m_bookingDate(bookingDate),
    m_amount(amount), m_departure(departure), m_destination(destination), m_status(status) {}

QString Order::orderNumber() const {
    return m_orderNumber;
}

QString Order::passenger() const {
    return m_passenger;
}

QString Order::bookingDate() const {
    return m_bookingDate;
}

QString Order::amount() const {
    return m_amount;
}

QString Order::route() const {
    return QString("%1 - %2").arg(m_departure).arg(m_destination);
}

Order::Status Order::status() const {
    return m_status;
}
