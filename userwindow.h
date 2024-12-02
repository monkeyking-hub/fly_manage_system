#ifndef USERWINDOW_H
#define USERWINDOW_H
#include "user.h"
#include <QMainWindow>

namespace Ui {
class Userwindow;
}

class Userwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit Userwindow(QWidget *parent = nullptr);
    ~Userwindow();

private slots:
    void on_pushButton_clicked();

    void on_saveButton_clicked();

    void on_uploadProfileButton_clicked();

    void on_logOutButton_clicked();

    void on_returnButton_clicked();

    void loadUserInfo();

    void updateUserInfo(const User& user);

private:
    Ui::Userwindow *ui;
};

#endif // USERWINDOW_H
