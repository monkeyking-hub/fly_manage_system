#ifndef NEWREGISTERWINDOW_H
#define NEWREGISTERWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QPropertyAnimation>
#include <QMessageBox>
#include <QSpacerItem>
#include <QApplication>
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

public:
    explicit newRegisterWindow(QWidget *parent = nullptr);
    ~newRegisterWindow();

signals:
    void registerRequested(const QString &usrname,const QString& password,const QString& email,
                           const QString &phone,const QString &idNumber,const int &age=-1,
                           const QString &gender="M/F",const int &usrLevel=1);

public slots:
    void onRegisterButtonClicked();

private:
    Ui::newRegisterWindow *ui;
};


class registerHandler:public QObject
{
    Q_OBJECT;

public slots:
    void handleRegister(const QString &usrname,const QString& password,const QString& email,
                        const QString &phone,const QString &idNumber,const int &age=-1,
                        const QString &gender="M/F",const int &usrLevel=1);
};

#endif // NEWREGISTERWINDOW_H
