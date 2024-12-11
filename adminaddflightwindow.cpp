#include "adminAddFlightWindow.h"
#include "ui_adminAddFlightWindow.h"
#include <QCalendarWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include<QMessageBox>
adminAddFlightWindow::adminAddFlightWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::adminAddFlightWindow)
{
    ui->setupUi(this);

    ui->calendarWidget->hide();
    ui->lineEdit_date1->setPlaceholderText("请选择日期");
    ui->lineEdit_date2->setPlaceholderText("请选择日期");

    // 加载图片
    QPixmap pixmap_position(":/position.png"); //起点和终点的图片
    QPixmap pixmap_date(":/date.png"); //出发时间和到达时间的图片
    QPixmap pixmap_flight(":/flightNumber.png"); //航班编号和飞机型号的图片
    QPixmap pixmap_company(":/company.png"); //航空公司的图片
    QPixmap pixmap_map(":/map.png"); //中国地图的图片

    // 调整图片大小以适应 QLabel 的尺寸
    QPixmap scaledPixmap1 = pixmap_position.scaled(ui->img_departure->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledPixmap2 = pixmap_position.scaled(ui->img_destination->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledPixmap3 = pixmap_date.scaled(ui->img_date1->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledPixmap4 = pixmap_date.scaled(ui->img_date2->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledPixmap5 = pixmap_flight.scaled(ui->img_flightNumber->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledPixmap6 = pixmap_flight.scaled(ui->img_aircraftModel->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledPixmap7 = pixmap_company.scaled(ui->img_airlineCompany->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledPixmap8 = pixmap_map.scaled(ui->img_map->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // 将调整后的图片设置为 QLabel 的内容
    ui->img_departure->setPixmap(scaledPixmap1);
    ui->img_destination->setPixmap(scaledPixmap2);
    ui->img_date1->setPixmap(scaledPixmap3);
    ui->img_date2->setPixmap(scaledPixmap4);
    ui->img_flightNumber->setPixmap(scaledPixmap5);
    ui->img_aircraftModel->setPixmap(scaledPixmap6);
    ui->img_airlineCompany->setPixmap(scaledPixmap7);
    ui->img_map->setPixmap(scaledPixmap8);

    connect(ui->btn_selectDate1, &QPushButton::clicked, this, [this]() {
        showCalendar(ui->btn_selectDate1);
    });
    connect(ui->btn_selectDate2, &QPushButton::clicked, this, [this]() {
        showCalendar(ui->btn_selectDate2);
    });
    connect(ui->calendarWidget, &QCalendarWidget::clicked, this, &adminAddFlightWindow::selectDate);

    connect(ui->btn_add,&QPushButton::clicked,this,&adminAddFlightWindow::onAddFlightBtnClicked);
}

void adminAddFlightWindow::showCalendar(QPushButton *button)
{
    selectedBtn = button; // 标记哪个按钮被点击
    ui->calendarWidget->setHidden(false); // 显示日历控件
}

void adminAddFlightWindow::selectDate(const QDate &date)
{
    if (selectedBtn == ui->btn_selectDate1) {
        ui->lineEdit_date1->setText(date.toString("yyyy-MM-dd"));
    } else if (selectedBtn == ui->btn_selectDate2) {
        ui->lineEdit_date2->setText(date.toString("yyyy-MM-dd"));
    }
    ui->calendarWidget->setHidden(true); // 选择日期后隐藏日历控件
}

void adminAddFlightWindow::onAddFlightBtnClicked()
{
    QString flightNumber = ui->lineEdit_flightNumber->text();
    QString aircraftModel = ui->lineEdit_aircraftModel->text();
    QString departure = ui->lineEdit_departure->text();
    QString destination = ui->lineEdit_destination->text();

    QString departure_date=ui->lineEdit_date1->text();
    QString departure_time=ui->dateTimeEdit1->time().toString("HH:mm");
    QString dateTimeString1 = departure_date+" "+departure_time;
    QDateTime combinedDateTime1=QDateTime::fromString(dateTimeString1,"yyyy-MM-dd HH:mm");
    qint64 unixTimeStamp1 = combinedDateTime1.toSecsSinceEpoch();

    QString arrival_date=ui->lineEdit_date2->text();
    QString arrival_time=ui->dateTimeEdit2->time().toString("HH:mm");
    QString dateTimeString2 = arrival_date+" "+arrival_time;
    QDateTime combinedDateTime2=QDateTime::fromString(dateTimeString2,"yyyy-MM-dd HH:mm");
    qint64 unixTimeStamp2 = combinedDateTime2.toSecsSinceEpoch();

    int firstClassSeats = ui->spinBox_firstClassSeat->value();
    int economyClassSeats = ui->spinBox_economyClassSeat->value();
    int firstClassPrice = ui->spinBox_firstClassPrice->value();
    int economyClassPrice = ui->spinBox_economyClassPrice->value();
    QString company = ui->lineEdit_airlineCompany->text();

    //检查是否填了全部内容
    if(flightNumber.contains(" ") || aircraftModel.contains(" ") || departure.contains(" ") || destination.contains(" ") || departure_date.contains(" ") || arrival_date.contains(" ") ||company.contains(" "))
    {
        QMessageBox::critical(nullptr, "添加失败",
                              "添加失败: 输入的内容不能含有空格！",
                              QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    //检查是否填了全部内容
    if(flightNumber=="" || aircraftModel=="" || departure=="" || destination=="" || departure_date=="" || arrival_date=="" ||company=="")
    {
        QMessageBox::critical(nullptr, "添加失败",
                              "添加失败: 您需要填写全部信息！",
                              QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    //检查出发时间是否在到达时间之前
    if(unixTimeStamp1>=unixTimeStamp2)
    {
        QMessageBox::critical(nullptr, "添加失败",
                              "添加失败: 出发时间必须在到达时间之前",
                              QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    //检查座舱价格和数量是否为0
    if(firstClassSeats==0)
    {
        QMessageBox::critical(nullptr, "添加失败",
                              "添加失败: 头等舱数量不能为0！",
                              QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if(firstClassPrice==0)
    {
        QMessageBox::critical(nullptr, "添加失败",
                              "添加失败: 头等舱价格不能为0！",
                              QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if(economyClassSeats==0)
    {
        QMessageBox::critical(nullptr, "添加失败",
                              "添加失败: 经济舱数量不能为0！",
                              QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if(economyClassPrice==0)
    {
        QMessageBox::critical(nullptr, "添加失败",
                              "添加失败: 经济舱价格不能为0！",
                              QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    emit addFlightRequested(flightNumber,aircraftModel,departure,destination,unixTimeStamp1,unixTimeStamp2,firstClassSeats,economyClassSeats,firstClassPrice,economyClassPrice,company);
}

adminAddFlightWindow::~adminAddFlightWindow()
{
    delete ui;
}

void addFlightHandler::handleAddFlight(const QString &flightNumber,const QString& aircraftModel,const QString& departure,
                                       const QString &destination,qint64 departureTime,qint64 arrivalTime,
                                       const int &firstClassSeats,const int &economyClassSeats,const int &firstClassPrice,
                                       const int &economyClassPrice,const QString &company)
{
    // // 创建网络管理器
    QNetworkAccessManager* manager = new QNetworkAccessManager();

    // // 设置请求 URL
    QUrl url("http://localhost:8080/api/flights/create");
    QNetworkRequest request(url);

    // // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // // 构建 JSON 请求体
    QJsonObject json;
    json["flightNumber"]=flightNumber;
    json["departure"] = departure;
    json["destination"] =destination;
    json["departureTime"] = departureTime;
    json["arrivalTime"] = arrivalTime;
    json["firstClassSeats"] = firstClassSeats;
    json["economyClassSeats"] = economyClassSeats;
    json["firstClassPrice"] = firstClassPrice;
    json["economyClassPrice"] = economyClassPrice;
    json["airlineCompany"] = company;
    json["aircraftModel"] = aircraftModel;


    QJsonDocument jsonDoc(json);
    QByteArray requestData = jsonDoc.toJson();

    // // 发送 POST 请求
    QNetworkReply* reply = manager->post(request, requestData);
    // 连接信号，等待响应
    connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            // 请求成功，读取响应数据
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject responseObject = jsonResponse.object();

            // 解析响应 JSON
            int code = responseObject["code"].toInt();  // 获取返回的 code
            QString message = responseObject["message"].toString();
            QJsonObject data = responseObject["data"].toObject();

            if (code == 200) {
                // 添加成功,弹出信息框
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Information);  // 设置图标类型
                msgBox.setWindowTitle("添加成功");         // 设置窗口标题
                msgBox.setText("您已成功添加航班信息！");          // 设置提示文本
                msgBox.setStandardButtons(QMessageBox::Ok); // 设置标准按钮（此处只有“确定”按钮）
                // 显示消息框
                msgBox.exec();
            } else {
                // 注册失败，弹出错误提示框
                qDebug() << "Add failed, code: " << code;
                QMessageBox::critical(nullptr, "Add Error",
                                      "Add failed: " + message,
                                      QMessageBox::Ok, QMessageBox::Ok);
            }
        } else {
            // 请求失败，弹出错误提示框
            QString errorString = reply->errorString();
            qDebug() << "Error:" << errorString;

            // 创建 QMessageBox 来显示错误信息
            QMessageBox::critical(nullptr, "Add Error",
                                  "Request failed: " + errorString,
                                  QMessageBox::Ok, QMessageBox::Ok);
        }

        reply->deleteLater(); // 释放资源
    });
}


