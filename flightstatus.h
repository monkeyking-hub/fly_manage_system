#ifndef FLIGHTSTATUS_H
#define FLIGHTSTATUS_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTableWidget>

namespace Ui {
class flightstatus;
}

class flightstatus : public QMainWindow
{
    Q_OBJECT

public:
    explicit flightstatus(QWidget *parent = nullptr);
    ~flightstatus();

private slots:
    void searchFlights();  // 搜索航班信息
    void handleSearchReply();  // 处理服务器返回的数据

private:
    Ui::flightstatus *ui;  // UI 类名保持一致
    QNetworkAccessManager *networkManager;  // 用于发送 HTTP 请求
};

#endif // FLIGHTSTATUS_H
