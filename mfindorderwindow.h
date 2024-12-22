#ifndef M_FIND_ORDER_WINDOW_H
#define M_FIND_ORDER_WINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>

class MFindOrderWindow : public QWidget {
    Q_OBJECT

public:
    MFindOrderWindow(QWidget *parent = nullptr);
    ~MFindOrderWindow();

private slots:
    void onSearchButtonClicked();

private:
    QLineEdit *userIdLineEdit; // 用户ID输入框
    QPushButton *searchButton;  // 搜索按钮
};

#endif // M_FIND_ORDER_WINDOW_H
