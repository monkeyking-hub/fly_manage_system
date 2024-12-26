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
#include <QDate>
#include <QDateEdit>
#include "pay_window.h"
#include "ui_flightstatus.h"
#include "interfacemanager.h"
#include "listitem.h"

int cnt=0;

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
    ui->radioButton->setVisible(false);
    ui->radioButton_2->setVisible(false);
    ui->dateEditPushbutton_2->setVisible(false);
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
    ui->dp_airpot_cbx->setCurrentIndex(0);
    ui->ds_airpot_cbx->setCurrentIndex(0);
    ui->flight_company_cbx->setCurrentIndex(0);
    while (ui->dp_airpot_cbx->count() > 1) {
        ui->dp_airpot_cbx->removeItem(ui->dp_airpot_cbx->count() - 1);
    }
    while (ui->flight_company_cbx->count() > 1) {
        ui->flight_company_cbx->removeItem(ui->flight_company_cbx->count() - 1);
    }
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
    ui->dp_airpot_cbx->setCurrentIndex(0);
    ui->ds_airpot_cbx->setCurrentIndex(0);
    ui->flight_company_cbx->setCurrentIndex(0);
    while (ui->ds_airpot_cbx->count() > 1) {
        ui->ds_airpot_cbx->removeItem(ui->ds_airpot_cbx->count() - 1);
    }
    while (ui->flight_company_cbx->count() > 1) {
        ui->flight_company_cbx->removeItem(ui->flight_company_cbx->count() - 1);
    }
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
    ui->groupBox_4->setVisible(false);
    calenIni();
    btnIni();
    ui->radioButton->setChecked(true);
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
    cityList  << "阿尔山" << "aershan"
        << "阿勒泰" << "aletai"
        << "阿拉尔" << "alaer"
        << "安康" << "ankang"
        << "阿克苏" << "akesu"
        << "鞍山" << "anshan"
        << "安庆" << "anqing"
        << "阿拉善左旗" << "alashanzuoqi"
        << "毕节" << "bijie"
        << "北海" << "beihai"
        << "博乐" << "bole"
        << "保山" << "baoshan"
        << "巴中" << "bazhong"
        << "布尔津" << "buerjin"
        << "北京" << "beijing"
        << "白山" << "baishan"
        << "百色" << "baise"
        << "巴彦淖尔" << "bayannaoer"
        << "包头" << "baotou"
        << "成都" << "chengdu"
        << "常德" << "changde"
        << "长春" << "changchun"
        << "朝阳" << "chaoyang"
        << "赤峰" << "chifeng"
        << "长治" << "changzhi"
        << "重庆" << "chongqing"
        << "长沙" << "changsha"
        << "常州" << "changzhou"
        << "承德" << "chengde"
        << "郴州" << "chenzhou"
        << "池州" << "chizhou"
        << "大同" << "datong"
        << "丹东" << "dandong"
        << "迪庆" << "diqing"
        << "大连" << "dalian"
        << "大理" << "dali"
        << "敦煌" << "dunhuang"
        << "东营" << "dongying"
        << "大庆" << "daqing"
        << "达州" << "dazhou"
        << "德令哈" << "delingha"
        << "鄂州" << "ezhou"
        << "恩施" << "enshi"
        << "二连浩特" << "erlianhaote"
        << "福州" << "fuzhou"
        << "阜阳" << "fuyang"
        << "佛山" << "foshan"
        << "抚远" << "fuyuan"
        << "富蕴" << "fuyun"
        << "广州" << "guangzhou"
        << "果洛" << "guoluo"
        << "格尔木" << "geermu"
        << "广元" << "guangyuan"
        << "固原" << "guyuan"
        << "赣州" << "ganzhou"
        << "贵阳" << "guiyang"
        << "桂林" << "guilin"
        << "黄山" << "huangshan"
        << "合肥" << "hefei"
        << "海口" << "haikou"
        << "河池" << "hechi"
        << "邯郸" << "handan"
        << "黑河" << "heihe"
        << "呼和浩特" << "huhehaote"
        << "杭州" << "hangzhou"
        << "淮安" << "huaian"
        << "怀化" << "huaihua"
        << "海拉尔" << "hailaer"
        << "哈密" << "hami"
        << "衡阳" << "hengyang"
        << "哈尔滨" << "haerbin"
        << "和田" << "hetian"
        << "惠州" << "huizhou"
        << "菏泽" << "heze"
        << "汉中" << "hanzhong"
        << "荆州" << "jingzhou"
        << "揭阳" << "jieyang"
        << "济南" << "jinan"
        << "景德镇" << "jingdezhen"
        << "加格达奇" << "jiagedaqi"
        << "嘉峪关" << "jiayuguan"
        << "井冈山" << "jinggangshan"
        << "金昌" << "jinchang"
        << "九江" << "jiujiang"
        << "佳木斯" << "jiamusi"
        << "济宁" << "jining"
        << "锦州" << "jinzhou"
        << "鸡西" << "jixi"
        << "九寨沟" << "jiuzhaigou"
        << "库车" << "kuche"
        << "康定" << "kangding"
        << "喀什" << "kashi"
        << "昆明" << "kunming"
        << "库尔勒" << "kuerle"
        << "克拉玛依" << "kelamayi"
        << "临汾" << "linfen"
        << "兰州" << "lanzhou"
        << "丽江" << "lijiang"
        << "荔波" << "libo"
        << "吕梁" << "lvliang"
        << "临沧" << "lincang"
        << "六盘水" << "liupanshui"
        << "洛阳" << "luoyang"
        << "连云港" << "lianyungang"
        << "临沂" << "linyi"
        << "阆中" << "langzhong"
        << "柳州" << "liuzhou"
        << "泸州" << "luzhou"
        << "牡丹江" << "mudanjiang"
        << "绵阳" << "mianyang"
        << "梅州" << "meizhou"
        << "满洲里" << "manzhouli"
        << "漠河" << "mohe"
        << "南昌" << "nanchang"
        << "林芝" << "linzhi"
        << "南充" << "nanchong"
        << "宁波" << "ningbo"
        << "阿里" << "ali"
        << "南京" << "nanjing"
        << "宁蒗" << "ninglang"
        << "南宁" << "nanning"
        << "南阳" << "nanyang"
        << "南通" << "nantong"
        << "鄂尔多斯" << "eerduosi"
        << "普洱" << "puer"
        << "攀枝花" << "panzhihua"
        << "昌都" << "changdu"
        << "青岛" << "qingdao"
        << "祁连" << "qilian"
        << "且末" << "qiemo"
        << "庆阳" << "qingyang"
        << "奇台" << "qitai"
        << "黔江" << "qianjiang"
        << "泉州" << "quanzhou"
        << "衢州" << "quzhou"
        << "齐齐哈尔" << "qiqihaer"
        << "琼海" << "qionghai"
        << "上海" << "shanghai"
        << "石河子" << "shihezi"
        << "沈阳" << "shenyang"
        << "石家庄" << "shijiazhuang"
        << "上饶" << "shangrao"
        << "三亚" << "sanya"
        << "朔州" << "shuozhou"
        << "深圳" << "shenzhen"
        << "日喀则" << "rikaze"
        << "山南" << "shannan"
        << "塔城" << "tacheng"
        << "腾冲" << "tengchong"
        << "铜仁" << "tongren"
        << "通辽" << "tongliao"
        << "天水" << "tianshui"
        << "吐鲁番" << "tulufan"
        << "通化" << "tonghua"
        << "天津" << "tianjin"
        << "唐山" << "tangshan"
        << "图木舒克" << "tumushuke"
        << "太原" << "taiyuan"
        << "塔什库尔干" << "tashikuergan"
        << "台州" << "taizhou"
        << "乌兰浩特" << "wulanhaote"
        << "武隆" << "wulong"
        << "潍坊" << "weifang"
        << "威海" << "weihai"
        << "武汉" << "wuhan"
        << "文山" << "wenshan"
        << "温州" << "wenzhou"
        << "巫山" << "wushan"
        << "乌海" << "wuhai"
        << "无锡" << "wuxi"
        << "梧州" << "wuzhou"
        << "万州" << "wanzhou"
        << "湘西" << "xiangxi"
        << "忻州" << "xinzhou"
        << "信阳" << "xinyang"
        << "襄阳" << "xiangyang"
        << "西昌" << "xichang"
        << "锡林浩特" << "xilinhaote"
        << "西安" << "xian"
        << "厦门" << "xiamen"
        << "西宁" << "xining"
        << "邢台" << "xingtai"
        << "徐州" << "xuzhou"
        << "西双版纳" << "xishuangbanna"
        << "新源" << "xinyuan"
        << "兴义" << "xingyi"
        << "榆林" << "yulin"
        << "延安" << "yanan"
        << "宜宾" << "yibin"
        << "运城" << "yuncheng"
        << "宜春" << "yichun"
        << "宜昌" << "yichang"
        << "伊宁" << "yining"
        << "义乌" << "yiwu"
        << "玉林" << "yulin"
        << "延吉" << "yanji"
        << "烟台" << "yantai"
        << "盐城" << "yancheng"
        << "扬州" << "yangzhou"
        << "玉树" << "yushu"
        << "银川" << "yinchuan"
        << "伊春" << "yichun"
        << "永州" << "yongzhou"
        << "郑州" << "zhengzhou"
        << "张家界" << "zhangjiajie"
        << "舟山" << "zhoushan"
        << "张掖" << "zhangye"
        << "昭通" << "zhaotong"
        << "昭苏" << "zhaosu"
        << "珠海" << "zhuhai"
        << "湛江" << "zhanjiang"
        << "中卫" << "zhongwei"
        << "张家口" << "zhangjiakou"
        << "遵义" << "zunyi"
        << "扎兰屯" << "zhalantun"
        << "莎车" << "shache"
        << "白城" << "baicheng"
        << "霍林郭勒" << "huolinguole"
        << "建三江" << "jiansanjiang"
        << "陇南" << "longnan"
        << "茅台" << "maotai"
        << "芒市" << "mangshi"
        << "十堰" << "shiyan"
        << "松原" << "songyuan"
        << "韶关" << "shaoguan"
        << "三明" << "sanming"
        << "邵阳" << "shaoyang"
        << "于田" << "yutian"
        << "岳阳" << "yueyang"
        << "日照" << "rizhao"
        << "花土沟" << "huatugou"
        << "乌兰察布" << "wulanchabu"
        << "澜沧" << "lancang"
        << "五大连池" << "wudalianchi"
        << "秦皇岛" << "qinhuangdao"
        << "若羌" << "ruoqiang"
        << "沧源" << "cangyuan"
        << "红原" << "hongyuan"
        << "营口" << "yingkou";

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
    ui->dateEdit->setReadOnly(true); // 设置为只读
    ui->frame->setVisible(false);
    ui->dateEdit_2->setVisible(false);
    ui->radioButton->isChecked();
    connect(ui->calendar, &QCalendarWidget::selectionChanged, [this]() {
        emit flightstatus::columnchange();
        on_searchButton_clicked();
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
    connect(ui->flight_company_cbx, &QComboBox::currentTextChanged, [this](){
        clear_btn_state();
        on_searchButton_clicked();
    });
    connect(ui->dp_airpot_cbx, &QComboBox::currentTextChanged, this, [this](){
        clear_btn_state();
        on_searchButton_clicked();
    });
    connect(ui->ds_airpot_cbx, &QComboBox::currentTextChanged, this, [this](){
        clear_btn_state();
        on_searchButton_clicked();
    });
    connect(ui->flight_class_cbx, &QComboBox::currentTextChanged, this, [this](){
        clear_btn_state();
        on_searchButton_clicked();
    });
}

void flightstatus::on_searchButton_clicked()
{
    if(!cnt)
    {
        emit flightstatus::columnchange();
    }
    if(cnt==1)    ui->groupBox_4->setVisible(true);
    cnt++;
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
                QStringList qs,qap1,qap2;
                QJsonArray flights = jsonObject["data"].toArray();
                for (const QJsonValue &flightValue : flights) {
                    QString date = ui->dateEdit->text();
                    qint64 timestamp = flightValue["departureTime"].toVariant().toLongLong();
                    QDateTime dateTime = QDateTime::fromSecsSinceEpoch(timestamp);
                    QString dateStr = dateTime.toString("yyyy-MM-dd");
                    if(date.mid(0,10) != dateStr) continue;
                    qs.append(flightValue["airlineCompany"].toString());
                    qap1.append(flightValue["departureAirport"].toString());
                    qap2.append(flightValue["arrivalAirport"].toString());
                }
                updateAirlineCompanies(qs);
                updatedpAirports(qap1);
                updatedsAirports(qap2);
                for (const QJsonValue &flightValue : flights) {
                    QString date = ui->dateEdit->text();
                    qint64 timestamp = flightValue["departureTime"].toVariant().toLongLong();
                    QDateTime dateTime = QDateTime::fromSecsSinceEpoch(timestamp);
                    QString dateStr = dateTime.toString("yyyy-MM-dd");
                    if(date.mid(0,10) != dateStr) continue;
                    QString company = ui->flight_company_cbx->currentText();
                    QString departureAirport = ui->dp_airpot_cbx->currentText();
                    QString destinationAirport = ui->ds_airpot_cbx->currentText();
                    QString fcompany = flightValue["airlineCompany"].toString();
                    QString fdepartureAirport =flightValue["departureAirport"].toString();
                    QString fdestinationAirport = flightValue["arrivalAirport"].toString();
                    QString seatclass = ui->flight_class_cbx->currentText();
                    if(seatclass=="头等舱" && flightValue["firstClassSeats"]==0) continue;
                    if(seatclass=="经济舱" && flightValue["economyClassSeats"]==0) continue;
                    if(fcompany!=company && company != "不限航空公司") continue;
                    if(fdepartureAirport!=departureAirport && departureAirport != "不限出发机场") continue;
                    if(fdestinationAirport!=destinationAirport && destinationAirport != "不限抵达机场") continue;
                    QJsonObject flightObject = flightValue.toObject();
                    addListItem(InterfaceManager::instance()->pw,flightObject);
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
                QStringList qs,qap1,qap2;
                QJsonArray flights = jsonObject["data"].toArray();
                for (const QJsonValue &flightValue : flights) {
                    QString date = ui->dateEdit->text();
                    qint64 timestamp = flightValue["departureTime"].toVariant().toLongLong();
                    QDateTime dateTime = QDateTime::fromSecsSinceEpoch(timestamp);
                    QString dateStr = dateTime.toString("yyyy-MM-dd");
                    if(date.mid(0,10) != dateStr) continue;
                    qs.append(flightValue["airlineCompany"].toString());
                    qap1.append(flightValue["departureAirport"].toString());
                    qap2.append(flightValue["arrivalAirport"].toString());
                }
                updateAirlineCompanies(qs);
                updatedpAirports(qap1);
                updatedsAirports(qap2);
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
                    QString company = ui->flight_company_cbx->currentText();
                    QString departureAirport = ui->dp_airpot_cbx->currentText();
                    QString destinationAirport = ui->ds_airpot_cbx->currentText();
                    QString fcompany = flightValue["airlineCompany"].toString();
                    QString fdepartureAirport =flightValue["departureAirport"].toString();
                    QString fdestinationAirport = flightValue["arrivalAirport"].toString();
                    QString seatclass = ui->flight_class_cbx->currentText();
                    if(seatclass=="头等舱" && flightValue["firstClassSeats"]==0) continue;
                    if(seatclass=="经济舱" && flightValue["economyClassSeats"]==0) continue;
                    if(fcompany!=company && company != "不限航空公司") continue;
                    if(fdepartureAirport!=departureAirport && departureAirport != "不限出发机场") continue;
                    if(fdestinationAirport!=destinationAirport && destinationAirport != "不限抵达机场") continue;
                    QJsonObject flightObject = flightValue.toObject();
                    addListItem(InterfaceManager::instance()->pw,flightObject);
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
                QStringList qs,qap1,qap2;
                QJsonArray flights = jsonObject["data"].toArray();
                for (const QJsonValue &flightValue : flights) {
                    QString date = ui->dateEdit->text();
                    qint64 timestamp = flightValue["departureTime"].toVariant().toLongLong();
                    QDateTime dateTime = QDateTime::fromSecsSinceEpoch(timestamp);
                    QString dateStr = dateTime.toString("yyyy-MM-dd");
                    if(date.mid(0,10) != dateStr) continue;
                    qs.append(flightValue["airlineCompany"].toString());
                    qap1.append(flightValue["departureAirport"].toString());
                    qap2.append(flightValue["arrivalAirport"].toString());
                }
                updateAirlineCompanies(qs);
                updatedpAirports(qap1);
                updatedsAirports(qap2);
                std::vector<QJsonObject> flightList;
                for (const QJsonValue &value : flights) {
                    flightList.push_back(value.toObject());
                }

                std::sort(flightList.begin(), flightList.end(), [](const QJsonObject &a, const QJsonObject &b) {
                    return a["departureTime"].toInt() < b["departureTime"].toInt();
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
                    QString company = ui->flight_company_cbx->currentText();
                    QString departureAirport = ui->dp_airpot_cbx->currentText();
                    QString destinationAirport = ui->ds_airpot_cbx->currentText();
                    QString fcompany = flightValue["airlineCompany"].toString();
                    QString fdepartureAirport =flightValue["departureAirport"].toString();
                    QString fdestinationAirport = flightValue["arrivalAirport"].toString();
                    QString seatclass = ui->flight_class_cbx->currentText();
                    if(seatclass=="头等舱" && flightValue["firstClassSeats"]==0) continue;
                    if(seatclass=="经济舱" && flightValue["economyClassSeats"]==0) continue;
                    if(fcompany!=company && company != "不限航空公司") continue;
                    if(fdepartureAirport!=departureAirport && departureAirport != "不限出发机场") continue;
                    if(fdestinationAirport!=destinationAirport && destinationAirport != "不限抵达机场") continue;
                    QJsonObject flightObject = flightValue.toObject();
                    addListItem(InterfaceManager::instance()->pw,flightObject);
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
                QStringList qs,qap1,qap2;
                QJsonArray flights = jsonObject["data"].toArray();
                for (const QJsonValue &flightValue : flights) {
                    QString date = ui->dateEdit->text();
                    qint64 timestamp = flightValue["departureTime"].toVariant().toLongLong();
                    QDateTime dateTime = QDateTime::fromSecsSinceEpoch(timestamp);
                    QString dateStr = dateTime.toString("yyyy-MM-dd");
                    if(date.mid(0,10) != dateStr) continue;
                    qs.append(flightValue["airlineCompany"].toString());
                    qap1.append(flightValue["departureAirport"].toString());
                    qap2.append(flightValue["arrivalAirport"].toString());
                }
                updateAirlineCompanies(qs);
                updatedpAirports(qap1);
                updatedsAirports(qap2);
                std::vector<QJsonObject> flightList;
                for (const QJsonValue &value : flights) {
                    flightList.push_back(value.toObject());
                }

                std::sort(flightList.begin(), flightList.end(), [](const QJsonObject &a, const QJsonObject &b) {
                    return a["arrivalTime"].toDouble()-a["departureTime"].toDouble()<b["arrivalTime"].toDouble()-b["departureTime"].toDouble();
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
                    QString company = ui->flight_company_cbx->currentText();
                    QString departureAirport = ui->dp_airpot_cbx->currentText();
                    QString destinationAirport = ui->ds_airpot_cbx->currentText();
                    QString fcompany = flightValue["airlineCompany"].toString();
                    QString fdepartureAirport =flightValue["departureAirport"].toString();
                    QString fdestinationAirport = flightValue["arrivalAirport"].toString();
                    QString seatclass = ui->flight_class_cbx->currentText();
                    if(seatclass=="头等舱" && flightValue["firstClassSeats"]==0) continue;
                    if(seatclass=="经济舱" && flightValue["economyClassSeats"]==0) continue;
                    if(fcompany!=company && company != "不限航空公司") continue;
                    if(fdepartureAirport!=departureAirport && departureAirport != "不限出发机场") continue;
                    if(fdestinationAirport!=destinationAirport && destinationAirport != "不限抵达机场") continue;
                    QJsonObject flightObject = flightValue.toObject();
                    addListItem(InterfaceManager::instance()->pw,flightObject);
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

void flightstatus::addListItem(pay_window* payWin,QJsonObject flightObject)
{
    listItem* li = new listItem();
    flightInfo fliInfo;
    fliInfo.fromJson(flightObject,ui->flight_class_cbx->currentText());
    li->setFlightDetails(fliInfo);
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    item->setSizeHint(li->sizeHint());
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, li);

    // 确保传递正确的 pay_window 实例
    connect(li, &listItem::bookClicked, payWin, &pay_window::loadFliInfo);
}


void flightstatus::btnIni()
{
    const QString normalStyle = "QPushButton {"
                                "    background-color: transparent;"
                                "    border: none;"
                                "    color: black;"
                                "}"
                                "QPushButton:hover {"
                                "    cursor: pointer;"
                                "    background-color: #E5F1FB;"
                                "    color: #0078D7;"
                                "}";

    const QString selectedStyle = "QPushButton {"
                                  "    background-color: transparent;"
                                  "    border: none;"
                                  "    color: #0078D7;"
                                  "}";
    connect(ui->btn_lowPrice, &QPushButton::clicked, [this,normalStyle,selectedStyle]() {
        ui->btn_lowPrice->setStyleSheet(selectedStyle);
        ui->btn_takeoff_early->setStyleSheet(normalStyle);
        ui->btn_period_short->setStyleSheet(normalStyle);
    });
    connect(ui->btn_period_short, &QPushButton::clicked,[this,normalStyle,selectedStyle]() {
        ui->btn_period_short->setStyleSheet(selectedStyle);
        ui->btn_takeoff_early->setStyleSheet(normalStyle);
        ui->btn_lowPrice->setStyleSheet(normalStyle);
    });
    connect(ui->btn_takeoff_early, &QPushButton::clicked, [this,normalStyle,selectedStyle]() {
        ui->btn_takeoff_early->setStyleSheet(selectedStyle);
        ui->btn_period_short->setStyleSheet(normalStyle);
        ui->btn_lowPrice->setStyleSheet(normalStyle);
    });
    updateDateButtons();
}

void flightstatus::updateDateButtons() {
    QFont font;
    font.setPointSize(10);  // 设置字体大小为10点
    QPushButton* dateButtons[] = {ui->day1, ui->day2, ui->day3, ui->day4, ui->day5, ui->day6, ui->day7};

    for (int i = 0; i < 7; ++i) {
        dateButtons[i]->setFont(font);
        dateButtons[i]->setMinimumHeight(50); // 设置最小高度
        dateButtons[i]->setCheckable(true);
        connect(dateButtons[i], &QPushButton::clicked, [this, i, dateButtons] {
            clear_btn_state();
            dateButtons[i]->setChecked(true);
            for(int j=0;j<7;j++)
            {
                if(j==i) continue;
                dateButtons[j]->setChecked(false);
            }
            QLocale chineseLocale(QLocale::Chinese, QLocale::China);
            QDate date = QDate::fromString(dateButtons[i]->text().left(5), "MM-dd");
            QString formattedDate = date.toString("MM-dd");
            if(formattedDate.left(2)=="12") formattedDate="2024-"+formattedDate;
            else formattedDate="2025-"+formattedDate;
            QString dayOfWeek = chineseLocale.toString(date, "dddd");
            ui->dateEdit->setText(formattedDate + " " + dayOfWeek);  // 设置 dateEdit 的文本
            on_searchButton_clicked();
        });
    }
    connect(this, &flightstatus::columnchange, this, [this, dateButtons]() {
        QLocale chineseLocale(QLocale::Chinese, QLocale::China);
        QDate date = ui->calendar->selectedDate();
        for (int i = 0; i < 7; ++i) {
            dateButtons[i]->setText(date.addDays(i - 3).toString("MM-dd") + " " + chineseLocale.toString(date.addDays(i - 3), "dddd"));
            dateButtons[i]->setChecked(false);
        }
        dateButtons[3]->setChecked(true);
    });

}

void flightstatus::updateAirlineCompanies(const QStringList &companies) {
    for (const QString &company : companies) {
        if (!comboBoxContains(ui->flight_company_cbx, company)) {
            ui->flight_company_cbx->addItem(company);
        }
    }
}

void flightstatus::updatedpAirports(const QStringList &airports) {
    for (const QString &airport : airports) {
        if (!comboBoxContains(ui->dp_airpot_cbx, airport)) {
            ui->dp_airpot_cbx->addItem(airport);
        }
    }
}

void flightstatus::updatedsAirports(const QStringList &airports) {
    for (const QString &airport : airports) {
        if (!comboBoxContains(ui->ds_airpot_cbx, airport)) {
            ui->ds_airpot_cbx->addItem(airport);
        }
    }
}

bool flightstatus::comboBoxContains(QComboBox *comboBox, const QString &text) {
    for (int index = 0; index < comboBox->count(); index++) {
        if (comboBox->itemText(index) == text) {
            return true; // 发现已存在
        }
    }
    return false; // 不存在
}

void flightstatus:: clear_btn_state()
{
    const QString normalStyle = "QPushButton {"
                                "    background-color: transparent;"
                                "    border: none;"
                                "    color: black;"
                                "}"
                                "QPushButton:hover {"
                                "    cursor: pointer;"
                                "    background-color: #E5F1FB;"
                                "    color: #0078D7;"
                                "}";
    ui->btn_lowPrice->setStyleSheet(normalStyle);
    ui->btn_period_short->setStyleSheet(normalStyle);
    ui->btn_takeoff_early->setStyleSheet(normalStyle);
}








