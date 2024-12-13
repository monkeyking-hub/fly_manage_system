#include "orderdetailwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QIcon>
#include <QFont>

OrderDetailWindow::OrderDetailWindow(const Order &order, QWidget *parent)
    : QWidget(parent)
{
    setupUI(order);
}

void OrderDetailWindow::setupUI(const Order &order)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 订单详细信息，标题加粗、字体增大
    QFont titleFont = QFont("Arial", 12, QFont::Bold);
    QFont contentFont = QFont("Arial", 10);

    QLabel *orderNumberLabel = new QLabel(QString("订单号: %1").arg(order.orderNumber()), this);
    orderNumberLabel->setFont(titleFont);
    layout->addWidget(orderNumberLabel);

    QLabel *passengerLabel = new QLabel(QString("乘客: %1").arg(order.passenger()), this);
    passengerLabel->setFont(titleFont);
    layout->addWidget(passengerLabel);

    QLabel *bookingDateLabel = new QLabel(QString("预订日期: %1").arg(order.bookingDate()), this);
    bookingDateLabel->setFont(titleFont);
    layout->addWidget(bookingDateLabel);

    QLabel *amountLabel = new QLabel(QString("金额: %1").arg(order.amount()), this);
    amountLabel->setFont(titleFont);
    layout->addWidget(amountLabel);

    QLabel *routeLabel = new QLabel(QString("行程: %1").arg(order.route()), this);
    routeLabel->setFont(titleFont);
    layout->addWidget(routeLabel);

    // 评价模块
    QLabel *evaluationLabel = new QLabel("订单评价", this);
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

    // 添加“哭脸”图标并加说明文字
    QVBoxLayout *cryFaceLayout = new QVBoxLayout();
    QPushButton *cryFaceButton = new QPushButton(QIcon(":/newicon/bad.png"), "", this);
    cryFaceButton->setIconSize(QSize(30, 30));
    cryFaceButton->setFlat(true);
    cryFaceLayout->addWidget(cryFaceButton);
    QLabel *cryFaceLabel = new QLabel("不满意", this);
    cryFaceLayout->addWidget(cryFaceLabel);

    // 添加“正常”图标并加说明文字
    QVBoxLayout *normalFaceLayout = new QVBoxLayout();
    QPushButton *normalFaceButton = new QPushButton(QIcon(":/newicon/nor.png"), "", this);
    normalFaceButton->setIconSize(QSize(30, 30));
    normalFaceButton->setFlat(true);
    normalFaceLayout->addWidget(normalFaceButton);
    QLabel *normalFaceLabel = new QLabel("一般", this);
    normalFaceLayout->addWidget(normalFaceLabel);

    // 添加“笑脸”图标并加说明文字
    QVBoxLayout *smileFaceLayout = new QVBoxLayout();
    QPushButton *smileFaceButton = new QPushButton(QIcon(":/newicon/good.png"), "", this);
    smileFaceButton->setIconSize(QSize(30, 30));
    smileFaceButton->setFlat(true);
    smileFaceLayout->addWidget(smileFaceButton);
    QLabel *smileFaceLabel = new QLabel("满意", this);
    smileFaceLayout->addWidget(smileFaceLabel);

    // 将三个布局（哭脸、正常、笑脸）添加到评价模块
    QHBoxLayout *facesLayout = new QHBoxLayout();
    facesLayout->addLayout(cryFaceLayout);
    facesLayout->addLayout(normalFaceLayout);
    facesLayout->addLayout(smileFaceLayout);
    ratingLayout->addLayout(facesLayout);

    layout->addLayout(ratingLayout);

    setWindowTitle("订单详情");
    setFixedSize(702, 627);  // 设置窗口固定大小
    setLayout(layout);
}
