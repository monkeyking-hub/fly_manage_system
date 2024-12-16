#include "orderdetailwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QIcon>
#include <QFont>
#include <QPixmap>
#include <QTableWidget>
#include <QHeaderView>
#include "maininterface.h"

OrderDetailWindow::OrderDetailWindow(const Order &order, QWidget *parent)
    : QWidget(parent)
{
    setupUI(order);
}

void OrderDetailWindow::setupUI(const Order &order)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 字体设置
    QFont titleFont = QFont("Arial", 14, QFont::Bold);  // 更大字体
    QFont contentFont = QFont("Arial", 12);
    QPalette palette;
    palette.setColor(QPalette::WindowText, QColor("#008000")); // 绿色


    // 订单号
    QLabel *orderNumberLabel = new QLabel(QString("订单号: %1").arg(order.orderNumber()), this);
    orderNumberLabel->setFont(titleFont);
    orderNumberLabel->setPalette(palette);
    layout->addWidget(orderNumberLabel);

    // 出行人信息
    QLabel *passengerLabel = new QLabel(QString("出行人信息: %1").arg(order.passenger()), this);
    passengerLabel->setFont(titleFont);
    layout->addWidget(passengerLabel);

    // 预订日期
    QLabel *bookingDateLabel = new QLabel(QString("预订日期: %1").arg(order.bookingDate()), this);
    bookingDateLabel->setFont(titleFont);
    layout->addWidget(bookingDateLabel);

    // 金额
    QLabel *amountLabel = new QLabel(QString("金额: %1").arg(order.amount()), this);
    amountLabel->setFont(titleFont);
    layout->addWidget(amountLabel);





    // 评价模块
    QLabel *evaluationLabel = new QLabel("您对此次出行满意吗？", this);
    evaluationLabel->setFont(titleFont);
    layout->addWidget(evaluationLabel);

    QVBoxLayout *ratingLayout = new QVBoxLayout();

    // 星星评分
    QHBoxLayout *starsLayout = new QHBoxLayout();
    for (int i = 0; i < 5; ++i) {
        QPushButton *starButton = new QPushButton(QIcon(":/newicon/star.png"), "", this);
        starButton->setIconSize(QSize(30, 30));
        starButton->setFlat(true);
        starsLayout->addWidget(starButton);

        // 点击事件：更新评价
        connect(starButton, &QPushButton::clicked, this, [this, i]() {
            QString message;
            if (i == 0 || i == 1) {
                message = "差评，感谢反馈，我们会积极改正";
            } else if (i == 2) {
                message = "还行，我们会继续努力";
            } else {
                message = "很满意，我们会继续加油";
            }
            QMessageBox::information(this, "评价", message);
        });
    }

    ratingLayout->addLayout(starsLayout);

    // 表情与说明文字对齐
    QHBoxLayout *facesLayout = new QHBoxLayout();

    QVBoxLayout *cryFaceLayout = new QVBoxLayout();
    QPushButton *cryFaceButton = new QPushButton(QIcon(":/newicon/bad.png"), "", this);
    cryFaceButton->setIconSize(QSize(30, 30));
    cryFaceButton->setFlat(true);
    cryFaceLayout->addWidget(cryFaceButton, 0, Qt::AlignHCenter);
    QLabel *cryFaceLabel = new QLabel("不满意", this);
    cryFaceLabel->setAlignment(Qt::AlignHCenter);
    cryFaceLayout->addWidget(cryFaceLabel);

    QVBoxLayout *normalFaceLayout = new QVBoxLayout();
    QPushButton *normalFaceButton = new QPushButton(QIcon(":/newicon/nor.png"), "", this);
    normalFaceButton->setIconSize(QSize(30, 30));
    normalFaceButton->setFlat(true);
    normalFaceLayout->addWidget(normalFaceButton, 0, Qt::AlignHCenter);
    QLabel *normalFaceLabel = new QLabel("一般", this);
    normalFaceLabel->setAlignment(Qt::AlignHCenter);
    normalFaceLayout->addWidget(normalFaceLabel);

    QVBoxLayout *smileFaceLayout = new QVBoxLayout();
    QPushButton *smileFaceButton = new QPushButton(QIcon(":/newicon/good.png"), "", this);
    smileFaceButton->setIconSize(QSize(30, 30));
    smileFaceButton->setFlat(true);
    smileFaceLayout->addWidget(smileFaceButton, 0, Qt::AlignHCenter);
    QLabel *smileFaceLabel = new QLabel("满意", this);
    smileFaceLabel->setAlignment(Qt::AlignHCenter);
    smileFaceLayout->addWidget(smileFaceLabel);

    facesLayout->addLayout(cryFaceLayout);
    facesLayout->addLayout(normalFaceLayout);
    facesLayout->addLayout(smileFaceLayout);
    ratingLayout->addLayout(facesLayout);

    layout->addLayout(ratingLayout);






    // 常见问题模块
    QLabel *faqLabel = new QLabel("常见问题", this);
    faqLabel->setFont(titleFont);
    layout->addWidget(faqLabel);

    QVBoxLayout *faqLayout = new QVBoxLayout();

    // 常见问题按钮
    QPushButton *serviceIssueButton = new QPushButton("服务态度差", this);
    QPushButton *flightDelayButton = new QPushButton("航班晚点", this);
    QPushButton *baggageRuleButton = new QPushButton("行李重量/尺寸规定", this);
    QPushButton *poorExperienceButton = new QPushButton("候机体验不好", this);
    QPushButton *complaintButton = new QPushButton("我要投诉", this);

    faqLayout->addWidget(serviceIssueButton);
    faqLayout->addWidget(flightDelayButton);
    faqLayout->addWidget(baggageRuleButton);
    faqLayout->addWidget(poorExperienceButton);
    faqLayout->addWidget(complaintButton);

    layout->addLayout(faqLayout);

    // 点击"行李重量/尺寸规定"显示表格
    connect(baggageRuleButton, &QPushButton::clicked, this, [this]() {
        showBaggageTable();
    });


    // 连接信号与槽函数
    connect(serviceIssueButton, &QPushButton::clicked, this, [this]() {
        QMessageBox::information(this, "反馈", "感谢反馈，我们会积极改正");
    });

    connect(flightDelayButton, &QPushButton::clicked, this, [this]() {
        QMessageBox::information(this, "反馈", "感谢反馈，我们会积极改正");
    });

    // 点击按钮显示表格
    connect(baggageRuleButton, &QPushButton::clicked, this, [this]() {
        showBaggageTable();
    });

    connect(poorExperienceButton, &QPushButton::clicked, this, [this]() {
        QMessageBox::information(this, "反馈", "感谢反馈，我们会积极改正");
    });

    // 连接按钮的点击事件到槽函数
    connect(complaintButton, &QPushButton::clicked, this, [=]() {
        // 关闭当前窗口
        this->close();
        emit complaintButtonClicked();
    });

    // 窗口设置
    setWindowTitle("订单详情");
    setFixedSize(702, 800); // 调整窗口大小
    setLayout(layout);


}

// 显示行李重量/尺寸规定表格
void OrderDetailWindow::showBaggageTable()
{
    QWidget *tableWindow = new QWidget;
    tableWindow->setWindowTitle("行李重量/尺寸规定");
    tableWindow->resize(600, 400);

    QTableWidget *tableWidget = new QTableWidget(9, 3, tableWindow);  // 9行3列
    tableWidget->setHorizontalHeaderLabels(QStringList() << "尺寸" << "三边尺寸" << "三边之和不超过");
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 表格数据
    QStringList sizes = {"13寸拉杆箱尺寸", "18寸拉杆箱尺寸", "20寸拉杆箱尺寸",
                         "22寸拉杆箱尺寸", "24寸拉杆箱尺寸", "26寸拉杆箱尺寸",
                         "28寸拉杆箱尺寸", "30寸拉杆箱尺寸", "32寸拉杆箱尺寸"};
    QStringList dimensions = {"28cm*40cm*13cm", "34cm*44cm*20cm", "33cm*22cm*52cm",
                              "35cm*24cm*54cm", "37cm*29cm*64cm", "39cm*33cm*69cm",
                              "44cm*34cm*74cm", "44cm*34cm*78cm", "57cm*81cm*35cm"};
    QStringList totalSizes = {"81cm", "105cm", "115cm", "125cm", "135cm", "148cm", "158cm", "170cm", "195cm"};

    for (int i = 0; i < 9; ++i) {
        tableWidget->setItem(i, 0, new QTableWidgetItem(sizes[i]));
        tableWidget->setItem(i, 1, new QTableWidgetItem(dimensions[i]));
        tableWidget->setItem(i, 2, new QTableWidgetItem(totalSizes[i]));
    }

    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 禁止编辑
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);   // 禁止选中
    tableWidget->verticalHeader()->setVisible(false);                // 隐藏行号

    QVBoxLayout *layout = new QVBoxLayout(tableWindow);
    layout->addWidget(tableWidget);
    tableWindow->setLayout(layout);

    tableWindow->show();
}
