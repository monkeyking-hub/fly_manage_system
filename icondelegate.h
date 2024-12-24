#ifndef ICONDELEGATE_H
#define ICONDELEGATE_H

#include <QStyledItemDelegate>
#include <QPixmap>
#include <QPainter>
#include <QIcon>
#include <QStyleOptionViewItem>
#include <QModelIndex>

class IconDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    // 构造函数
    explicit IconDelegate(QObject *parent = nullptr);

    // 重写 paint 方法，控制图标的绘制
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    // 其他成员函数（如果需要可以扩展）
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // ICONDELEGATE_H
