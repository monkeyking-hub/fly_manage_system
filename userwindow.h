#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "user.h"

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
    // 用户操作相关槽函数
    void on_pushButton_clicked();          // 切换到编辑模式
    void on_saveButton_clicked();          // 保存用户修改信息
    void on_uploadProfileButton_clicked(); // 上传头像
    void on_logOutButton_clicked();        // 登出
    void on_returnButton_clicked();        // 返回主界面

    // 加载和更新用户信息
    void loadUserInfo();                   // 加载用户信息到界面
    void updateUserInfo(const User &user); // 根据用户对象更新界面

    void on_logOffButton_clicked();

    void on_pushButton_2_clicked();

private:
    // 私有方法
    void fetchUserInfoFromServer();                            // 从服务器获取用户信息
    void sendUpdatedUserInfoToServer(const User &updatedUser); // 发送修改后的信息到服务器

private:
    Ui::Userwindow *ui;
    QNetworkAccessManager *networkManager; // 网络请求管理器
};

#endif // USERWINDOW_H
