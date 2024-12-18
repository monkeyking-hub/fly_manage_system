#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "user.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QDir>



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
        fetchAvatarFromServer(); // 然后获取头像
        emit currentUserChanged(currentUser);
        emit loginSuccess();
    }


    void fetchAvatarFromServer() {
        QString email = currentUser.email;
        QUrl url("http://127.0.0.1:5000/download-avatar/" + QUrl::toPercentEncoding(email));
        QNetworkRequest request(url);
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);

        QNetworkReply *reply = manager->get(request);
        connect(reply, &QNetworkReply::finished, this, [this, reply, email]() {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray responseData = reply->readAll();

                // Ensure the directory exists
                QString directoryPath = "./avatars";
                QDir dir(directoryPath);
                if (!dir.exists()) {
                    dir.mkpath("."); // Try to create the directory
                }

                // Set up the file path
                QString imagePath = directoryPath + "/" + email + ".jpg";
                QFile file(imagePath);
                if (file.open(QIODevice::WriteOnly)) {
                    file.write(responseData);
                    file.close();
                    currentUser.avatarUrl = imagePath;
                    qDebug() << "fetch头像获取成功"<<imagePath;
                } else {
                    qDebug() << "Fetch头像获取失败" << file.errorString();
                    currentUser.avatarUrl = ":/images/default_avatar.png"; // Use default avatar on failure
                }
            } else {
                qDebug() << "fetch avartar 头像连接失败" << reply->errorString();
                currentUser.avatarUrl = ":/images/default_avatar.png"; // Use default avatar on failure
            }
            emit currentUserChanged(currentUser);
            reply->deleteLater();
        });
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
    void profileChanged();

};

#endif // USERMANAGER_H
