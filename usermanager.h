#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "user.h"
#include <QObject>

class UserManager : public QObject {
    Q_OBJECT

private:
    static UserManager* instance;
    User currentUser;

    explicit UserManager(QObject *parent = nullptr) : QObject(parent) {} // 私有构造函数

public:
    static UserManager* getInstance() {
        if (instance == nullptr) {
            instance = new UserManager;
        }
        return instance;
    }

    void setCurrentUser(const User& user) {
        currentUser = user;
        emit currentUserChanged(currentUser);
        emit loginSuccess();  // 登录成功后发出信号
    }
    void clearCurrentUser() {
        // 清空当前用户信息
        currentUser.username = "";
        currentUser.email = "";
        currentUser.phonenumber = "";
        currentUser.age = 0;
        currentUser.sex = "";

        emit currentUserChanged(currentUser); // 发出用户更改信号，通知界面或其他组件
    }

    User getCurrentUser() const {
        return currentUser;
    }

signals:
    void currentUserChanged(const User& user);
    void loginSuccess();  // 登录成功的信号

};

#endif // USERMANAGER_H
