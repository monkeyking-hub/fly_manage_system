#include "pay_window.h"
#include "ui_pay_window.h"
#include "boarderitem.h"
#include "interfacemanager.h"
#include "flightinfo.h"
#include "listitem.h"
#include "usermanager.h"
#include <QListWidgetItem>
#include <QListWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QMessageBox>

pay_window::pay_window(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::pay_window)
{
    ui->setupUi(this);
    ui->btn_f1->setVisible(false);
    ui->btn_f2->setVisible(false);
    ui->btn_f3->setVisible(false);
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
    emit countChanged();
}

void pay_window:: loadFliInfo()
{
    on_pushButton_clicked();
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
    if(fli.chosenSeat=="经济舱") price=fli.economyClassPrice*ui->listWidget->count();
    else if(fli.chosenSeat=="头等舱") price=fli.firstClassPrice*ui->listWidget->count();
    ui->lbl_price->setText("¥" + QString::number(static_cast<int>(price)));
    ui->lbl_seatCate->setText(fli.chosenSeat);
    connect(this,&pay_window::countChanged,this,[this,fli](){
        if(fli.chosenSeat=="经济舱") price+=fli.economyClassPrice;
        else if(fli.chosenSeat=="头等舱") price+=fli.firstClassPrice;
        ui->lbl_price->setText("¥" + QString::number(static_cast<int>(price)));
    });
    connect(this,&pay_window::itemCountChanged,this,[this,fli](){
        ui->lbl_price->setText("¥" + QString::number(static_cast<int>(price)));
    });
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


void pay_window::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget_2->setCurrentIndex(1);
}


void pay_window::on_btn_1_clicked()
{
    ui->btn_1->setVisible(false);
    ui->btn_f1->setVisible(true);
    price+=40*ui->listWidget->count();
    emit itemCountChanged();
}


void pay_window::on_btn_2_clicked()
{
    ui->btn_2->setVisible(false);
    ui->btn_f2->setVisible(true);
    price+=39*ui->listWidget->count();
    emit itemCountChanged();
}


void pay_window::on_btn_3_clicked()
{
    ui->btn_3->setVisible(false);
    ui->btn_f3->setVisible(true);
    price+=75*ui->listWidget->count();
    emit itemCountChanged();
}


void pay_window::on_btn_f1_clicked()
{
    ui->btn_f1->setVisible(false);
    ui->btn_1->setVisible(true);
    price-=40*ui->listWidget->count();
    emit itemCountChanged();
}


void pay_window::on_btn_f2_clicked()
{
    ui->btn_f2->setVisible(false);
    ui->btn_2->setVisible(true);
    price-=39*ui->listWidget->count();
    emit itemCountChanged();
}


void pay_window::on_btn_f3_clicked()
{
    ui->btn_f3->setVisible(false);
    ui->btn_3->setVisible(true);
    price-=75*ui->listWidget->count();
    emit itemCountChanged();
}


void pay_window::on_pushButton_8_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->stackedWidget_2->setCurrentIndex(2);
}


void pay_window::on_pushButton_3_clicked()
{
    QNetworkAccessManager* networkmanager = new QNetworkAccessManager();
    QNetworkRequest request(QUrl("http://127.0.0.1:8080/api/orders/create"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    User currentUser = UserManager::getInstance()->getCurrentUser();
    json["userId"] = UserManager::getInstance()->getCurrentUser().id;
    json["flightId"] = InterfaceManager::instance()->Info.id;
    json["price"] = price; // 确保为浮点数
    json["seatType"] =InterfaceManager::instance()->Info.chosenSeat; // 改为中文

    QJsonDocument doc(json);
    qDebug() << "Request Body:" << doc.toJson();

    QNetworkReply* reply = networkmanager->post(request, doc.toJson());
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        QByteArray responseData = reply->readAll();
        qDebug() << "Response:" << responseData;

        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObject = jsonResponse.object();
            if (jsonObject["code"].toInt() == 200) {
                QMessageBox msgBox(this);
                msgBox.setWindowTitle("Success");
                msgBox.setText("订单添加成功 !");
                msgBox.setIcon(QMessageBox::Information);
                msgBox.exec();
            } else {
                QMessageBox msgBox(this);
                msgBox.setWindowTitle("Failed");
                msgBox.setText("订单添加失败: " + jsonObject["message"].toString());
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.exec();
            }
        } else {
            qDebug() << "Network Error:" << reply->errorString();
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("Failed");
            msgBox.setText("订单添加系统连接失败 !");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
        }
        reply->deleteLater();
    });
}



void pay_window::on_pushButton_4_clicked()
{
    int now_index=ui->stackedWidget->currentIndex();
    if(now_index != 0)
    {
        ui->stackedWidget->setCurrentIndex(now_index-1);
        ui->stackedWidget_2->setCurrentIndex(now_index-1);
    }
    else
    {
        InterfaceManager::instance()->switchToPage("lxt_mainInterface");
    }
}

