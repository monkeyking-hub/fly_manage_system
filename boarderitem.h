#ifndef BOARDERITEM_H
#define BOARDERITEM_H

#include <QWidget>

namespace Ui {
class boarderItem;
}

class boarderItem : public QWidget
{
    Q_OBJECT

public:
    explicit boarderItem(QWidget *parent = nullptr);
    ~boarderItem();
    static int num;
    QSize sizeHint() const {
        return QSize(641, 245);  // 根据需要调整宽度和高度
    }
private:
    Ui::boarderItem *ui;
};

#endif // BOARDERITEM_H
