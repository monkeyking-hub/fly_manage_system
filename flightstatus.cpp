#include "flightstatus.h"
#include "ui_flightstatus.h"  // UI 文件的名称
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>

// 构造函数
flightstatus::flightstatus(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::flightstatus),  // 与 .h 文件保持一致
    networkManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);  // setupUi 调用类名一致

    // 设置主窗口背景
    this->setStyleSheet("background-color: #f0f4f7; font-family: Arial, sans-serif;");

    // 设置 QLabel 样式
    ui->label->setStyleSheet("color: #2c3e50; font-size: 16px; font-weight: bold;");
    ui->label_2->setStyleSheet("color: #2c3e50; font-size: 16px; font-weight: bold;");
    ui->label_3->setStyleSheet("color: #2c3e50; font-size: 16px; font-weight: bold;");

    // 设置 QLineEdit 样式
    ui->departureInput->setStyleSheet(
        "border: 1px solid #dcdde1; "
        "border-radius: 5px; "
        "padding: 5px; "
        "background-color: #ffffff; "
        "font-size: 14px;"
        );
    ui->destinationInput->setStyleSheet(
        "border: 1px solid #dcdde1; "
        "border-radius: 5px; "
        "padding: 5px; "
        "background-color: #ffffff; "
        "font-size: 14px;"
        );

    // 设置 QDateTimeEdit 样式
    ui->departureTimeInput->setStyleSheet(
        "border: 1px solid #dcdde1; "
        "border-radius: 5px; "
        "padding: 5px; "
        "background-color: #ffffff; "
        "font-size: 14px;"
        );

    // 设置 QPushButton 样式
    ui->searchButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #3498db; "
        "    color: white; "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "    border-radius: 5px; "
        "    padding: 8px 16px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1c598a;"
        "}"
        );

    // 设置 QTableWidget 样式
    ui->resultTable->setStyleSheet(
        "QTableWidget {"
        "    background-color: #ffffff;"
        "    border: 1px solid #dcdde1;"
        "    gridline-color: #dcdde1;"
        "    font-size: 14px;"
        "}"
        "QHeaderView::section {"
        "    background-color: #3498db;"
        "    color: white;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    padding: 4px;"
        "    border: none;"
        "}"
        "QTableWidget::item {"
        "    padding: 5px;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: #dcdde1;"
        "    color: #2c3e50;"
        "}"
        "QTableWidget::alternate-background {"
        "    background-color: #f9f9f9;"
        "}"
        );
    ui->resultTable->setAlternatingRowColors(true);

    // 设置 QStatusBar 样式
    ui->statusbar->setStyleSheet("background-color: #ecf0f1; color: #2c3e50; font-size: 12px;");
    // 设置表格列标题
    QStringList headers = {
         "航班号", "始发地", "目的地",
        "出发时间", "到达时间", "头等舱余票",
        "经济舱余票", "头等舱票价", "经济舱票价"
    };
    ui->resultTable->setColumnCount(headers.size());
    ui->resultTable->setHorizontalHeaderLabels(headers);

    // 连接搜索按钮点击信号到槽函数
    connect(ui->searchButton, &QPushButton::clicked, this, &flightstatus::searchFlights);
}

// 析构函数
flightstatus::~flightstatus()
{
    delete ui;
}

// 搜索航班信息
void flightstatus::searchFlights()
{
    // 获取用户输入
    QString departure = ui->departureInput->text();
    QString destination = ui->destinationInput->text();
    QDateTime departureTime = ui->departureTimeInput->dateTime();

    if (departure.isEmpty() || destination.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Departure and destination cannot be empty.");
        return;
    }

    // 创建 HTTP 请求
    QUrl url("http://your_server/api/flights/search");  // 替换为实际服务器地址
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 构建 JSON 请求体
    QJsonObject json;
    json["departure"] = departure;
    json["destination"] = destination;
    json["departureTime"] = departureTime.toSecsSinceEpoch();  // 转换为 Unix 时间戳

    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson();

    // 发送 POST 请求
    QNetworkReply *reply = networkManager->post(request, jsonData);

    // 处理返回结果
    connect(reply, &QNetworkReply::finished, this, &flightstatus::handleSearchReply);
}

// 处理服务器返回的数据
void flightstatus::handleSearchReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
        QJsonObject jsonObject = jsonResponse.object();

        if (jsonObject["code"].toInt() == 200) {
            // 清空表格数据
            ui->resultTable->setRowCount(0);

            QJsonArray dataArray = jsonObject["data"].toArray();
            ui->resultTable->setRowCount(dataArray.size());

            // 填充数据到表格
            for (int i = 0; i < dataArray.size(); ++i) {
                QJsonObject flight = dataArray[i].toObject();
                ui->resultTable->setItem(i, 0, new QTableWidgetItem(flight["flightNumber"].toString()));
                ui->resultTable->setItem(i, 1, new QTableWidgetItem(flight["departure"].toString()));
                ui->resultTable->setItem(i, 2, new QTableWidgetItem(flight["destination"].toString()));
                ui->resultTable->setItem(i, 3, new QTableWidgetItem(QDateTime::fromSecsSinceEpoch(flight["departureTime"].toInt()).toString()));
                ui->resultTable->setItem(i, 4, new QTableWidgetItem(QDateTime::fromSecsSinceEpoch(flight["arrivalTime"].toInt()).toString()));
                ui->resultTable->setItem(i, 5, new QTableWidgetItem(QString::number(flight["firstClassSeats"].toInt())));
                ui->resultTable->setItem(i, 6, new QTableWidgetItem(QString::number(flight["economyClassSeats"].toInt())));
                ui->resultTable->setItem(i, 7, new QTableWidgetItem(QString::number(flight["firstClassPrice"].toDouble())));
                ui->resultTable->setItem(i, 8, new QTableWidgetItem(QString::number(flight["economyClassPrice"].toDouble())));
            }
        } else {
            QMessageBox::warning(this, "Search Failed", jsonObject["message"].toString());
        }
    } else {
        QMessageBox::critical(this, "Error", reply->errorString());
    }

    reply->deleteLater();
}
