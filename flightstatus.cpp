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

// 析构函数
flightstatus::~flightstatus()
{
    delete ui;
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

