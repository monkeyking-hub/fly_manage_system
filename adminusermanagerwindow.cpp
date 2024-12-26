#include "adminusermanagerwindow.h"
#include "ui_adminusermanagerwindow.h"
#include "QNetworkAccessManager"
#include "usermanager.h"
#include "QMessageBox"
#include "QJsonObject"
#include "QJsonDocument"
#include "QFileDialog"

adminUserManagerWindow::adminUserManagerWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::adminUserManagerWindow)
{
    ui->setupUi(this);
    QPixmap pixmap("://Aplace2.jpg");
    QPixmap scaledPixmap = pixmap.scaled(ui->label->size(),
                                         Qt::KeepAspectRatio,
                                         Qt::SmoothTransformation);
    ui->label_3->setPixmap(scaledPixmap);
    ui->personInfoBox->setVisible(false);
}

adminUserManagerWindow::~adminUserManagerWindow()
{
    delete ui;
}

void adminUserManagerWindow:: getUserProfile()
{
    QString email = user.email;
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
                user.avatarUrl = imagePath;
                QPixmap avatar;
                qDebug() << "fetch头像获取成功" << imagePath;
                if (avatar.load(user.avatarUrl)) {
                    ui->profileLbl->setPixmap(
                        avatar.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                    qDebug("加载头像成功");
                } else {
                    ui->profileLbl->setPixmap(QPixmap("://defaultedProfile.jpg")); // 加载默认头像
                    qDebug("加载头像不成功,使用默认头像");
                }
            } else {
                qDebug() << "Fetch头像获取失败" << file.errorString();
                user.avatarUrl
                    = ":/images/default_avatar.png"; // Use default avatar on failure
            }
        } else {
            qDebug() << "fetch avartar 头像连接失败" << reply->errorString();
            user.avatarUrl = "://defaultedProfile.jpg"; // Use default avatar on failure
        }
        reply->deleteLater();
    });
}


void adminUserManagerWindow::updateUserInfo(const User &user)
{
    ui->userNameSetBox->setText(user.username);
    ui->mailSetBox->setText(user.email);
    ui->phoneNumberSetBox->setText(user.phonenumber);
    ui->ageSetBox->setText(QString::number(user.age));
    if (user.sex == "F") {
        ui->famelButton->setChecked(true);
    } else
        ui->maleButton->setChecked(true);
    ui->spinBox->setValue(user.level);
    QPixmap avatar;
    qDebug() << "当前头像路径" << user.avatarUrl;
    if (avatar.load(user.avatarUrl)) {
        ui->profileLbl->setPixmap(
            avatar.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        qDebug("加载头像成功");
    } else {
        ui->profileLbl->setPixmap(QPixmap("://defaultedProfile.jpg")); // 加载默认头像
        qDebug("加载头像不成功,使用默认头像");
    }
}

void adminUserManagerWindow:: fetchUserInfoFromServer()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://localhost:8080/api/users/get");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 获取当前用户的邮箱
    QString email = ui->le_userEmail->text();
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
                user.username = data["username"].toString();
                user.email = data["email"].toString();
                user.phonenumber = data["phone"].toString();
                user.age = data["age"].toInt();
                user.level=data["userLevel"].toInt();
                data["gender"].toString() == "M" ? user.sex = "M" : user.sex = "F";
                getUserProfile();
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

void adminUserManagerWindow::on_btn_userSearch_clicked()
{
    fetchUserInfoFromServer();
    updateUserInfo(user);
    loadUserInfo();
    ui->personInfoBox->setVisible(true);
}
void adminUserManagerWindow::on_pushButton_clicked()
{
    updateUserInfo(user);
    ui->userNameStackedWidget->setCurrentIndex(1);
    ui->mailStackedWidget->setCurrentIndex(1);
    ui->phoneNumberStackedWidget->setCurrentIndex(1);
    ui->sexStackedWidget->setCurrentIndex(1);
    ui->oiStackedWidget->setCurrentIndex(1);
    ui->ProfileStackedWidget->setCurrentIndex(1);
    ui->ageStackedWidget->setCurrentIndex(1);
    ui->stackedWidget->setCurrentIndex(1);
}

void adminUserManagerWindow::on_saveButton_clicked()
{
    // 获取当前用户的信息
    User currentUser = user;

    // 获取用户输入的修改信息
    QString username = ui->userNameSetBox->text();
    QString email = ui->mailSetBox->text();
    QString phone = ui->phoneNumberSetBox->text();
    int age = ui->ageSetBox->text().toInt();
    QString gender = ui->maleButton->isChecked()
                         ? "M"
                         : (ui->famelButton->isChecked() ? "F" : "UnSelected");
    int lvl=ui->spinBox->value();
    // 检查是否有任何信息发生变化
    if (username == currentUser.username && email == currentUser.email
        && phone == currentUser.phonenumber && age == currentUser.age
        && gender == currentUser.sex && ui->levelLabel->text()==QString::number(user.level)) {
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
    json["userLevel"] = lvl;
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
                ui->stackedWidget->setCurrentIndex(0);
            } else {
                QMessageBox::warning(this, "Error", "Failed to update user information.");
            }
        } else {
            QMessageBox::critical(this, "Error", "Network error: " + reply->errorString());
        }
        reply->deleteLater();
    });
}

void adminUserManagerWindow::on_uploadProfileButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "选择头像",
                                                    "",
                                                    "Images (*.jpg);;All Files (*)");
    if (filePath.isEmpty()) {
        return; // 如果用户没有选择文件，直接返回
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

    QByteArray finalData
        = QString("--%1\r\n"
                  "Content-Disposition: form-data; name=\"email\"\r\n\r\n"
                  "%2\r\n"
                  "--%1\r\n"
                  "Content-Disposition: form-data; name=\"avatar\"; filename=\"%3\"\r\n"
                  "Content-Type: image/jpeg\r\n\r\n")
              .arg(boundary)
              .arg(user.email)
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
            QMessageBox::information(this,
                                     "成功",
                                     "头像上传成功： " + responseObject["file_path"].toString());
            fetchUserInfoFromServer();
            QPixmap avatar;
            if (avatar.load(filePath)) {
                ui->profileLbl->setPixmap(
                    avatar.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        } else {
            QMessageBox::warning(this, "错误123", reply->errorString());
        }
        reply->deleteLater();
    });
}
void adminUserManagerWindow::loadUserInfo()
{
    ui->useNameLabel->setText(user.username);
    ui->mailLabel_2->setText(user.email);
    ui->phoneNumberLabel->setText(user.phonenumber);
    ui->agelabel->setText(QString::number(user.age));
    user.sex == "M" ? ui->sexLabel->setText("男") : ui->sexLabel->setText("女");
    ui->levelLbl->setText(QString::number(user.level));
    QPixmap avatar;
    qDebug() << "当前头像路径" << user.avatarUrl;
    if (avatar.load(user.avatarUrl)) {
        ui->profileLbl->setPixmap(
            avatar.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        qDebug("加载头像成功");
    } else {
        ui->profileLbl->setPixmap(QPixmap("://defaultedProfile.jpg")); // 加载默认头像
        qDebug("加载头像不成功,使用默认头像");
    }
}


void adminUserManagerWindow::on_logoff_button_clicked()
{
    // 获取当前用户的邮箱
    QString email = user.email;

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
                QMessageBox::information(this, "Success", "注销成功.");
            } else {
                QMessageBox::warning(this, "Error", responseObject["message"].toString());
            }
        } else {
            QMessageBox::critical(this, "Error", "Network error: " + reply->errorString());
        }
        reply->deleteLater();
    });
    ui->personInfoBox->setVisible(false);
    ui->le_userEmail->clear();
}

