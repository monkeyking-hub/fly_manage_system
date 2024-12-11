#include "flightstatus.h"
#include "ui_flightstatus.h"
#include <QStringList>
#include <QCompleter>
#include <QListView>
#include <QKeyEvent>
#include <QDebug>

flightstatus::flightstatus(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::flightstatus)
{
    ui->setupUi(this);

    // 初始化城市列表
    cityList << "A:安庆:anqing" << "A:安阳:anyang"
             << "B:北京:beijing" << "B:包头:baotou" << "B:北平:beipin" << ""
             << "C:成都:chengdu" << "C:重庆:chongqing" << "C:长沙:changsha" << "C:长春:changchun" << "C:常州:changzhou"
             << "D:大连:dalian" << "D:东莞:dongguan" << "D:丹东:dandong" << "D:敦煌:dunhuang" << "D:大理:dali"
             << "E:鄂尔多斯:eerduosi" << "E:恩施:enshi"
             << "F:福州:fuzhou" << "F:阜阳:fuyang" << "F:佛山:foshan"
             << "G:广州:guangzhou" << "G:桂林:guilin" << "G:贵阳:guiyang" << "G:赣州:ganzhou"
             << "H:杭州:hangzhou" << "H:合肥:hefei" << "H:哈尔滨:haerbin" << "H:呼和浩特:huhehaote" << "H:海口:haikou"
             << "H:邯郸:handan" << "H:惠州:huizhou" << "H:黄山:huangshan"
             << "J:济南:jinan" << "J:吉林:jilin" << "J:锦州:jianzhou" << "J:嘉峪关:jiayuguan" << "J:景德镇:jingdezhen"
             << "K:昆明:kunming" << "K:喀什:kashi" << "K:克拉玛依:kelamayi"
             << "L:兰州:lanzhou" << "L:洛阳:luoyang" << "L:丽江:lijian" << "L:柳州:liuzhou" << "L:临沂:linyi"
             << "M:绵阳:mianyang" << "M:梅州:meizhou"
             << "N:南京:nanjing" << "N:南昌:nanchang" << "N:南宁:nanning" << "N:宁波:ningbo" << "N:南阳:nanyang"
             << "P:攀枝花:panzhihua"
             << "Q:青岛:qingdao" << "Q:秦皇岛:qinhaungdao" << "Q:齐齐哈尔:qiqihaer" << "Q:衢州:quzhou"
             << "S:上海:shanghai" << "S:深圳:shenzhen" << "S:石家庄:shijiazhuang" << "S:汕头:shantou" << "S:三亚:sanya"
             << "S:苏州:suzhou" << "S:绍兴:shaoxing"
             << "T:天津:tianjin" << "T:太原:taiyuan" << "T:通辽:tongliao" << "T:台州:taizhou"
             << "W:武汉:wuhan" << "W:无锡:wuxi" << "W:温州:wenzhou" << "W:威海:weihai" << "W:乌鲁木齐:wulumuqi"
             << "X:西安:xian" << "X:厦门:xiamen" << "X:西宁:xining" << "X:徐州:xuzhou" << "X:襄阳:xiangyang"
             << "Y:延吉:yanji" << "Y:银川:yinchuan" << "Y:宜昌:yichang" << "Y:烟台:yantai" << "Y:运城:yuncheng"
             << "Z:郑州:zhengzhou" << "Z:珠海:zhuhai" << "Z:张家界:zhangjiajie" << "Z:中山:zhongshan" << "Z:遵义:zunyi";

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

// // 出发地输入框文本变化时触发
// void flightstatus::on_departureInput_textChanged(const QString &text)
// {
//     // 使用拼音或中文进行匹配
//     QStringList filteredCities;
//     for (const QString &city : cityList) {
//         if (city.contains(text, Qt::CaseInsensitive)) {
//             filteredCities << city;
//         }
//     }
//     cityListModel->setStringList(filteredCities);
// }

// // 目的地输入框文本变化时触发
// void flightstatus::on_destinationInput_textChanged(const QString &text)
// {
//     // 使用拼音或中文进行匹配
//     QStringList filteredCities;
//     for (const QString &city : cityList) {
//         if (city.contains(text, Qt::CaseInsensitive)) {
//             filteredCities << city;
//         }
//     }
//     cityListModel->setStringList(filteredCities);
// }

// 出发地输入框文本变化时触发
void flightstatus::on_departureInput_textChanged(const QString &text)
{
    // 使用拼音或中文进行匹配
    QStringList filteredCities;
    for (const QString &entry : cityList) {
        QStringList parts = entry.split(":");  // 格式: "字母:城市名:拼音"
        QString cityName = parts[1];           // 获取中文城市名
        QString cityPinyin = parts[2];         // 获取拼音

        // 检查中文名或拼音是否包含输入的文本
        if (cityName.contains(text, Qt::CaseInsensitive) || cityPinyin.startsWith(text, Qt::CaseInsensitive)) {
            qDebug()<<"cityName:"+cityName;
            qDebug()<<"pinNyin:"+cityPinyin;
            qDebug()<<"test:"+text;
            filteredCities << cityName;  // 只添加中文城市名
        }
    }
    cityListModel->setStringList(filteredCities);  // 更新匹配的城市名
}

// 目的地输入框文本变化时触发
void flightstatus::on_destinationInput_textChanged(const QString &text)
{
    // 使用拼音或中文进行匹配
    QStringList filteredCities;
    for (const QString &entry : cityList) {
        QStringList parts = entry.split(":");  // 格式: "字母:城市名:拼音"
        QString cityName = parts[1];           // 获取中文城市名
        QString cityPinyin = parts[2];         // 获取拼音

        // 检查中文名或拼音是否包含输入的文本
        if (cityName.contains(text, Qt::CaseInsensitive) || cityPinyin.contains(text, Qt::CaseInsensitive)) {
            filteredCities << cityName;  // 只添加中文城市名
        }
    }
    cityListModel->setStringList(filteredCities);  // 更新匹配的城市名
}
