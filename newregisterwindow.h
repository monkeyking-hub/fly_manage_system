#ifndef NEWREGISTERWINDOW_H
#define NEWREGISTERWINDOW_H

#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QWidget>
#include "animatedInputField.h"

namespace Ui {
class newRegisterWindow;
}

class newRegisterWindow : public QMainWindow
{
    Q_OBJECT

public:
    AnimatedInputField *usernameField;
    AnimatedInputField *emailField;
    AnimatedInputField *passwordField;
    AnimatedInputField *phoneField;
    AnimatedInputField *idField;
    AnimatedInputField *emailCodeField;
    QString emailCode = "我是初始验证码"; //验证码

public:
    explicit newRegisterWindow(QWidget *parent = nullptr);
    ~newRegisterWindow();

signals:
    void registerRequested(const QString &usrname,
                           const QString &password,
                           const QString &email,
                           const QString &phone,
                           const QString &idNumber,
                           const QString &emailCodeInput,
                           const int &age = -1,
                           const QString &gender = "M/F",
                           const int &usrLevel = 1);

public slots:
    void onRegisterButtonClicked();
    void onReturnButtonClicked();
    void onSendEmailCodeBtnClicked();

private:
    Ui::newRegisterWindow *ui;
};

class registerHandler : public QObject
{
    Q_OBJECT;

public slots:
    void handleRegister(const QString &usrname,
                        const QString &password,
                        const QString &email,
                        const QString &phone,
                        const QString &idNumber,
                        const QString &emailCodeInput,
                        const int &age = -1,
                        const QString &gender = "M/F",
                        const int &usrLevel = 1);
};

#endif // NEWREGISTERWINDOW_H
