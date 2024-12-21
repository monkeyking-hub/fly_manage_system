#include "adminupdateflightwindow.h"
#include "ui_adminupdateflightwindow.h"
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

adminUpdateFlightWindow::adminUpdateFlightWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::adminUpdateFlightWindow)
{
    ui->setupUi(this);

    ui->calendarWidget->hide();
    ui->lineEdit_date1->setPlaceholderText("请选择日期");
    ui->lineEdit_date2->setPlaceholderText("请选择日期");

    ui->lineEdit_search->setPlaceholderText("请输入航班ID进行查找");

    // 加载图片
    QPixmap pixmap_position(":/position.png"); //起点和终点的图片
    QPixmap pixmap_date(":/date.png"); //出发时间和到达时间的图片
    QPixmap pixmap_flight(":/flightNumber.png"); //航班编号和飞机型号的图片
    QPixmap pixmap_company(":/company.png"); //航空公司的图片
    QPixmap pixmap_search(":/imgforsearch.jpg"); //搜索框上方的图片

    // 调整图片大小以适应 QLabel 的尺寸
    QPixmap scaledPixmap1 = pixmap_position.scaled(ui->img_departure->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledPixmap2 = pixmap_position.scaled(ui->img_destination->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledPixmap3 = pixmap_date.scaled(ui->img_date1->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledPixmap4 = pixmap_date.scaled(ui->img_date2->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledPixmap5 = pixmap_flight.scaled(ui->img_flightNumber->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledPixmap6 = pixmap_flight.scaled(ui->img_aircraftModel->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledPixmap7 = pixmap_company.scaled(ui->img_airlineCompany->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledPixmap8 = pixmap_search.scaled(ui->label_img->size());

    // 将调整后的图片设置为 QLabel 的内容
    ui->img_departure->setPixmap(scaledPixmap1);
    ui->img_destination->setPixmap(scaledPixmap2);
    ui->img_date1->setPixmap(scaledPixmap3);
    ui->img_date2->setPixmap(scaledPixmap4);
    ui->img_flightNumber->setPixmap(scaledPixmap5);
    ui->img_aircraftModel->setPixmap(scaledPixmap6);
    ui->img_airlineCompany->setPixmap(scaledPixmap7);
    ui->label_img->setPixmap(scaledPixmap8);

    QString styleSheet = R"(
    QLineEdit {
        background-color: #F5F5F5;  /* 默认浅灰色背景 */
        border: 2px solid #D3D3D3; /* 默认浅灰色边框 */
        border-radius: 8px;        /* 圆角边框 */
        padding: 6px;             /* 内边距 */
        font-size: 14px;          /* 字体大小 */
        color: #333333;           /* 默认深灰色字体 */
    }
    QLineEdit:hover {
        border: 2px solid #A6A6A6; /* 鼠标悬浮时边框颜色 */
        background-color: #FFFFFF; /* 鼠标悬浮时背景颜色 */
    }
    QLineEdit:focus {
        border: 2px solid #5B9BD5; /* 聚焦时的边框颜色 */
        background-color: #FFFFFF; /* 聚焦时的背景颜色 */
    }
    )";
    ui->lineEdit_search->setStyleSheet(styleSheet);

    connect(ui->btn_selectDate1, &QPushButton::clicked, this, [this]() {
        showCalendar(ui->btn_selectDate1);
    });
    connect(ui->btn_selectDate2, &QPushButton::clicked, this, [this]() {
        showCalendar(ui->btn_selectDate2);
    });
    connect(ui->calendarWidget, &QCalendarWidget::clicked, this, &adminUpdateFlightWindow::selectDate);
    connect(ui->btn_search,&QPushButton::clicked,this,&adminUpdateFlightWindow::onSearchButtonClicked);
    connect(ui->btn_cancel,&QPushButton::clicked,this,&adminUpdateFlightWindow::onCancelButtonClicked);
    connect(ui->btn_save,&QPushButton::clicked,this,&adminUpdateFlightWindow::onSaveButtonClicked);
}

void adminUpdateFlightWindow::showCalendar(QPushButton *button)
{
    selectedBtn = button; // 标记哪个按钮被点击
    ui->calendarWidget->setHidden(false); // 显示日历控件
}

void adminUpdateFlightWindow::selectDate(const QDate &date)
{
    if (selectedBtn == ui->btn_selectDate1) {
        ui->lineEdit_date1->setText(date.toString("yyyy-MM-dd"));
    } else if (selectedBtn == ui->btn_selectDate2) {
        ui->lineEdit_date2->setText(date.toString("yyyy-MM-dd"));
    }
    ui->calendarWidget->setHidden(true); // 选择日期后隐藏日历控件
}

void adminUpdateFlightWindow::onSearchButtonClicked()
{
    if(ui->lineEdit_search->text()=="" || ui->lineEdit_search->text().contains(" "))
    {
        QMessageBox::critical(nullptr, "查找失败",
                              "查找失败: 请输入有效的航班ID！",
                              QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    QString id_string=ui->lineEdit_search->text();
    int id=id_string.toInt();
    this->id=id;

    // // 创建网络管理器
    QNetworkAccessManager* manager = new QNetworkAccessManager();

    // // 设置请求 URL
    QUrl url("http://localhost:8080/api/flights/queryById");
    QNetworkRequest request(url);

    // // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // // 构建 JSON 请求体
    QJsonObject json;
    json["id"]=id;

    QJsonDocument jsonDoc(json);
    QByteArray requestData = jsonDoc.toJson();

    // // 发送 POST 请求
    QNetworkReply* reply = manager->post(request, requestData);
    // 连接信号，等待响应
    connect(reply, &QNetworkReply::finished, [reply,this]() {
        if (reply->error() == QNetworkReply::NoError) {
            // 请求成功，读取响应数据
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject responseObject = jsonResponse.object();

            // 解析响应 JSON
            int code = responseObject["code"].toInt();  // 获取返回的 code
            QString message = responseObject["message"].toString();
            QJsonObject data = responseObject["data"].toObject();

            if (code == 200 && data["flightNumber"].toString()!="") {
                // 添加成功,弹出信息框
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Information);  // 设置图标类型
                msgBox.setWindowTitle("成功");         // 设置窗口标题
                msgBox.setText("查找成功！");          // 设置提示文本
                msgBox.setStandardButtons(QMessageBox::Ok); // 设置标准按钮（此处只有“确定”按钮）
                // 显示消息框
                msgBox.exec();

                QString flightNumber = data["flightNumber"].toString();
                QString aircraftModel = data["aircraftModel"].toString();
                QString departure= data["departure"].toString();
                QString destination = data["destination"].toString();
                qint64 timestamp_departure = data["departureTime"].toVariant().toLongLong();
                qint64 timestamp_arrival = data["arrivalTime"].toVariant().toLongLong();
                QString airlineCompany = data["airlineCompany"].toString();
                int firstClassSeats = data["firstClassSeats"].toInt();
                int economyClassSeats = data["economyClassSeats"].toInt();
                int firstClassPrice = data["firstClassPrice"].toInt()/100;
                int economyClassPrice = data["economyClassPrice"].toInt()/100;

                //提取小时分钟
                QDateTime dateTime_departure = QDateTime::fromSecsSinceEpoch(timestamp_departure);
                QDateTime dateTime_arrival = QDateTime::fromSecsSinceEpoch(timestamp_arrival);
                QTime departureTime = dateTime_departure.time();
                QTime arrivalTime = dateTime_arrival.time();

                //提取年月日
                QString datestr_departure=dateTime_departure.date().toString("yyyy-MM-dd");
                QString datestr_arrival=dateTime_arrival.date().toString("yyyy-MM-dd");

                //将获取的内容填到输入框中
                ui->lineEdit_flightNumber->setText(flightNumber);
                ui->lineEdit_aircraftModel->setText(aircraftModel);
                ui->lineEdit_departure->setText(departure);
                ui->lineEdit_destination->setText(destination);
                ui->lineEdit_date1->setText(datestr_departure);
                ui->lineEdit_date2->setText(datestr_arrival);
                ui->dateTimeEdit1->setTime(departureTime);
                ui->dateTimeEdit2->setTime(arrivalTime);
                ui->spinBox_firstClassSeat->setValue(firstClassSeats);
                ui->spinBox_economyClassSeat->setValue(economyClassSeats);
                ui->spinBox_firstClassPrice->setValue(firstClassPrice);
                ui->spinBox_economyClassPrice->setValue(economyClassPrice);
                ui->lineEdit_airlineCompany->setText(airlineCompany);
            } else {
                // 注册失败，弹出错误提示框
                qDebug() << "查找失败";
                QMessageBox::critical(nullptr, "失败",
                                      "查找失败：ID不存在",
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

void adminUpdateFlightWindow::onCancelButtonClicked()
{
    ui->lineEdit_flightNumber->setText("");
    ui->lineEdit_aircraftModel->setText("");
    ui->lineEdit_departure->setText("");
    ui->lineEdit_destination->setText("");
    ui->lineEdit_date1->setText("");
    ui->lineEdit_date2->setText("");
    ui->dateTimeEdit1->setTime(QTime (0,0,0));
    ui->dateTimeEdit2->setTime(QTime (0,0,0));
    ui->spinBox_firstClassSeat->setValue(0);
    ui->spinBox_economyClassSeat->setValue(0);
    ui->spinBox_firstClassPrice->setValue(0);
    ui->spinBox_economyClassPrice->setValue(0);
    ui->lineEdit_airlineCompany->setText("");
}

void adminUpdateFlightWindow::onSaveButtonClicked()
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
        QMessageBox::critical(nullptr, "修改失败",
                              "修改失败: 输入的内容不能含有空格！",
                              QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    //检查是否填了全部内容
    if(flightNumber=="" || aircraftModel=="" || departure=="" || destination=="" || departure_date=="" || arrival_date=="" ||company=="")
    {
        QMessageBox::critical(nullptr, "修改失败",
                              "修改失败: 您需要填写全部信息！",
                              QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    //检查出发时间是否在到达时间之前
    if(unixTimeStamp1>=unixTimeStamp2)
    {
        QMessageBox::critical(nullptr, "修改失败",
                              "修改失败: 出发时间必须在到达时间之前",
                              QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    //检查座舱价格和数量是否为0
    if(firstClassSeats==0)
    {
        QMessageBox::critical(nullptr, "修改失败",
                              "修改失败: 头等舱数量不能为0！",
                              QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if(firstClassPrice==0)
    {
        QMessageBox::critical(nullptr, "修改失败",
                              "修改失败: 头等舱价格不能为0！",
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
        QMessageBox::critical(nullptr, "修改失败",
                              "修改失败: 经济舱价格不能为0！",
                              QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    // // 创建网络管理器
    QNetworkAccessManager* manager = new QNetworkAccessManager();

    // // 设置请求 URL
    QUrl url("http://localhost:8080/api/flights/update");
    QNetworkRequest request(url);

    // // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // // 构建 JSON 请求体
    QJsonObject json;
    json["id"]=this->id;
    json["flightNumber"]=flightNumber;
    json["departure"] = departure;
    json["destination"] =destination;
    json["departureTime"] = unixTimeStamp1;
    json["arrivalTime"] = unixTimeStamp2;
    json["firstClassSeats"] = firstClassSeats;
    json["economyClassSeats"] = economyClassSeats;
    json["firstClassPrice"] = firstClassPrice*100;
    json["economyClassPrice"] = economyClassPrice*100;
    json["airlineCompany"] = company;
    json["aircraftModel"] = aircraftModel;


    QJsonDocument jsonDoc(json);
    QByteArray requestData = jsonDoc.toJson();

    // // 发送 POST 请求
    QNetworkReply* reply = manager->post(request, requestData);
    // 连接信号，等待响应
    connect(reply, &QNetworkReply::finished, [reply,this]() {
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
                msgBox.setWindowTitle("修改成功");         // 设置窗口标题
                msgBox.setText("您已成功修改ID为："+QString::number(this->id)+"的航班信息");          // 设置提示文本
                msgBox.setStandardButtons(QMessageBox::Ok); // 设置标准按钮（此处只有“确定”按钮）
                // 显示消息框
                msgBox.exec();
            } else {
                // 注册失败，弹出错误提示框
                qDebug() << "修改失败, code: " << code;
                QMessageBox::critical(nullptr, "修改失败",
                                      "修改失败: " + message,
                                      QMessageBox::Ok, QMessageBox::Ok);
            }
        } else {
            // 请求失败，弹出错误提示框
            QString errorString = reply->errorString();
            qDebug() << "Error:" << errorString;

            // 创建 QMessageBox 来显示错误信息
            QMessageBox::critical(nullptr, "修改失败",
                                  "请求失败: " + errorString,
                                  QMessageBox::Ok, QMessageBox::Ok);
        }

        reply->deleteLater(); // 释放资源
    });
}

adminUpdateFlightWindow::~adminUpdateFlightWindow()
{
    delete ui;
}
