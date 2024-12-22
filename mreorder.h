#ifndef MREORDERWINDOW_H
#define MREORDERWINDOW_H

#include <QComboBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class MReorderWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MReorderWindow(QWidget *parent = nullptr);
    ~MReorderWindow();

private slots:
    void onSubmitClicked();                     // 提交按钮点击事件
    void onReplyFinished(QNetworkReply *reply); // 网络请求完成事件

private:
    QLineEdit *orderIdEdit;                // 订单ID输入框
    QComboBox *statusCombo;                // 订单状态下拉框
    QLineEdit *paymentTimeEdit;            // 支付时间输入框
    QPushButton *submitButton;             // 提交按钮
    QNetworkAccessManager *networkManager; // 网络管理器，用于发送请求
};

#endif // MREORDERWINDOW_H
