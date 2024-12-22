#ifndef LISTITEM_H
#define LISTITEM_H
#include "flightinfo.h"
#include <QWidget>

namespace Ui {
class listItem;
}

class listItem : public QWidget
{
    Q_OBJECT

public:
    explicit listItem(QWidget *parent = nullptr);
    void setFlightDetails(const flightInfo &flight);
    ~listItem();
    QSize sizeHint() const {
        return QSize(1051, 72);  // 根据需要调整宽度和高度
    }
    QPixmap getPic(const QString &name);


private:
    Ui::listItem *ui;
};

#endif // LISTITEM_H
