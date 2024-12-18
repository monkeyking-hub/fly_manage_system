#include "admindeleteflightwindow.h"
#include "ui_admindeleteflightwindow.h"

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
}

adminDeleteFlightWindow::~adminDeleteFlightWindow()
{
    delete ui;
}
