#include "flightstatus.h"
#include <QCalendarWidget>
#include <QCompleter>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QKeyEvent>
#include <QListView>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QStringList>
#include <QListWidgetItem>
#include "ui_flightstatus.h"
#include "listitem.h"

flightstatus::flightstatus(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::flightstatus)
    , networkmanager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    init();
    ui->departureInput->installEventFilter(this);
    ui->destinationInput->installEventFilter(this);
    connect(ui->departureInput, &QLineEdit::textChanged, this, [this]() {
        ui->pick_widget->setVisible(false);
    });
    connect(ui->destinationInput, &QLineEdit::textChanged, this, [this]() {
        ui->pick_widget_2->setVisible(false);
    });
    this->installEventFilter(this);

    // 安装事件过滤器，处理点击 pick_widget 以外的区域隐藏 pick_widget
    ui->pick_widget->installEventFilter(this);
    ui->pick_widget_2->installEventFilter(this);
}

// 析构函数
flightstatus::~flightstatus()
{
    delete ui;
}

void flightstatus::showPickWidget(QLineEdit *qle)
{
    if (qle == ui->departureInput) {
        QPoint pos = QPoint(80, 131);
        ui->pick_widget->move(pos);
        ui->pick_widget->setVisible(true);
    }
    if (qle == ui->destinationInput) {
        QPoint pos = QPoint(380, 131);
        ui->pick_widget_2->move(pos);
        ui->pick_widget_2->setVisible(true);
    }
}
bool flightstatus::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->groupBox_2) {
        switch (event->type()) {
        case QEvent::Enter:
            // 鼠标进入时的样式
            ui->groupBox_2->setStyleSheet(
                "QGroupBox { background-color: #f0f0f0; border: 2px solid #0096D6; }");
            break;
        case QEvent::Leave:
            // 鼠标离开时恢复原样
            ui->groupBox_2->setStyleSheet(
                "QGroupBox { background-color: white; border: 2px solid #A3C1DA; }");
            break;
        default:
            break;
        }
    }
    // 处理全局的鼠标点击事件，隐藏 pick_widget 和 pick_widget_2
    if (event->type() == QEvent::MouseButtonPress) {
        if (ui->pick_widget->isVisible()
            && !ui->pick_widget->geometry().contains(static_cast<QMouseEvent *>(event)->pos())) {
            ui->pick_widget->setVisible(false);
        }
        if (ui->pick_widget_2->isVisible()
            && !ui->pick_widget_2->geometry().contains(static_cast<QMouseEvent *>(event)->pos())) {
            ui->pick_widget_2->setVisible(false);
        }
    }
    // 处理 departureInput 和 destinationInput 的其他事件
    if (obj == ui->departureInput || obj == ui->destinationInput) {
        if (event->type() == QEvent::MouseButtonPress) {
            QLineEdit *qle = static_cast<QLineEdit *>(obj);
            showPickWidget(qle);
            ui->calendar->setVisible(false);
            return true; // 阻止事件继续传播
        }
    }

    // 处理 pick_widget 和 pick_widget_2 的事件
    if (obj == ui->pick_widget || obj == ui->pick_widget_2) {
        if (event->type() == QEvent::MouseButtonPress) {
            // 点击了 pick_widget 或 pick_widget_2，不隐藏窗口，直接返回
            return true;
        }
    }
    if (obj == ui->dateEdit && event->type() == QEvent::MouseButtonPress) {
        // Toggle calendar visibility on mouse press events inside QLineEdit
        ui->calendar->setVisible(!ui->calendar->isVisible());
        return true; // Stop event propagation
    } else if (event->type() == QEvent::MouseButtonPress) {
        // Check if the click is outside the QLineEdit and QCalendarWidget
        if (!ui->dateEdit->geometry().contains(static_cast<QMouseEvent *>(event)->pos())
            && !ui->calendar->geometry().contains(static_cast<QMouseEvent *>(event)->pos())
            && ui->calendar->isVisible()) {
            ui->calendar->hide();
        }
    } else if (obj == ui->calendar && event->type() == QEvent::Hide) {
        // Update QLineEdit text when the calendar is hidden
        if (ui->calendar->selectedDate().isValid()) {
            ui->dateEdit->setText(ui->calendar->selectedDate().toString("yyyy-MM-dd"));
        }
    }
    // 默认事件过滤器行为
    return QObject::eventFilter(obj, event);
}

void flightstatus::cityPicked(const QString city)
{
    ui->departureInput->setText(city);
    ui->pick_widget->setVisible(false);
}

void flightstatus::dscityPicked(const QString city)
{
    ui->destinationInput->setText(city);
    ui->pick_widget->setVisible(false);
}

// 设置出发地和目的地的补全器
void flightstatus::setupCompleter()
{
    cityListModel = new QStringListModel(cityList, this);

    // 创建补全器
    departureCompleter = new QCompleter(this);
    departureCompleter->setModel(cityListModel);
    departureCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    departureCompleter->setCompletionMode(QCompleter::PopupCompletion);
    ui->departureInput->setCompleter(departureCompleter);

    destinationCompleter = new QCompleter(this);
    destinationCompleter->setModel(cityListModel);
    destinationCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    destinationCompleter->setCompletionMode(QCompleter::PopupCompletion);
    ui->destinationInput->setCompleter(destinationCompleter);
}

// 出发地输入框文本变化时触发
void flightstatus::on_departureInput_textChanged(const QString &text)
{
    // 使用拼音或中文进行匹配
    QStringList filteredCities;
    for (const QString &city : cityList) {
        if (city.contains(text, Qt::CaseInsensitive)) {
            filteredCities << city;
        }
    }
    cityListModel->setStringList(filteredCities);
}

// 目的地输入框文本变化时触发
void flightstatus::on_destinationInput_textChanged(const QString &text)
{
    // 使用拼音或中文进行匹配
    QStringList filteredCities;
    for (const QString &city : cityList) {
        if (city.contains(text, Qt::CaseInsensitive)) {
            filteredCities << city;
        }
    }
    cityListModel->setStringList(filteredCities);
}

void flightstatus::on_pushButton_4_clicked()
{
    ui->sw->setCurrentIndex(2);
    setActiveSection("ABCDEF");
}

void flightstatus::on_pushButton_5_clicked()
{
    ui->sw->setCurrentIndex(3);
    setActiveSection("GHIJ");
}

void flightstatus::on_pushButton_6_clicked()
{
    ui->sw->setCurrentIndex(4);
    setActiveSection("KLMN");
}

void flightstatus::on_pushButton_7_clicked()
{
    ui->sw->setCurrentIndex(5);
    setActiveSection("PQRST");
}

void flightstatus::on_pushButton_8_clicked()
{
    ui->sw->setCurrentIndex(6);
    setActiveSection("XYZ");
}

void flightstatus::on_btn_popuplar_clicked()
{
    ui->sw->setCurrentIndex(1);
    setActiveSection("HOT");
}

void flightstatus::on_btn_domestic_clicked()
{
    ui->btn_abroad->setChecked(false);
    ui->btn_domestic->setChecked(true);
    ui->top_sw->setCurrentIndex(0);
    ui->sw->setCurrentIndex(1);
}

void flightstatus::on_btn_abroad_clicked()
{
    ui->btn_domestic->setChecked(false);
    ui->btn_abroad->setChecked(true);
    ui->top_sw->setCurrentIndex(1);
    ui->sw->setCurrentIndex(0);
}

void flightstatus::on_pushButton_9_clicked()
{
    ui->sw_2->setCurrentIndex(2);
    setActiveSection_2("ABCDEF");
}

void flightstatus::on_pushButton_10_clicked()
{
    ui->sw_2->setCurrentIndex(3);
    setActiveSection_2("GHIJ");
}

void flightstatus::on_pushButton_11_clicked()
{
    ui->sw_2->setCurrentIndex(4);
    setActiveSection_2("KLMN");
}

void flightstatus::on_pushButton_12_clicked()
{
    ui->sw_2->setCurrentIndex(5);
    setActiveSection_2("PQRST");
}

void flightstatus::on_pushButton_13_clicked()
{
    ui->sw_2->setCurrentIndex(6);
    setActiveSection_2("XYZ");
}

void flightstatus::on_btn_popuplar_2_clicked()
{
    ui->sw_2->setCurrentIndex(1);
    setActiveSection_2("HOT");
}

void flightstatus::on_btn_domestic_2_clicked()
{
    ui->btn_abroad_2->setChecked(false);
    ui->btn_domestic_2->setChecked(true);
    ui->top_sw_2->setCurrentIndex(0);
    ui->sw_2->setCurrentIndex(1);
}

void flightstatus::on_btn_abroad_2_clicked()
{
    ui->btn_domestic_2->setChecked(false);
    ui->btn_abroad_2->setChecked(true);
    ui->top_sw_2->setCurrentIndex(1);
    ui->sw_2->setCurrentIndex(0);
}
void flightstatus::setActiveSection(const QString &section)
{
    for (QPushButton *button : checkableButtons) {
        button->setChecked(false);
    }
    if (section == " ") {
        ui->btn_domestic->setChecked(true);
        ui->btn_popuplar->setChecked(true);
    }
    // 根据分区名称激活对应的按钮
    if (section == "HOT") {
        ui->btn_popuplar->setChecked(true);
    }
    if (section == "ABCDEF") {
        ui->pushButton_4->setChecked(true);
    } else if (section == "GHIJ") {
        ui->pushButton_5->setChecked(true);
    } else if (section == "KLMN") {
        ui->pushButton_6->setChecked(true);
    } else if (section == "PQRST") {
        ui->pushButton_7->setChecked(true);
    } else if (section == "XYZ") {
        ui->pushButton_8->setChecked(true);
    }
}
void flightstatus::setActiveSection_2(const QString &section)
{
    for (QPushButton *button : checkableButtons_2) {
        button->setChecked(false);
    }
    if (section == " ") {
        ui->btn_domestic_2->setChecked(true);
        ui->btn_popuplar_2->setChecked(true);
    }
    // 根据分区名称激活对应的按钮
    if (section == "HOT") {
        ui->btn_popuplar_2->setChecked(true);
    }
    if (section == "ABCDEF") {
        ui->pushButton_9->setChecked(true);
    } else if (section == "GHIJ") {
        ui->pushButton_10->setChecked(true);
    } else if (section == "KLMN") {
        ui->pushButton_11->setChecked(true);
    } else if (section == "PQRST") {
        ui->pushButton_12->setChecked(true);
    } else if (section == "XYZ") {
        ui->pushButton_13->setChecked(true);
    }
}
void flightstatus::init()
{
    calenIni();
    QList<QPushButton *> allButtons = ui->pick_widget->findChildren<QPushButton *>();
    // 2. 筛选名字以 "pushbutton" 或 "btn" 开头的控件
    for (QPushButton *button : allButtons) {
        QString objectName = button->objectName();
        if (objectName.startsWith("pushButton") || objectName.startsWith("btn_")) {
            if (objectName.startsWith("btn_p") || objectName.startsWith("pushButton"))
                checkableButtons.append(button);
            button->setCheckable(true);
        }
    }
    QList<QPushButton *> allButtons_2 = ui->pick_widget_2->findChildren<QPushButton *>();
    // 2. 筛选名字以 "pushbutton" 或 "btn" 开头的控件
    for (QPushButton *button : allButtons_2) {
        QString objectName = button->objectName();
        if (objectName.startsWith("pushButton") || objectName.startsWith("btn_")) {
            if (objectName.startsWith("btn_p") || objectName.startsWith("pushButton"))
                checkableButtons_2.append(button);
            button->setCheckable(true);
        }
    }
    setActiveSection(" ");
    setActiveSection_2(" ");
    QString buttonStyle = R"(
QPushButton {
    background-color: transparent;  /* 设置背景透明 */
    border: none;  /* 无边框 */
    color: black;  /* 文字颜色为黑色 */
}

QPushButton:hover {
    cursor: pointer;
    background-color: #E5F1FB;  /* 鼠标悬浮时背景颜色为浅蓝色 */
    color: #0078D7;  /* 鼠标悬浮时文字颜色为深蓝色 */
}
)";
    ui->pick_widget->setVisible(false);
    for (int i = 0; i < ui->sw->count(); i++) {
        QWidget *widget = ui->sw->widget(i);
        QList<QPushButton *> buttonsInpage = widget->findChildren<QPushButton *>();
        for (QPushButton *button : buttonsInpage) {
            button->setStyleSheet(buttonStyle);
            connect(button, &QPushButton::clicked, [button, this]() {
                ui->departureInput->setText(button->text());
                ui->pick_widget->setVisible(false);
                ui->label_90->setFocus();
            });
        }
    }
    ui->pick_widget_2->setVisible(false);
    for (int i = 0; i < ui->sw_2->count(); i++) {
        QWidget *widget = ui->sw_2->widget(i);
        QList<QPushButton *> buttonsInpage = widget->findChildren<QPushButton *>();
        for (QPushButton *button : buttonsInpage) {
            button->setStyleSheet(buttonStyle);
            connect(button, &QPushButton::clicked, [button, this]() {
                ui->destinationInput->setText(button->text());
                ui->pick_widget_2->setVisible(false);
                ui->label_90->setFocus();
            });
        }
    }
    // 初始化城市列表
    cityList << "安庆" << "anqing"
             << "安阳" << "anyang"
             << "北京" << "beijing"
             << "包头" << "baotou"
             << "北平" << "beipin"
             << ""
             << "成都" << "chengdu"
             << "重庆" << "chongqing"
             << "长沙" << "changsha"
             << "长春" << "changchun"
             << "常州" << "changzhou"
             << "大连" << "dalian"
             << "东莞" << "dongguan"
             << "丹东" << "dandong"
             << "敦煌" << "dunhuang"
             << "大理" << "dali"
             << "鄂尔多斯" << "eerduosi"
             << "恩施" << "enshi"
             << "福州" << "fuzhou"
             << "阜阳" << "fuyang"
             << "佛山" << "foshan"
             << "广州" << "guangzhou"
             << "桂林" << "guilin"
             << "贵阳" << "guiyang"
             << "赣州" << "ganzhou"
             << "杭州" << "hangzhou"
             << "合肥" << "hefei"
             << "哈尔滨" << "haerbin"
             << "呼和浩特" << "huhehaote"
             << "海口" << "haikou"
             << "邯郸" << "handan"
             << "惠州" << "huizhou"
             << "黄山" << "huangshan"
             << "济南" << "jinan"
             << "吉林" << "jilin"
             << "锦州" << "jianzhou"
             << "嘉峪关" << "jiayuguan"
             << "景德镇" << "jingdezhen"
             << "昆明" << "kunming"
             << "喀什" << "kashi"
             << "克拉玛依" << "kelamayi"
             << "兰州" << "lanzhou"
             << "洛阳" << "luoyang"
             << "丽江" << "lijian"
             << "柳州" << "liuzhou"
             << "临沂" << "linyi"
             << "绵阳" << "mianyang"
             << "梅州" << "meizhou"
             << "南京" << "nanjing"
             << "南昌" << "nanchang"
             << "南宁" << "nanning"
             << "宁波" << "ningbo"
             << "南阳" << "nanyang"
             << "攀枝花" << "panzhihua"
             << "青岛" << "qingdao"
             << "秦皇岛" << "qinhaungdao"
             << "齐齐哈尔" << "qiqihaer"
             << "衢州" << "quzhou"
             << "上海" << "shanghai"
             << "深圳" << "shenzhen"
             << "石家庄" << "shijiazhuang"
             << "汕头" << "shantou"
             << "三亚" << "sanya"
             << "苏州" << "suzhou"
             << "绍兴" << "shaoxing"
             << "天津" << "tianjin"
             << "太原" << "taiyuan"
             << "通辽" << "tongliao"
             << "台州" << "taizhou"
             << "武汉" << "wuhan"
             << "无锡" << "wuxi"
             << "温州" << "wenzhou"
             << "威海" << "weihai"
             << "乌鲁木齐" << "wulumuqi"
             << "西安" << "xian"
             << "厦门" << "xiamen"
             << "西宁" << "xining"
             << "徐州" << "xuzhou"
             << "襄阳" << "xiangyang"
             << "延吉" << "yanji"
             << "银川" << "yinchuan"
             << "宜昌" << "yichang"
             << "烟台" << "yantai"
             << "运城" << "yuncheng"
             << "郑州" << "zhengzhou"
             << "珠海" << "zhuhai"
             << "张家界" << "zhangjiajie"
             << "中山" << "zhongshan"
             << "遵义" << "zunyi";

    // 设置补全器
    setupCompleter();
}

void flightstatus::calenIni()
{
    ui->lbl_db->setVisible(false);
    ui->calendar->setHidden(true);
    ui->dateEditPushbutton->setStyleSheet("QPushButton {"
                                          "background: transparent;" // 背景透明
                                          "border: none;"            // 无边框
                                          "}");
    ui->dateEditPushbutton_2->setStyleSheet("QPushButton {"
                                            "background: transparent;" // 背景透明
                                            "border: none;"            // 无边框
                                            "}");
    //ui->dateEdit->setPlaceholderText("选择出发日期");
    //ui->dateEdit_2->setPlaceholderText("选择返回日期");
    ui->dateEdit->setReadOnly(true); // 设置为只读
    ui->frame->setVisible(false);
    ui->dateEdit_2->setVisible(false);
    ui->radioButton->isChecked();
    connect(ui->calendar, &QCalendarWidget::selectionChanged, [this]() {
        QDate selectedDate = ui->calendar->selectedDate();
        QLocale locale = QLocale(QLocale::Chinese, QLocale::China); // 设置区域为中国
        QString dateString = selectedDate.toString("yyyy-MM-dd");
        QString dayOfWeek = locale.dayName(selectedDate.dayOfWeek());
        if (ui->calendar->pos() == QPoint(680, 131))
            ui->dateEdit->setText(dateString + " " + dayOfWeek);
        else
            ui->dateEdit_2->setText(dateString + " " + dayOfWeek);
        ui->calendar->setHidden(true);
        ui->label_75->setFocus();
    });
    connect(ui->dateEditPushbutton, &QPushButton::clicked, [this]() {
        ui->calendar->setHidden(!ui->calendar->isHidden()); // 切换日历的显示状态
        ui->calendar->move(680, 131);
        ui->pick_widget->setVisible(false);
        ui->pick_widget_2->setVisible(false);
    });
    connect(ui->dateEditPushbutton_2, &QPushButton::clicked, [this]() {
        ui->calendar->setHidden(!ui->calendar->isHidden()); // 切换日历的显示状态
        ui->calendar->move(840, 131);
        ui->pick_widget->setVisible(false);
        ui->pick_widget_2->setVisible(false);
    });
    connect(ui->radioButton, &QRadioButton::clicked, [this]() {
        ui->frame->setVisible(false);
        ui->dateEdit_2->setVisible(false);
        ui->lbl_db->setVisible(false);
    });
    connect(ui->radioButton_2, &QRadioButton::clicked, [this]() {
        ui->frame->setVisible(true);
        ui->dateEdit_2->setVisible(true);
        ui->lbl_db->setVisible(true);
    });
}

void flightstatus::on_searchButton_clicked()
{
    // 创建 JSON 对象
    QJsonObject json;
    json["departure"] = ui->departureInput->text();
    json["destination"] = ui->destinationInput->text();

    // 设置请求
    QNetworkRequest request(QUrl("http://127.0.0.1:8080/api/flights/search"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 发送 POST 请求
    QNetworkReply *reply = networkmanager->post(request, QJsonDocument(json).toJson());

    // 处理响应
    connect(reply, &QNetworkReply::finished, [reply,this]() {
        ui->listWidget->clear();
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObject = jsonResponse.object();
            if (jsonObject["code"].toInt() == 200) {
                QJsonArray flights = jsonObject["data"].toArray();
                for (const QJsonValue &flightValue : flights) {
                    QString date = ui->dateEdit->text();
                    qint64 timestamp = flightValue["departureTime"].toVariant().toLongLong();
                    QDateTime dateTime = QDateTime::fromSecsSinceEpoch(timestamp);
                    QString dateStr = dateTime.toString("yyyy-MM-dd");
                    if(date.mid(0,10) != dateStr) continue;
                    QJsonObject flightObject = flightValue.toObject();
                    listItem* li = new listItem();
                    flightInfo fliInfo;
                    fliInfo.fromJson(flightObject);
                    li->setFlightDetails(fliInfo);
                    QListWidgetItem *item = new QListWidgetItem(ui->listWidget); // 假设你的列表控件叫 listWidget
                    item->setSizeHint(li->sizeHint());
                    ui->listWidget->addItem(item);
                    ui->listWidget->setItemWidget(item, li);
                }
            } else {
                qDebug() << "查询错误:" << jsonObject["message"].toString();
            }
        } else {
            qDebug() << "查询连接错误:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

void flightstatus::on_btn_lowPrice_clicked()
{
    // 创建 JSON 对象
    QJsonObject json;
    json["departure"] = ui->departureInput->text();
    json["destination"] = ui->destinationInput->text();

    // 设置请求
    QNetworkRequest request(QUrl("http://127.0.0.1:8080/api/flights/search"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 发送 POST 请求
    QNetworkReply *reply = networkmanager->post(request, QJsonDocument(json).toJson());

    // 处理响应
    connect(reply, &QNetworkReply::finished, [reply,this]() {
        ui->listWidget->clear();
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObject = jsonResponse.object();
            if (jsonObject["code"].toInt() == 200) {
                QJsonArray flights = jsonObject["data"].toArray();
                std::vector<QJsonObject> flightList;
                for (const QJsonValue &value : flights) {
                    flightList.push_back(value.toObject());
                }

                std::sort(flightList.begin(), flightList.end(), [](const QJsonObject &a, const QJsonObject &b) {
                    return a["economyClassPrice"].toDouble() < b["economyClassPrice"].toDouble();
                });

                // 将排序后的对象重新放入QJsonArray
                flights = QJsonArray(); // 清空原数组
                for (const QJsonObject &obj : flightList) {
                    flights.push_back(obj);
                }
                for (const QJsonValue &flightValue : flights) {
                    QString date = ui->dateEdit->text();
                    qint64 timestamp = flightValue["departureTime"].toVariant().toLongLong();
                    QDateTime dateTime = QDateTime::fromSecsSinceEpoch(timestamp);
                    QString dateStr = dateTime.toString("yyyy-MM-dd");
                    if(date.mid(0,10) != dateStr) continue;
                    QJsonObject flightObject = flightValue.toObject();
                    listItem* li = new listItem();
                    flightInfo fliInfo;
                    fliInfo.fromJson(flightObject);
                    li->setFlightDetails(fliInfo);
                    QListWidgetItem *item = new QListWidgetItem(ui->listWidget); // 假设你的列表控件叫 listWidget
                    item->setSizeHint(li->sizeHint());
                    ui->listWidget->addItem(item);
                    ui->listWidget->setItemWidget(item, li);
                }
            } else {
                qDebug() << "查询错误:" << jsonObject["message"].toString();
            }
        } else {
            qDebug() << "查询连接错误:" << reply->errorString();
        }
        reply->deleteLater();
    });
}


void flightstatus::on_btn_takeoff_early_clicked()
{
    // 创建 JSON 对象
    QJsonObject json;
    json["departure"] = ui->departureInput->text();
    json["destination"] = ui->destinationInput->text();

    // 设置请求
    QNetworkRequest request(QUrl("http://127.0.0.1:8080/api/flights/search"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 发送 POST 请求
    QNetworkReply *reply = networkmanager->post(request, QJsonDocument(json).toJson());

    // 处理响应
    connect(reply, &QNetworkReply::finished, [reply,this]() {
        ui->listWidget->clear();
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObject = jsonResponse.object();
            if (jsonObject["code"].toInt() == 200) {
                QJsonArray flights = jsonObject["data"].toArray();
                std::vector<QJsonObject> flightList;
                for (const QJsonValue &value : flights) {
                    flightList.push_back(value.toObject());
                }

                std::sort(flightList.begin(), flightList.end(), [](const QJsonObject &a, const QJsonObject &b) {
                    return a["departureTime"].toDouble() < b["departureTime"].toDouble();
                });

                // 将排序后的对象重新放入QJsonArray
                flights = QJsonArray(); // 清空原数组
                for (const QJsonObject &obj : flightList) {
                    flights.push_back(obj);
                }
                for (const QJsonValue &flightValue : flights) {
                    QString date = ui->dateEdit->text();
                    qint64 timestamp = flightValue["departureTime"].toVariant().toLongLong();
                    QDateTime dateTime = QDateTime::fromSecsSinceEpoch(timestamp);
                    QString dateStr = dateTime.toString("yyyy-MM-dd");
                    if(date.mid(0,10) != dateStr) continue;
                    QJsonObject flightObject = flightValue.toObject();
                    listItem* li = new listItem();
                    flightInfo fliInfo;
                    fliInfo.fromJson(flightObject);
                    li->setFlightDetails(fliInfo);
                    QListWidgetItem *item = new QListWidgetItem(ui->listWidget); // 假设你的列表控件叫 listWidget
                    item->setSizeHint(li->sizeHint());
                    ui->listWidget->addItem(item);
                    ui->listWidget->setItemWidget(item, li);
                }
            } else {
                qDebug() << "查询错误:" << jsonObject["message"].toString();
            }
        } else {
            qDebug() << "查询连接错误:" << reply->errorString();
        }
        reply->deleteLater();
    });
}


void flightstatus::on_btn_period_short_clicked()
{
    // 创建 JSON 对象
    QJsonObject json;
    json["departure"] = ui->departureInput->text();
    json["destination"] = ui->destinationInput->text();

    // 设置请求
    QNetworkRequest request(QUrl("http://127.0.0.1:8080/api/flights/search"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 发送 POST 请求
    QNetworkReply *reply = networkmanager->post(request, QJsonDocument(json).toJson());

    // 处理响应
    connect(reply, &QNetworkReply::finished, [reply,this]() {
        ui->listWidget->clear();
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObject = jsonResponse.object();
            if (jsonObject["code"].toInt() == 200) {
                QJsonArray flights = jsonObject["data"].toArray();
                std::vector<QJsonObject> flightList;
                for (const QJsonValue &value : flights) {
                    flightList.push_back(value.toObject());
                }

                std::sort(flightList.begin(), flightList.end(), [](const QJsonObject &a, const QJsonObject &b) {
                    return a["arrivalTime"].toDouble()-a["departureTime"].toDouble() < b["arrivalTime"].toDouble()-b["departureTime"].toDouble();
                });

                // 将排序后的对象重新放入QJsonArray
                flights = QJsonArray(); // 清空原数组
                for (const QJsonObject &obj : flightList) {
                    flights.push_back(obj);
                }
                for (const QJsonValue &flightValue : flights) {
                    QString date = ui->dateEdit->text();
                    qint64 timestamp = flightValue["departureTime"].toVariant().toLongLong();
                    QDateTime dateTime = QDateTime::fromSecsSinceEpoch(timestamp);
                    QString dateStr = dateTime.toString("yyyy-MM-dd");
                    if(date.mid(0,10) != dateStr) continue;
                    QJsonObject flightObject = flightValue.toObject();
                    listItem* li = new listItem();
                    flightInfo fliInfo;
                    fliInfo.fromJson(flightObject);
                    li->setFlightDetails(fliInfo);
                    QListWidgetItem *item = new QListWidgetItem(ui->listWidget); // 假设你的列表控件叫 listWidget
                    item->setSizeHint(li->sizeHint());
                    ui->listWidget->addItem(item);
                    ui->listWidget->setItemWidget(item, li);
                }
            } else {
                qDebug() << "查询错误:" << jsonObject["message"].toString();
            }
        } else {
            qDebug() << "查询连接错误:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

