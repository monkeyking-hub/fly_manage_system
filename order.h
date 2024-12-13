#ifndef ORDER_H
#define ORDER_H

#include <QString>

class Order
{
public:
    // 枚举类型：表示订单状态
    enum Status {
        Pending,  // 待支付
        Upcoming, // 待出行
        Canceled  // 已取消
    };

    // 默认构造函数
    Order();

    // 带参数构造函数
    Order(const QString &orderNumber, const QString &passenger, const QString &bookingDate,
          const QString &amount, const QString &departure, const QString &destination, Status status);

    // 获取各个属性
    QString orderNumber() const;
    QString passenger() const;
    QString bookingDate() const;
    QString amount() const;
    QString route() const;
    Status status() const;

private:
    QString m_orderNumber;
    QString m_passenger;
    QString m_bookingDate;
    QString m_amount;
    QString m_departure;
    QString m_destination;
    Status m_status;
};

#endif // ORDER_H
