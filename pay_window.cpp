#include "pay_window.h"
#include "ui_pay_window.h"
#include "boarderitem.h"
#include "interfacemanager.h"
#include "flightinfo.h"
#include "listitem.h"
#include <QListWidgetItem>
#include <QListWidget>

pay_window::pay_window(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::pay_window)
{
    ui->setupUi(this);
}

pay_window::~pay_window()
{
    delete ui;
}

void pay_window::on_pushButton_clicked()
{
    boarderItem* li = new boarderItem();
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget); // 假设你的列表控件叫 listWidget
    item->setSizeHint(li->sizeHint());
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, li);
}

void pay_window:: loadFliInfo()
{
    flightInfo fli= InterfaceManager::instance()->Info;
    ui->lbl_compName->setText(fli.airlineCompany);
    const QPixmap airlogo=getPic(fli.airlineCompany);
    ui->lbl_compPic->setPixmap(airlogo);
    ui->lbl_depar->setText(fli.departure);
    ui->lbl_des->setText(fli.destination);
    ui->lbl_flightCate->setText(fli.aircraftModel);
    ui->lbl_flightNo->setText(fli.flightNumber);
    QDateTime departureDateTime = QDateTime::fromSecsSinceEpoch(fli.departureTime);
    QDateTime arrivalDateTime = QDateTime::fromSecsSinceEpoch(fli.arrivalTime);
    ui->lbl_deparTime->setText(departureDateTime.toString("hh:mm"));
    ui->lbl_arrivalTime->setText(arrivalDateTime.toString("hh:mm"));
    QLocale chineseLocale(QLocale::Chinese, QLocale::China);
    QString dayofweek=chineseLocale.toString(departureDateTime, "dddd");
    ui->lbl_date->setText(departureDateTime.toString("MM-dd")+" "+dayofweek);
    ui->lbl_deparAirport->setText(fli.departureAirport);
    ui->lbl_arrivalAirport->setText(fli.arrivalAirport);
    ui->lbl_price->setText("¥" + QString::number(static_cast<int>(fli.economyClassPrice)));
}

const QPixmap pay_window:: getPic(const QString &name)
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

