#ifndef M_FIND_ORDER_WINDOW_H
#define M_FIND_ORDER_WINDOW_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class MFindOrderWindow : public QWidget
{
    Q_OBJECT

public:
    MFindOrderWindow(QWidget *parent = nullptr);
    ~MFindOrderWindow();

private slots:
    void onSearchButtonClicked();

private:
    QLineEdit *userIdLineEdit; // 用户ID输入框
    QPushButton *searchButton; // 搜索按钮
};

#endif // M_FIND_ORDER_WINDOW_H
