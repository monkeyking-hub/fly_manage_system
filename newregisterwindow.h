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

private:
    Ui::newRegisterWindow *ui;
};

#endif // NEWREGISTERWINDOW_H
