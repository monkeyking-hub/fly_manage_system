#ifndef ADMINUSERMANAGERWINDOW_H
#define ADMINUSERMANAGERWINDOW_H
#include "usermanager.h"

#include <QWidget>

namespace Ui {
class adminUserManagerWindow;
}

class adminUserManagerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit adminUserManagerWindow(QWidget *parent = nullptr);
    ~adminUserManagerWindow();

private slots:
    void on_btn_userSearch_clicked();

    void on_pushButton_clicked();

    void on_saveButton_clicked();

    void on_uploadProfileButton_clicked();

    void on_logoff_button_clicked();

private:
    Ui::adminUserManagerWindow *ui;
    User user;
    void loadUserInfo();
    void fetchUserInfoFromServer();
    void updateUserInfo(const User &user);
    void getUserProfile();
};

#endif // ADMINUSERMANAGERWINDOW_H
