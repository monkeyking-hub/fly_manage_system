#include "admindeleteflightwindow.h"
#include "ui_admindeleteflightwindow.h"
#include<QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

adminDeleteFlightWindow::adminDeleteFlightWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::adminDeleteFlightWindow)
{
    ui->setupUi(this);

    ui->lineEdit->setPlaceholderText("请输入航班ID");
    QString styleSheet = R"(
    QLineEdit {
        background-color: #cccccc;  /* 默认浅灰色背景 */
        border: 2px solid #D3D3D3; /* 默认浅灰色边框 */
        border-radius: 8px;        /* 圆角边框 */
        padding: 6px;             /* 内边距 */
        font-size: 14px;          /* 字体大小 */
        color: #333333;           /* 默认深灰色字体 */
    }
    QLineEdit:hover {
        border: 2px solid #A6A6A6; /* 鼠标悬浮时边框颜色 */
        background-color: #FFFFFF; /* 鼠标悬浮时背景颜色 */
    }
    QLineEdit:focus {
        border: 2px solid #5B9BD5; /* 聚焦时的边框颜色 */
        background-color: #FFFFFF; /* 聚焦时的背景颜色 */
    }
    )";
    ui->lineEdit->setStyleSheet(styleSheet);

    QPixmap pixmap(":/imgfordelete.png");
    QPixmap scaledPixmap=pixmap.scaled(ui->label_image->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_image->setPixmap(scaledPixmap);
    connect(ui->btn_delete,&QPushButton::clicked,this,&adminDeleteFlightWindow::onDeleteButtonClicked);
}

void adminDeleteFlightWindow::onDeleteButtonClicked()
{
    if(ui->lineEdit->text()=="" || ui->lineEdit->text().contains(" "))
    {
        QMessageBox::critical(nullptr, "删除失败",
                              "删除失败: 请输入有效的航班ID！",
                              QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, "确认操作", "您确定要删除id为："+ui->lineEdit->text()+"的航班信息吗？",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // // 创建网络管理器
        QNetworkAccessManager* manager = new QNetworkAccessManager();

        // // 设置请求 URL
        QUrl url("http://localhost:8080/api/flights/delete");
        QNetworkRequest request(url);

        // // 设置请求头
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        // // 构建 JSON 请求体
        QJsonObject json;
        json["id"]=ui->lineEdit->text().toInt();

        QJsonDocument jsonDoc(json);
        QByteArray requestData = jsonDoc.toJson();

        // // 发送 POST 请求
        QNetworkReply* reply = manager->post(request, requestData);
        // 连接信号，等待响应
        connect(reply, &QNetworkReply::finished, [reply,this]() {
            if (reply->error() == QNetworkReply::NoError) {
                // 请求成功，读取响应数据
                QByteArray responseData = reply->readAll();
                QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
                QJsonObject responseObject = jsonResponse.object();

                // 解析响应 JSON
                int code = responseObject["code"].toInt();  // 获取返回的 code
                QString message = responseObject["message"].toString();
                QJsonObject data = responseObject["data"].toObject();

                if (code == 200) {
                    // 删除成功,弹出信息框
                    QMessageBox msgBox;
                    msgBox.setIcon(QMessageBox::Information);  // 设置图标类型
                    msgBox.setWindowTitle("成功");         // 设置窗口标题
                    msgBox.setText("删除成功！");          // 设置提示文本
                    msgBox.setStandardButtons(QMessageBox::Ok); // 设置标准按钮（此处只有“确定”按钮）
                    // 显示消息框
                    msgBox.exec();
                } else {
                    // 删除失败，弹出错误提示框
                    qDebug() << "删除失败";
                    QMessageBox::critical(nullptr, "失败",
                                          "删除失败：ID不存在",
                                          QMessageBox::Ok, QMessageBox::Ok);
                }
            } else {
                // 请求失败，弹出错误提示框
                QString errorString = reply->errorString();
                qDebug() << "Error:" << errorString;

                // 创建 QMessageBox 来显示错误信息
                QMessageBox::critical(nullptr, "删除失败",
                                      "删除失败: ID不存在",
                                      QMessageBox::Ok, QMessageBox::Ok);
            }

            reply->deleteLater(); // 释放资源
        });
    } else {
        return;
    }
}

adminDeleteFlightWindow::~adminDeleteFlightWindow()
{
    delete ui;
}
