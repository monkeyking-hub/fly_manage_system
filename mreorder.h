#ifndef MREORDERWINDOW_H
#define MREORDERWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class MReorderWindow : public QWidget
{
    Q_OBJECT

public:
    MReorderWindow(QWidget *parent = nullptr);
    ~MReorderWindow();

private slots:
    void onSubmitClicked(const QDate &selectedDate);
    void onReplyFinished(QNetworkReply *reply);

private:
    QLineEdit *orderIdEdit;
    QComboBox *statusCombo;
    QDateEdit *dateEdit;
    QPushButton *submitButton;
    QNetworkAccessManager *networkManager;
};

#endif // MREORDERWINDOW_H
