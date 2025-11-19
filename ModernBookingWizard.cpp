#include "ModernBookingWizard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QScrollArea>
#include <QApplication>
#include <QScreen>
#include <QDebug>

ModernBookingWizard::ModernBookingWizard(const QJsonObject &flightData, QWidget *parent)
    : QMainWindow(parent), m_flightData(flightData), 
      networkManager(new QNetworkAccessManager(this)) {
    setupUI();
}

ModernBookingWizard::~ModernBookingWizard() {
}

void ModernBookingWizard::setupUI() {
    setWindowTitle("Sky Wings - Booking Wizard");
    resize(1000, 900);
    QRect screen = QApplication::primaryScreen()->geometry();
    move((screen.width() - width()) / 2, (screen.height() - height()) / 2);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(30);

    // ==================== 顶部进度条 ====================
    QLabel *title = new QLabel("Complete Your Booking");
    title->setStyleSheet(
        "QLabel {"
        "   color: #111827;"
        "   font-size: 28px;"
        "   font-weight: 700;"
        "}"
    );
    mainLayout->addWidget(title);

    QFrame *progressFrame = new QFrame();
    QHBoxLayout *progressLayout = new QHBoxLayout(progressFrame);
    progressLayout->setSpacing(10);

    QStringList steps = {"Passengers", "Seats", "Review", "Payment"};
    for (int i = 0; i < 4; ++i) {
        // 步骤圆圈
        QLabel *circle = new QLabel(QString::number(i + 1));
        circle->setAlignment(Qt::AlignCenter);
        if (i == 0) {
            circle->setStyleSheet(
                "QLabel {"
                "   background-color: #0052CC;"
                "   color: white;"
                "   border-radius: 20px;"
                "   width: 40px;"
                "   height: 40px;"
                "   font-weight: 700;"
                "   font-size: 14px;"
                "}"
            );
        } else {
            circle->setStyleSheet(
                "QLabel {"
                "   background-color: #E5E7EB;"
                "   color: #6B7280;"
                "   border-radius: 20px;"
                "   width: 40px;"
                "   height: 40px;"
                "   font-weight: 700;"
                "   font-size: 14px;"
                "}"
            );
        }
        circle->setFixedSize(40, 40);
        progressLayout->addWidget(circle, 0, Qt::AlignCenter);

        // 步骤标签
        QLabel *label = new QLabel(steps[i]);
        label->setStyleSheet(
            "QLabel {"
            "   color: " + QString(i == 0 ? "#0052CC" : "#9CA3AF") + ";"
            "   font-size: 12px;"
            "   font-weight: 600;"
            "}"
        );
        progressLayout->addWidget(label, 0, Qt::AlignCenter);

        // 分隔线
        if (i < 3) {
            QFrame *separator = new QFrame();
            separator->setStyleSheet("QFrame { background-color: #E5E7EB; }");
            separator->setFixedHeight(2);
            separator->setFixedWidth(40);
            progressLayout->addWidget(separator, 0, Qt::AlignCenter);
        }
    }
    progressLayout->addStretch();
    mainLayout->addWidget(progressFrame);

    // 分隔符
    QFrame *divider = new QFrame();
    divider->setStyleSheet("QFrame { background-color: #E5E7EB; }");
    divider->setFixedHeight(1);
    mainLayout->addWidget(divider);

    // ==================== 步骤内容 ====================
    stepsStack = new QStackedWidget();
    stepsStack->setStyleSheet("QStackedWidget { background-color: #F9FAFB; border-radius: 12px; }");

    createStep1PassengerInfo();
    createStep2SeatSelection();
    createStep3Review();
    createStep4Payment();

    mainLayout->addWidget(stepsStack, 1);

    // ==================== 底部按钮 ====================
    QFrame *buttonFrame = new QFrame();
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonFrame);
    buttonLayout->setSpacing(15);

    backBtn = new QPushButton("← Back");
    backBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: white;"
        "   color: #0052CC;"
        "   border: 2px solid #0052CC;"
        "   border-radius: 6px;"
        "   padding: 12px 24px;"
        "   font-weight: 600;"
        "   font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #F0F4FF;"
        "}"
        "QPushButton:disabled {"
        "   color: #9CA3AF;"
        "   border-color: #D1D5DB;"
        "}"
    );
    backBtn->setFixedWidth(120);
    backBtn->setEnabled(false);
    buttonLayout->addWidget(backBtn);

    buttonLayout->addStretch();

    nextBtn = new QPushButton("Next →");
    nextBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #0052CC;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 6px;"
        "   padding: 12px 24px;"
        "   font-weight: 600;"
        "   font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0048B8;"
        "}"
    );
    nextBtn->setFixedWidth(120);
    buttonLayout->addWidget(nextBtn);

    confirmBtn = new QPushButton("Confirm Booking");
    confirmBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #10B981;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 6px;"
        "   padding: 12px 24px;"
        "   font-weight: 600;"
        "   font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #059669;"
        "}"
    );
    confirmBtn->setFixedWidth(150);
    confirmBtn->setVisible(false);
    buttonLayout->addWidget(confirmBtn);

    mainLayout->addWidget(buttonFrame);

    connect(nextBtn, &QPushButton::clicked, this, &ModernBookingWizard::onNextClicked);
    connect(backBtn, &QPushButton::clicked, this, &ModernBookingWizard::onBackClicked);
    connect(confirmBtn, &QPushButton::clicked, this, &ModernBookingWizard::onConfirmClicked);
}

void ModernBookingWizard::createStep1PassengerInfo() {
    QWidget *step1 = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(step1);
    layout->setContentsMargins(30, 30, 30, 30);

    QLabel *title = new QLabel("Passenger Information");
    title->setStyleSheet("QLabel { color: #111827; font-size: 20px; font-weight: 700; }");
    layout->addWidget(title);

    QScrollArea *scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("QScrollArea { border: none; }");

    QWidget *scrollContent = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);

    // 创建乘客信息表单
    QGroupBox *passengerGroup = new QGroupBox("Passenger 1");
    passengerGroup->setStyleSheet(
        "QGroupBox {"
        "   color: #374151;"
        "   border: 1px solid #E5E7EB;"
        "   border-radius: 6px;"
        "   margin-top: 10px;"
        "   padding-top: 15px;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   left: 15px;"
        "   padding: 0px 5px;"
        "}"
    );

    QGridLayout *gridLayout = new QGridLayout(passengerGroup);
    gridLayout->setSpacing(15);

    QMap<QString, QLineEdit*> fields;

    // Title
    QLabel *titleLabel = new QLabel("Title:");
    titleLabel->setStyleSheet("QLabel { font-weight: 600; }");
    QComboBox *titleCombo = new QComboBox();
    titleCombo->addItems({"Mr", "Mrs", "Miss", "Ms"});
    titleCombo->setStyleSheet(
        "QComboBox { border: 1px solid #E5E7EB; border-radius: 4px; padding: 8px; }"
    );
    gridLayout->addWidget(titleLabel, 0, 0);
    gridLayout->addWidget(titleCombo, 0, 1);

    // First Name
    QLabel *fnameLabel = new QLabel("First Name:");
    fnameLabel->setStyleSheet("QLabel { font-weight: 600; }");
    QLineEdit *fnameInput = new QLineEdit();
    fnameInput->setPlaceholderText("John");
    fnameInput->setStyleSheet(
        "QLineEdit { border: 1px solid #E5E7EB; border-radius: 4px; padding: 8px; }"
    );
    fields["firstName"] = fnameInput;
    gridLayout->addWidget(fnameLabel, 0, 2);
    gridLayout->addWidget(fnameInput, 0, 3);

    // Last Name
    QLabel *lnameLabel = new QLabel("Last Name:");
    lnameLabel->setStyleSheet("QLabel { font-weight: 600; }");
    QLineEdit *lnameInput = new QLineEdit();
    lnameInput->setPlaceholderText("Doe");
    lnameInput->setStyleSheet(
        "QLineEdit { border: 1px solid #E5E7EB; border-radius: 4px; padding: 8px; }"
    );
    fields["lastName"] = lnameInput;
    gridLayout->addWidget(lnameLabel, 1, 0);
    gridLayout->addWidget(lnameInput, 1, 1);

    // Email
    QLabel *emailLabel = new QLabel("Email:");
    emailLabel->setStyleSheet("QLabel { font-weight: 600; }");
    QLineEdit *emailInput = new QLineEdit();
    emailInput->setPlaceholderText("john@example.com");
    emailInput->setStyleSheet(
        "QLineEdit { border: 1px solid #E5E7EB; border-radius: 4px; padding: 8px; }"
    );
    fields["email"] = emailInput;
    gridLayout->addWidget(emailLabel, 1, 2);
    gridLayout->addWidget(emailInput, 1, 3);

    // Date of Birth
    QLabel *dobLabel = new QLabel("Date of Birth:");
    dobLabel->setStyleSheet("QLabel { font-weight: 600; }");
    QDateEdit *dobInput = new QDateEdit();
    dobInput->setDate(QDate(1990, 1, 1));
    dobInput->setStyleSheet(
        "QDateEdit { border: 1px solid #E5E7EB; border-radius: 4px; padding: 8px; }"
    );
    gridLayout->addWidget(dobLabel, 2, 0);
    gridLayout->addWidget(dobInput, 2, 1);

    // Passport
    QLabel *passportLabel = new QLabel("Passport Number:");
    passportLabel->setStyleSheet("QLabel { font-weight: 600; }");
    QLineEdit *passportInput = new QLineEdit();
    passportInput->setPlaceholderText("AB123456");
    passportInput->setStyleSheet(
        "QLineEdit { border: 1px solid #E5E7EB; border-radius: 4px; padding: 8px; }"
    );
    fields["passport"] = passportInput;
    gridLayout->addWidget(passportLabel, 2, 2);
    gridLayout->addWidget(passportInput, 2, 3);

    passengerFields.append(fields);
    scrollLayout->addWidget(passengerGroup);
    scrollLayout->addStretch();

    scroll->setWidget(scrollContent);
    layout->addWidget(scroll, 1);

    stepsStack->addWidget(step1);
}

void ModernBookingWizard::createStep2SeatSelection() {
    QWidget *step2 = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(step2);
    layout->setContentsMargins(30, 30, 30, 30);

    QLabel *title = new QLabel("Select Your Seats");
    title->setStyleSheet("QLabel { color: #111827; font-size: 20px; font-weight: 700; }");
    layout->addWidget(title);

    // 座位图示
    QGroupBox *seatMap = new QGroupBox("Cabin Layout");
    seatMap->setStyleSheet(
        "QGroupBox {"
        "   color: #374151;"
        "   border: 1px solid #E5E7EB;"
        "   border-radius: 6px;"
        "   padding: 20px;"
        "}"
    );

    QGridLayout *seatLayout = new QGridLayout(seatMap);
    seatLayout->setSpacing(5);

    // 创建座位按钮网格
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 6; ++col) {
            QPushButton *seatBtn = new QPushButton();
            seatBtn->setFixedSize(50, 50);
            seatBtn->setText(QString::number(row + 1) + (char)('A' + col));
            seatBtn->setCheckable(true);
            seatBtn->setStyleSheet(
                "QPushButton {"
                "   background-color: #E5E7EB;"
                "   color: #374151;"
                "   border: none;"
                "   border-radius: 4px;"
                "   font-weight: 600;"
                "   font-size: 11px;"
                "}"
                "QPushButton:hover {"
                "   background-color: #D1D5DB;"
                "}"
                "QPushButton:checked {"
                "   background-color: #0052CC;"
                "   color: white;"
                "}"
            );
            seatLayout->addWidget(seatBtn, row, col);
        }
    }

    layout->addWidget(seatMap, 1);

    stepsStack->addWidget(step2);
}

void ModernBookingWizard::createStep3Review() {
    QWidget *step3 = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(step3);
    layout->setContentsMargins(30, 30, 30, 30);

    QLabel *title = new QLabel("Review Your Booking");
    title->setStyleSheet("QLabel { color: #111827; font-size: 20px; font-weight: 700; }");
    layout->addWidget(title);

    // 航班信息卡片
    QFrame *flightCard = new QFrame();
    flightCard->setStyleSheet(
        "QFrame {"
        "   background-color: white;"
        "   border: 1px solid #E5E7EB;"
        "   border-radius: 12px;"
        "   padding: 20px;"
        "}"
    );

    QVBoxLayout *cardLayout = new QVBoxLayout(flightCard);

    QLabel *airline = new QLabel("Sky Airways - SA123");
    airline->setStyleSheet("QLabel { color: #0052CC; font-size: 16px; font-weight: 700; }");
    cardLayout->addWidget(airline);

    QLabel *route = new QLabel("New York (JFK) → Los Angeles (LAX)");
    route->setStyleSheet("QLabel { color: #111827; font-size: 15px; font-weight: 600; }");
    cardLayout->addWidget(route);

    QLabel *time = new QLabel("08:00 AM - 11:30 AM (5h 30m) • Feb 15, 2024");
    time->setStyleSheet("QLabel { color: #6B7280; font-size: 13px; }");
    cardLayout->addWidget(time);

    layout->addWidget(flightCard);

    // 价格摘要
    QFrame *priceCard = new QFrame();
    priceCard->setStyleSheet(
        "QFrame {"
        "   background-color: white;"
        "   border: 1px solid #E5E7EB;"
        "   border-radius: 12px;"
        "   padding: 20px;"
        "}"
    );

    QVBoxLayout *priceLayout = new QVBoxLayout(priceCard);

    QLabel *priceTitle = new QLabel("Price Summary");
    priceTitle->setStyleSheet("QLabel { color: #111827; font-size: 16px; font-weight: 700; }");
    priceLayout->addWidget(priceTitle);

    QFrame *divider = new QFrame();
    divider->setStyleSheet("QFrame { background-color: #E5E7EB; }");
    divider->setFixedHeight(1);
    priceLayout->addWidget(divider);

    QHBoxLayout *itemLayout = new QHBoxLayout();
    QLabel *baseFareLabel = new QLabel("Base Fare:");
    QLabel *baseFareValue = new QLabel("$199.99");
    baseFareValue->setStyleSheet("QLabel { color: #111827; font-weight: 600; }");
    itemLayout->addWidget(baseFareLabel);
    itemLayout->addStretch();
    itemLayout->addWidget(baseFareValue);
    priceLayout->addLayout(itemLayout);

    itemLayout = new QHBoxLayout();
    QLabel *taxesLabel = new QLabel("Taxes & Fees:");
    QLabel *taxesValue = new QLabel("$50.00");
    taxesValue->setStyleSheet("QLabel { color: #111827; font-weight: 600; }");
    itemLayout->addWidget(taxesLabel);
    itemLayout->addStretch();
    itemLayout->addWidget(taxesValue);
    priceLayout->addLayout(itemLayout);

    divider = new QFrame();
    divider->setStyleSheet("QFrame { background-color: #E5E7EB; }");
    divider->setFixedHeight(1);
    priceLayout->addWidget(divider);

    itemLayout = new QHBoxLayout();
    QLabel *totalLabel = new QLabel("Total Price:");
    totalLabel->setStyleSheet("QLabel { font-weight: 700; }");
    QLabel *totalValue = new QLabel("$249.99");
    totalValue->setStyleSheet("QLabel { color: #10B981; font-weight: 700; font-size: 16px; }");
    itemLayout->addWidget(totalLabel);
    itemLayout->addStretch();
    itemLayout->addWidget(totalValue);
    priceLayout->addLayout(itemLayout);

    layout->addWidget(priceCard);
    layout->addStretch();

    stepsStack->addWidget(step3);
}

void ModernBookingWizard::createStep4Payment() {
    QWidget *step4 = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(step4);
    layout->setContentsMargins(30, 30, 30, 30);

    QLabel *title = new QLabel("Payment Information");
    title->setStyleSheet("QLabel { color: #111827; font-size: 20px; font-weight: 700; }");
    layout->addWidget(title);

    QGroupBox *paymentGroup = new QGroupBox("Payment Method");
    paymentGroup->setStyleSheet(
        "QGroupBox {"
        "   color: #374151;"
        "   border: 1px solid #E5E7EB;"
        "   border-radius: 6px;"
        "   padding: 20px;"
        "}"
    );

    QVBoxLayout *paymentLayout = new QVBoxLayout(paymentGroup);

    // 支付方式选择
    QCheckBox *creditCard = new QCheckBox("Credit Card");
    creditCard->setChecked(true);
    paymentLayout->addWidget(creditCard);

    QCheckBox *debitCard = new QCheckBox("Debit Card");
    paymentLayout->addWidget(debitCard);

    QCheckBox *paypal = new QCheckBox("PayPal");
    paymentLayout->addWidget(paypal);

    paymentLayout->addSpacing(15);

    // 信用卡信息
    QGridLayout *cardLayout = new QGridLayout();

    QLabel *holderLabel = new QLabel("Cardholder Name:");
    QLineEdit *holderInput = new QLineEdit();
    holderInput->setStyleSheet("QLineEdit { border: 1px solid #E5E7EB; border-radius: 4px; padding: 8px; }");
    cardLayout->addWidget(holderLabel, 0, 0);
    cardLayout->addWidget(holderInput, 0, 1);

    QLabel *cardNumLabel = new QLabel("Card Number:");
    QLineEdit *cardNumInput = new QLineEdit();
    cardNumInput->setPlaceholderText("1234 5678 9012 3456");
    cardNumInput->setStyleSheet("QLineEdit { border: 1px solid #E5E7EB; border-radius: 4px; padding: 8px; }");
    cardLayout->addWidget(cardNumLabel, 1, 0, 1, 2);
    cardLayout->addWidget(cardNumInput, 1, 2, 1, 2);

    QLabel *expLabel = new QLabel("Expiry Date:");
    QLineEdit *expInput = new QLineEdit();
    expInput->setPlaceholderText("MM/YY");
    expInput->setStyleSheet("QLineEdit { border: 1px solid #E5E7EB; border-radius: 4px; padding: 8px; }");
    cardLayout->addWidget(expLabel, 2, 0);
    cardLayout->addWidget(expInput, 2, 1);

    QLabel *cvvLabel = new QLabel("CVV:");
    QLineEdit *cvvInput = new QLineEdit();
    cvvInput->setPlaceholderText("123");
    cvvInput->setStyleSheet("QLineEdit { border: 1px solid #E5E7EB; border-radius: 4px; padding: 8px; }");
    cardLayout->addWidget(cvvLabel, 2, 2);
    cardLayout->addWidget(cvvInput, 2, 3);

    paymentLayout->addLayout(cardLayout);

    layout->addWidget(paymentGroup);

    // 条款同意
    QCheckBox *termsCheckbox = new QCheckBox("I agree to the terms and conditions");
    termsCheckbox->setStyleSheet("QCheckBox { color: #374151; }");
    layout->addWidget(termsCheckbox);

    layout->addStretch();

    stepsStack->addWidget(step4);
}

void ModernBookingWizard::onNextClicked() {
    m_currentStep++;
    if (m_currentStep >= 4) {
        nextBtn->setVisible(false);
        confirmBtn->setVisible(true);
    }
    backBtn->setEnabled(true);
    stepsStack->setCurrentIndex(m_currentStep - 1);
    updateProgressBar(m_currentStep);
}

void ModernBookingWizard::onBackClicked() {
    m_currentStep--;
    if (m_currentStep <= 1) {
        backBtn->setEnabled(false);
    }
    if (m_currentStep < 4) {
        nextBtn->setVisible(true);
        confirmBtn->setVisible(false);
    }
    stepsStack->setCurrentIndex(m_currentStep - 1);
    updateProgressBar(m_currentStep);
}

void ModernBookingWizard::onConfirmClicked() {
    qDebug() << "Booking confirmed!";
    // 发送预订请求
}

void ModernBookingWizard::updateProgressBar(int currentStep) {
    qDebug() << "Progress updated to step:" << currentStep;
}
