#include "IconDelegate.h"
#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QFontMetrics>

IconDelegate::IconDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void IconDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // 初始化样式选项
    QStyleOptionViewItem newOption(option);
    initStyleOption(&newOption, index);

    // 检查是否是悬停状态
    bool isHovered = newOption.state & QStyle::State_MouseOver;

    // 绘制背景色，如果是悬停状态，设置背景色为蓝色
    if (isHovered) {
        painter->fillRect(newOption.rect, QColor(173, 216, 230));  // 淡蓝色背景
    }

    // 获取图标和文本
    QIcon icon = newOption.icon;
    QString text = newOption.text;

    // 获取图标的原始图像，并调整大小
    QPixmap pixmap = icon.pixmap(40, 40);  // 设置图标的大小为 40x40

    // 计算绘制区域
    QRect rect = newOption.rect;

    // 设置一个适当的边距，确保图标和文本不重叠
    int padding = 5;
    int iconWidth = pixmap.width();
    int iconHeight = pixmap.height();

    // 设置图标绘制的位置
    QRect iconRect(rect.left() + padding, rect.top() + (rect.height() - iconHeight) / 2, iconWidth, iconHeight);

    // 绘制图标
    painter->drawPixmap(iconRect, pixmap);

    // 设置文本绘制位置，文本的位置应该在图标的右边
    QRect textRect(rect.left() + iconRect.width() + padding * 2, rect.top(), rect.width() - iconRect.width() - padding * 3, rect.height());

    // 设置文本颜色和字体
    painter->setPen(option.palette.color(QPalette::Text));
    painter->setFont(option.font);

    // 绘制文本
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);
}

QSize IconDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);

    // 为了确保图标和文本不重叠，我们增加高度和宽度
    size.setHeight(qMax(size.height(), 50));  // 设置最小高度为 50
    size.setWidth(size.width() + 50);  // 添加图标和文本的宽度

    return size;
}
