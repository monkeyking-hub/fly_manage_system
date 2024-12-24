#ifndef ORDERWINDOW_H
#define ORDERWINDOW_H

#include <QComboBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QMainWindow>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTabBar> // 引入 QTabBar 类
#include <QVBoxLayout>
#include <QWidget>
#include "order.h"
#include "orderdetailwindow.h"
#include <QComboBox>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QPixmap>
#include <QIcon>
#include <QPainter>
#include <QStyledItemDelegate>
#include <icondelegate.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class order_2;
}
QT_END_NAMESPACE

class orderwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit orderwindow(QWidget *parent = nullptr);
    ~orderwindow();

    // 创建订单页面
    QWidget *createOrderPage(const QString &type);

    // Tab 切换时处理
    void onTabChanged(int index);

    // 显示订单详情
    void showOrderDetails(const Order &order);

    void fetchOrders(int userId);
    //创建每个订单小窗口
    void createOrderWidget(const Order &order, QVBoxLayout *containerLayout);

    void updateOrderPage(const QString &type,
                         QVBoxLayout *pageLayout,
                         QComboBox *departureComboBox,
                         QComboBox *destinationComboBox,
                         QComboBox *airlineComboBox);

    // 获取航空公司图标的函数
    QPixmap getPic(const QString &name)
    {
        QPixmap airlineLogo;
        if (name == "四川航空") airlineLogo.load("://sc.png");
        if (name == "厦门航空") airlineLogo.load("://xm.png");
        if (name == "深圳航空") airlineLogo.load("://sh.png");
        if (name == "山东航空") airlineLogo.load("://sdh.png");
        if (name == "南方航空") airlineLogo.load("://nh.jpg");
        if (name == "昆明航空") airlineLogo.load("://km.png");
        if (name == "中国联合航空") airlineLogo.load("://lh.png");
        if (name == "中国国航") airlineLogo.load("://gh.png");
        if (name == "海南航空") airlineLogo.load("://hnhk.png");
        if (name == "东方航空") airlineLogo.load("://df.png");
        if (name == "吉祥航空") airlineLogo.load("://jx.png");
        if (name == "华夏航空") airlineLogo.load("://hxh.png");
        if (name == "长龙航空") airlineLogo.load("://cl.png");
        if (name == "重庆航空") airlineLogo.load("://cq.png");
        if (name == "西藏航空") airlineLogo.load("://xz.png");
        if (name == "成都航空") airlineLogo.load("://cd.png");
        if (name == "海航旗下西部航") airlineLogo.load("://hnhk.png");
        if (name == "海航旗下天津航") airlineLogo.load("://tj.png");
        if (name == "龙江航空") airlineLogo.load("://lj.png");
        if (name == "苏南瑞丽航空") airlineLogo.load("://rl.png");
        if (name == "河北航空") airlineLogo.load("://hb.png");
        if (name == "海航旗下北部湾") airlineLogo.load("://bbw.png");
        if (name == "海航旗下福航") airlineLogo.load("://fh.png");
        if (name == "海航旗下乌航") airlineLogo.load("://wh.png");
        if (name == "海航旗下金鹏航空") airlineLogo.load("://jp.png");
        if (name == "海航旗下祥鹏航空") airlineLogo.load("://xp.png");
        if (name == "青岛航空") airlineLogo.load("://qd.png");
        if (name == "上海航空") airlineLogo.load("://sh.png");
        if (name == "海航旗下首都航空") airlineLogo.load("://sd.png");
        if (name == "海航旗下长安航空") airlineLogo.load("://cahk.png");
        if (name == "湖南航空") airlineLogo.load("://hunan.png");

        airlineLogo = airlineLogo.scaled(QSize(51, 51), Qt::KeepAspectRatio);
        return airlineLogo;
    }


    // 设置 QComboBox 显示图片和文本
    void setupAirlineComboBox(QComboBox *comboBox, const QStringList &airlines)
    {
        // 创建一个 QStandardItemModel 来存放图像和文本
        QStandardItemModel *model = new QStandardItemModel(this);

        // 添加 "所有航空公司" 选项
        QStandardItem *item = new QStandardItem();
        QPixmap pixmap("://0.png"); // 获取对应的图像
        pixmap = pixmap.scaled(40, 40, Qt::KeepAspectRatio);  // 放大图标大小
        item->setIcon(QIcon(pixmap));      // 设置图标
        item->setText("所有航空公司");    // 设置文本
        item->setSizeHint(QSize(150, 60)); // 设置项的高度和宽度
        model->appendRow(item);

        // 为每个航空公司添加项（带图像和名称）
        for (const QString &airline : airlines) {
            QStandardItem *item = new QStandardItem();
            QPixmap pixmap = getPic(airline);  // 获取对应的图像
            pixmap = pixmap.scaled(40, 40, Qt::KeepAspectRatio);  // 放大图标大小
            item->setIcon(QIcon(pixmap));      // 设置图标
            item->setText(airline);            // 设置文本
            item->setSizeHint(QSize(150, 60)); // 设置项的高度和宽度
            model->appendRow(item);            // 将项添加到模型中
        }

        // 设置 QComboBox 的模型
        comboBox->setModel(model);

        // 设置 QComboBox 的自定义代理
        comboBox->setItemDelegate(new IconDelegate(this));  // 设置自定义的项代理
    }



private:
    Ui::order_2 *ui;
    QString m_passengerName;
    QTabWidget *tabWidget;

    QPushButton *refreshButton;  // 添加刷新按钮

public slots:
    void onRefreshButtonClicked();
};

#endif // ORDERWINDOW_H
