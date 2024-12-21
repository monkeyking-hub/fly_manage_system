#include "userwindow.h"
#include <QFileDialog>
#include <QImage>
#include <QMessageBox>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include "interfacemanager.h"
#include "ui_userwindow.h"
#include "UserManager.h"
#include "maininterface.h"

Userwindow::Userwindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::Userwindow) {
    ui->setupUi(this);
    connect(UserManager::getInstance(), &UserManager::currentUserChanged, this, &Userwindow::updateUserInfo);
    connect(UserManager::getInstance(), &UserManager::loginSuccess, this, &Userwindow::loadUserInfo);
    // 每次打开界面时获取最新用户信息
    fetchUserInfoFromServer();
}

void Userwindow::loadUserInfo() {
    User currentUser = UserManager::getInstance()->getCurrentUser();
    ui->useNameLabel->setText(currentUser.username);
    ui->mailLabel_2->setText(currentUser.email);
    ui->phoneNumberLabel->setText(currentUser.phonenumber);
    ui->agelabel->setText(QString::number(currentUser.age));
    currentUser.sex == "M" ? ui->sexLabel->setText("男") : ui->sexLabel->setText("女");

    QPixmap avatar;
    qDebug()<<"当前头像路径"<<currentUser.avatarUrl;
    if (avatar.load(currentUser.avatarUrl)) {
        ui->profileLbl->setPixmap(avatar.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        qDebug("加载头像成功");
    } else {
        ui->profileLbl->setPixmap(QPixmap("://defaultedProfile.jpg")); // 加载默认头像
        qDebug("加载头像不成功,使用默认头像");
    }
}


void Userwindow::updateUserInfo(const User &user) {
    ui->userNameSetBox->setText(user.username);
    ui->mailSetBox->setText(user.email);
    ui->phoneNumberSetBox->setText(user.phonenumber);
    ui->ageSetBox->setText(QString::number(user.age));
    if(user.sex=="F")
    {
        ui->famelButton->setChecked(true);
    }
    else ui->maleButton->setChecked(true);
    User currentUser = UserManager::getInstance()->getCurrentUser();
    QPixmap avatar;
    qDebug()<<"当前头像路径"<<currentUser.avatarUrl;
    if (avatar.load(currentUser.avatarUrl)) {
        ui->profileLbl->setPixmap(avatar.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        qDebug("加载头像成功");
    } else {
        ui->profileLbl->setPixmap(QPixmap("://defaultedProfile.jpg")); // 加载默认头像
        qDebug("加载头像不成功,使用默认头像");
    }
}

Userwindow::~Userwindow() {
    delete ui;
}

void Userwindow::fetchUserInfoFromServer() {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://localhost:8080/api/users/get");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 获取当前用户的邮箱
    QString email = UserManager::getInstance()->getCurrentUser().email;
    QJsonObject json;
    json["email"] = email;

    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject responseObject = jsonResponse.object();

            if (responseObject["code"].toInt() == 200) {
                // 更新用户信息
                QJsonObject data = responseObject["data"].toObject();
                User currentUser;
                currentUser.username = data["username"].toString();
                currentUser.email = data["email"].toString();
                currentUser.phonenumber = data["phone"].toString();
                currentUser.age = data["age"].toInt();
                data["gender"].toString()=="M"?currentUser.sex ="M":currentUser.sex ="F";
                UserManager::getInstance()->setCurrentUser(currentUser);
                loadUserInfo(); // 更新界面显示
            } else {
                //QMessageBox::warning(this, "Error", "Failed to fetch user information.");
            }
        } else {
            QMessageBox::critical(this, "Error", "Network error: " + reply->errorString());
        }
        reply->deleteLater();
    });
}

void Userwindow::on_pushButton_clicked() {
    ui->userNameStackedWidget->setCurrentIndex(1);
    ui->mailStackedWidget->setCurrentIndex(1);
    ui->phoneNumberStackedWidget->setCurrentIndex(1);
    ui->sexStackedWidget->setCurrentIndex(1);
    ui->oiStackedWidget->setCurrentIndex(1);
    ui->ProfileStackedWidget->setCurrentIndex(1);
    ui->ageStackedWidget->setCurrentIndex(1);
}

void Userwindow::on_saveButton_clicked() {
    // 获取当前用户的信息
    User currentUser = UserManager::getInstance()->getCurrentUser();

    // 获取用户输入的修改信息
    QString username = ui->userNameSetBox->text();
    QString email = ui->mailSetBox->text();
    QString phone = ui->phoneNumberSetBox->text();
    int age = ui->ageSetBox->text().toInt();
    QString gender = ui->maleButton->isChecked() ? "M" : (ui->famelButton->isChecked() ? "F" : "UnSelected");

    // 检查是否有任何信息发生变化
    if (username == currentUser.username &&
        email == currentUser.email &&
        phone == currentUser.phonenumber &&
        age == currentUser.age &&
        gender == currentUser.sex) {
        // 如果没有任何修改，直接返回
        ui->userNameStackedWidget->setCurrentIndex(0);
        ui->mailStackedWidget->setCurrentIndex(0);
        ui->phoneNumberStackedWidget->setCurrentIndex(0);
        ui->sexStackedWidget->setCurrentIndex(0);
        ui->oiStackedWidget->setCurrentIndex(0);
        ui->ProfileStackedWidget->setCurrentIndex(0);
        ui->ageStackedWidget->setCurrentIndex(0);
        return;
    }

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://localhost:8080/api/users/update");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 构建 JSON 请求体
    QJsonObject json;
    json["id"] = currentUser.id; // 当前用户ID
    json["username"] = username;
    json["email"] = email;
    json["phone"] = phone;
    json["age"] = age;
    json["gender"] = gender;

    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject responseObject = jsonResponse.object();

            if (responseObject["code"].toInt() == 200) {
                // 弹出提示框，等待用户点击后再返回信息显示界面
                QMessageBox msgBox(this);
                msgBox.setWindowTitle("Success");
                msgBox.setText("您已修改成功");
                msgBox.setIcon(QMessageBox::Information);
                msgBox.exec(); // 等待用户点击“确定”后再继续执行

                // 更新用户信息
                fetchUserInfoFromServer(); // 更新最新信息到界面

                // 切换回只读模式
                ui->userNameStackedWidget->setCurrentIndex(0);
                ui->mailStackedWidget->setCurrentIndex(0);
                ui->phoneNumberStackedWidget->setCurrentIndex(0);
                ui->sexStackedWidget->setCurrentIndex(0);
                ui->oiStackedWidget->setCurrentIndex(0);
                ui->ProfileStackedWidget->setCurrentIndex(0);
                ui->ageStackedWidget->setCurrentIndex(0);
            } else {
                QMessageBox::warning(this, "Error", "Failed to update user information.");
            }
        } else {
            QMessageBox::critical(this, "Error", "Network error: " + reply->errorString());
        }
        reply->deleteLater();
    });
}



void Userwindow::on_uploadProfileButton_clicked() {
    QString filePath = QFileDialog::getOpenFileName(this, "选择头像", "", "Images (*.jpg);;All Files (*)");
    if (filePath.isEmpty()) {
        return;  // 如果用户没有选择文件，直接返回
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "错误", "无法打开文件：" + filePath);
        return;
    }

    QByteArray fileContent = file.readAll();
    file.close();

    QString boundary = "boundary-" + QString::number(QDateTime::currentMSecsSinceEpoch(), 16);
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://127.0.0.1:5000/upload-avatar");
    QNetworkRequest request(url);
    request.setRawHeader("Content-Type", "multipart/form-data; boundary=" + boundary.toUtf8());

    QByteArray finalData = QString("--%1\r\n"
                                   "Content-Disposition: form-data; name=\"email\"\r\n\r\n"
                                   "%2\r\n"
                                   "--%1\r\n"
                                   "Content-Disposition: form-data; name=\"avatar\"; filename=\"%3\"\r\n"
                                   "Content-Type: image/jpeg\r\n\r\n")
                               .arg(boundary)
                               .arg(UserManager::getInstance()->getCurrentUser().email)
                               .arg(QFileInfo(file.fileName()).fileName())
                               .toUtf8();
    finalData += fileContent;
    finalData += QString("\r\n--%1--\r\n").arg(boundary).toUtf8();

    QNetworkReply *reply = manager->post(request, finalData);
    connect(reply, &QNetworkReply::finished, this, [this, reply, fileContent, filePath]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject responseObject = jsonResponse.object();
            QMessageBox::information(this, "成功", "头像上传成功： " + responseObject["file_path"].toString());
            fetchUserInfoFromServer();
            QPixmap avatar;
            if (avatar.load(filePath)) {
                ui->profileLbl->setPixmap(avatar.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        } else {
            QMessageBox::warning(this, "错误123", reply->errorString());
        }
        reply->deleteLater();
    });
}



void Userwindow::on_logOutButton_clicked() {
    InterfaceManager::instance()->switchToPage("lxt_newLoginWindow");
}

void Userwindow::on_returnButton_clicked() {
    InterfaceManager::instance()->switchToPage("lxt_mainInterface");
}

void Userwindow::on_logOffButton_clicked()
{
    // 获取当前用户的邮箱
    QString email = UserManager::getInstance()->getCurrentUser().email;

    if (email.isEmpty()) {
        QMessageBox::warning(this, "Error", "Cannot log out: No user is currently logged in.");
        return;
    }

    // 创建网络请求
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://localhost:8080/api/users/logout");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 构建 JSON 请求体
    QJsonObject json;
    json["email"] = email;

    // 发送 POST 请求
    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject responseObject = jsonResponse.object();

            if (responseObject["code"].toInt() == 200) {
                QMessageBox::information(this, "Success", "Logout successful.");

                // 清除当前用户数据
                UserManager::getInstance()->clearCurrentUser();

                // 跳转到登录界面
                InterfaceManager::instance()->switchToPage("lxt_newLoginWindow");
            } else {
                QMessageBox::warning(this, "Error", responseObject["message"].toString());
            }
        } else {
            QMessageBox::critical(this, "Error", "Network error: " + reply->errorString());
        }
        reply->deleteLater();
    });
}

