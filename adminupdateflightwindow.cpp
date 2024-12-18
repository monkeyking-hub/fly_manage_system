#include "adminupdateflightwindow.h"
#include "ui_adminupdateflightwindow.h"

adminUpdateFlightWindow::adminUpdateFlightWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::adminUpdateFlightWindow)
{
    ui->setupUi(this);

    ui->calendarWidget->hide();
    ui->lineEdit_date1->setPlaceholderText("请选择日期");
    ui->lineEdit_date2->setPlaceholderText("请选择日期");

    ui->lineEdit_search->setPlaceholderText("请输入航班ID进行查找");

    // 加载图片
    QPixmap pixmap_position(":/position.png"); //起点和终点的图片
    QPixmap pixmap_date(":/date.png"); //出发时间和到达时间的图片
    QPixmap pixmap_flight(":/flightNumber.png"); //航班编号和飞机型号的图片
    QPixmap pixmap_company(":/company.png"); //航空公司的图片
    QPixmap pixmap_search(":/imgforsearch.jpg"); //搜索框上方的图片

    // 调整图片大小以适应 QLabel 的尺寸
    QPixmap scaledPixmap1 = pixmap_position.scaled(ui->img_departure->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledPixmap2 = pixmap_position.scaled(ui->img_destination->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledPixmap3 = pixmap_date.scaled(ui->img_date1->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledPixmap4 = pixmap_date.scaled(ui->img_date2->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledPixmap5 = pixmap_flight.scaled(ui->img_flightNumber->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledPixmap6 = pixmap_flight.scaled(ui->img_aircraftModel->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledPixmap7 = pixmap_company.scaled(ui->img_airlineCompany->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledPixmap8 = pixmap_search.scaled(ui->label_img->size());

    // 将调整后的图片设置为 QLabel 的内容
    ui->img_departure->setPixmap(scaledPixmap1);
    ui->img_destination->setPixmap(scaledPixmap2);
    ui->img_date1->setPixmap(scaledPixmap3);
    ui->img_date2->setPixmap(scaledPixmap4);
    ui->img_flightNumber->setPixmap(scaledPixmap5);
    ui->img_aircraftModel->setPixmap(scaledPixmap6);
    ui->img_airlineCompany->setPixmap(scaledPixmap7);
    ui->label_img->setPixmap(scaledPixmap8);

    QString styleSheet = R"(
    QLineEdit {
        background-color: #F5F5F5;  /* 默认浅灰色背景 */
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
    ui->lineEdit_search->setStyleSheet(styleSheet);

    connect(ui->btn_selectDate1, &QPushButton::clicked, this, [this]() {
        showCalendar(ui->btn_selectDate1);
    });
    connect(ui->btn_selectDate2, &QPushButton::clicked, this, [this]() {
        showCalendar(ui->btn_selectDate2);
    });
    connect(ui->calendarWidget, &QCalendarWidget::clicked, this, &adminUpdateFlightWindow::selectDate);
}

void adminUpdateFlightWindow::showCalendar(QPushButton *button)
{
    selectedBtn = button; // 标记哪个按钮被点击
    ui->calendarWidget->setHidden(false); // 显示日历控件
}

void adminUpdateFlightWindow::selectDate(const QDate &date)
{
    if (selectedBtn == ui->btn_selectDate1) {
        ui->lineEdit_date1->setText(date.toString("yyyy-MM-dd"));
    } else if (selectedBtn == ui->btn_selectDate2) {
        ui->lineEdit_date2->setText(date.toString("yyyy-MM-dd"));
    }
    ui->calendarWidget->setHidden(true); // 选择日期后隐藏日历控件
}

adminUpdateFlightWindow::~adminUpdateFlightWindow()
{
    delete ui;
}
