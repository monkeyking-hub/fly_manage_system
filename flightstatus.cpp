#include "flightstatus.h"
#include <QCompleter>
#include <QDebug>
#include <QKeyEvent>
#include <QListView>
#include <QStringList>
#include "ui_flightstatus.h"

flightstatus::flightstatus(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::flightstatus)
{
    ui->setupUi(this);
    init();
    ui->departureInput->installEventFilter(this);
    ui->destinationInput->installEventFilter(this);
    connect(ui->departureInput, &QLineEdit::textChanged, this, [this](){ui->pick_widget->setVisible(false);});
    connect(ui->destinationInput, &QLineEdit::textChanged, this, [this](){ui->pick_widget_2->setVisible(false);});
    this->installEventFilter(this);
}

// 析构函数
flightstatus::~flightstatus()
{
    delete ui;
}

void flightstatus::showPickWidget(QLineEdit* qle)
{
    if(qle==ui->departureInput)
    {
    QPoint pos=QPoint(250,91);
    ui->pick_widget->move(pos);
    ui->pick_widget->setVisible(true);
    }
    if(qle==ui->destinationInput)
    {
        QPoint pos=QPoint(550,91);
        ui->pick_widget_2->move(pos);
        ui->pick_widget_2->setVisible(true);
    }
}

bool flightstatus::eventFilter(QObject* obj,QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        // 检查点击是否在特定控件上
        bool onDepartureInput = ui->departureInput->underMouse();
        bool onDestinationInput = ui->destinationInput->underMouse();
        bool onPickWidget = ui->pick_widget->underMouse();
        bool onPickWidget2 = ui->pick_widget_2->underMouse();

        // 如果没有点击在上述任何控件上，则隐藏
        if (!onDepartureInput && !onDestinationInput && !onPickWidget && !onPickWidget2) {
            ui->pick_widget->setVisible(false);
            ui->pick_widget_2->setVisible(false);
        }
    }

    if (obj == ui->departureInput || obj == ui->destinationInput) {
        if (event->type() == QEvent::FocusOut) {
            if (obj == ui->departureInput) {
                ui->pick_widget->setVisible(false);
            } else if (obj == ui->destinationInput) {
                ui->pick_widget_2->setVisible(false);
            }
            return false; // Continue the event propagation
        } else if (event->type() == QEvent::MouseButtonPress) {
            QLineEdit* qle = static_cast<QLineEdit*>(obj);
            showPickWidget(qle);
            return true; // Stop the event propagation
        }
    }
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
}


void flightstatus::on_pushButton_5_clicked()
{
    ui->sw->setCurrentIndex(3);
}


void flightstatus::on_pushButton_6_clicked()
{
    ui->sw->setCurrentIndex(4);
}


void flightstatus::on_pushButton_7_clicked()
{
    ui->sw->setCurrentIndex(5);
}


void flightstatus::on_pushButton_8_clicked()
{
    ui->sw->setCurrentIndex(6);
}


void flightstatus::on_btn_popuplar_clicked()
{
    ui->sw->setCurrentIndex(1);
}


void flightstatus::on_btn_domestic_clicked()
{
    ui->top_sw->setCurrentIndex(0);
    ui->sw->setCurrentIndex(1);
}


void flightstatus::on_btn_abroad_clicked()
{
    ui->top_sw->setCurrentIndex(1);
    ui->sw->setCurrentIndex(0);
}

void flightstatus::on_pushButton_9_clicked()
{
    ui->sw_2->setCurrentIndex(2);
}


void flightstatus::on_pushButton_10_clicked()
{
    ui->sw_2->setCurrentIndex(3);
}


void flightstatus::on_pushButton_11_clicked()
{
    ui->sw_2->setCurrentIndex(4);
}


void flightstatus::on_pushButton_12_clicked()
{
    ui->sw_2->setCurrentIndex(5);
}


void flightstatus::on_pushButton_13_clicked()
{
    ui->sw_2->setCurrentIndex(6);
}


void flightstatus::on_btn_popuplar_2_clicked()
{
    ui->sw_2->setCurrentIndex(1);
}

void flightstatus::on_btn_domestic_2_clicked()
{
    ui->top_sw_2->setCurrentIndex(0);
    ui->sw_2->setCurrentIndex(1);
}

void flightstatus::on_btn_abroad_2_clicked()
{
    ui->top_sw_2->setCurrentIndex(1);
    ui->sw_2->setCurrentIndex(0);
}

void flightstatus::init()
{
    ui->pick_widget->setVisible(false);
    for(int i=0;i<ui->sw->count();i++)
    {
        QWidget* widget=ui->sw->widget(i);
        QList<QPushButton*> buttonsInpage=widget->findChildren<QPushButton*>();

        for (QPushButton *button : buttonsInpage) {
            connect(button, &QPushButton::clicked, [button, this]() {
                ui->departureInput->setText(button->text());
                ui->pick_widget->setVisible(false);
            });
    }
    }
    ui->pick_widget_2->setVisible(false);
    for(int i=0;i<ui->sw_2->count();i++)
    {
        QWidget* widget=ui->sw_2->widget(i);
        QList<QPushButton*> buttonsInpage=widget->findChildren<QPushButton*>();

        for (QPushButton *button : buttonsInpage) {
            connect(button, &QPushButton::clicked, [button, this]() {
                ui->destinationInput->setText(button->text());
                ui->pick_widget_2->setVisible(false);
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














