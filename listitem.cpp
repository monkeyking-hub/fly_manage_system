#include "listitem.h"
#include "ui_listitem.h"
#include "flightinfo.h"

listItem::listItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::listItem)
{
    ui->setupUi(this);
}

QPixmap listItem:: getPic(const QString &name)
{
    QPixmap airlineLogo;
    if(name=="四川航空") airlineLogo.load("://sc.png");
    if(name=="厦门航空") airlineLogo.load("://xm.png");
    if(name=="深圳航空") airlineLogo.load("://sh.png");
    if(name=="山东航空") airlineLogo.load("://sdh.png");
    if(name=="南方航空") airlineLogo.load("://nh.jpg");
    if(name=="昆明航空") airlineLogo.load("://km.png");
    if(name=="中国联合航空") airlineLogo.load("://lh.png");
    if(name=="中国国航") airlineLogo.load("://gh.png");
    if(name=="海南航空") airlineLogo.load("://hnhk.png");
    if(name=="东方航空") airlineLogo.load("://df.png");
    if(name=="吉祥航空") airlineLogo.load("://jx.png");
    if(name=="华夏航空") airlineLogo.load("://hxh.png");
    if(name=="长龙航空") airlineLogo.load("://cl.png");
    if(name=="重庆航空") airlineLogo.load("://cq.png");
    if(name=="西藏航空") airlineLogo.load("://xz.png");
    if(name=="成都航空") airlineLogo.load("://cd.png");
    if(name=="海航旗下西部航") airlineLogo.load("://hnhk.png");
    if(name=="海航旗下天津航") airlineLogo.load("://tj.png");
    if(name=="龙江航空") airlineLogo.load("://lj.png");
    if(name=="苏南瑞丽航空") airlineLogo.load("://rl.png");
    if(name=="河北航空") airlineLogo.load("://hb.png");
    if(name=="海航旗下北部湾") airlineLogo.load("://bbw.png");
    if(name=="海航旗下福航") airlineLogo.load("://fh.png");
    if(name=="海航旗下乌航") airlineLogo.load("://wh.png");
    if(name=="海航旗下金鹏航空") airlineLogo.load("://jp.png");
    if(name=="海航旗下祥鹏航空") airlineLogo.load("://xp.png");
    if(name=="青岛航空") airlineLogo.load("://qd.png");
    if(name=="上海航空") airlineLogo.load("://sh.png");
    if(name=="海航旗下首都航空") airlineLogo.load("://sd.png");
    if(name=="海航旗下长安航空") airlineLogo.load("://cahk.png");
    if(name=="湖南航空") airlineLogo.load("://hunan.png");
    airlineLogo = airlineLogo.scaled(QSize(51, 51), Qt::KeepAspectRatio);
    return airlineLogo;
}

void listItem::setFlightDetails(const flightInfo &flight)
{
    // 转换时间戳为可读格式
    QDateTime departureDateTime = QDateTime::fromSecsSinceEpoch(flight.departureTime);
    QDateTime arrivalDateTime = QDateTime::fromSecsSinceEpoch(flight.arrivalTime);

    // 设置航空公司商标图片
    QPixmap airlineLogo = getPic(flight.airlineCompany);
    ui->lbl_compPic->setPixmap(airlineLogo);

    // 设置出发时间和抵达时间，仅显示小时和分钟
    ui->lbl_deparTime->setText(departureDateTime.toString("hh:mm"));
    ui->lbl_arrTime->setText(arrivalDateTime.toString("hh:mm"));

    // 计算不为零的最小价格
    double minPrice = std::numeric_limits<double>::max();  // 初始设置为最大值
    if (flight.economyClassPrice > 0)
        minPrice = std::min(minPrice, flight.economyClassPrice);
    if (flight.businessClassPrice > 0)
        minPrice = std::min(minPrice, flight.businessClassPrice);
    if (flight.firstClassPrice > 0)
        minPrice = std::min(minPrice, flight.firstClassPrice);

    // 如果所有价格都为0，可能需要一个默认值或错误处理
    if (minPrice == std::numeric_limits<double>::max())
        minPrice = 0; // 或其他逻辑

    // 设置价格标签（仅显示整数部分）
    ui->lbl_price->setText("¥" + QString::number(static_cast<int>(minPrice)));

    // 设置其余标签的文本
    ui->lbl_compName->setText(flight.airlineCompany);
    ui->lbl_dpAirPot->setText(flight.departureAirport);
    ui->lbl_desAirPot->setText(flight.arrivalAirport);
    ui->lbl_fliNo->setText(flight.flightNumber);
    ui->lbl_fliCate->setText(flight.aircraftModel);
}



listItem::~listItem()
{
    delete ui;
}
