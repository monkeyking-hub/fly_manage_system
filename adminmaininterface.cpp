#include "adminmaininterface.h"
#include "ui_adminmaininterface.h"

adminMainInterface::adminMainInterface(QWidget *parent)
    : QMainWindow(parent)
    , tabWidget(new QTabWidget(this))
    , ui(new Ui::adminMainInterface)
{
    ui->setupUi(this);

    setWindowTitle("航班管理系统 - 管理员界面");
    resize(1000, 700);

    // 设置选项卡
    setupUserTab();
    setupOrderTab();
    setupFlightTab();
    setupRefundTab();

    setCentralWidget(tabWidget);
}

void adminMainInterface::setupUserTab()
{
    QWidget *userTab = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(userTab);

    userSearchBox = new QLineEdit;
    userSearchBox->setPlaceholderText("搜索用户...");
    QPushButton *searchButton = new QPushButton("搜索用户");
    connect(searchButton, &QPushButton::clicked, this, &adminMainInterface::searchUser);

    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchLayout->addWidget(userSearchBox);
    searchLayout->addWidget(searchButton);

    userTable = new QTableWidget;
    userTable->setColumnCount(4);
    userTable->setHorizontalHeaderLabels({"用户ID", "用户名", "邮箱", "手机号"});

    QPushButton *addButton = new QPushButton("添加用户");
    QPushButton *editButton = new QPushButton("编辑用户");
    QPushButton *deleteButton = new QPushButton("删除用户");

    connect(addButton, &QPushButton::clicked, this, &adminMainInterface::addUser);
    connect(editButton, &QPushButton::clicked, this, &adminMainInterface::editUser);
    connect(deleteButton, &QPushButton::clicked, this, &adminMainInterface::deleteUser);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);

    layout->addLayout(searchLayout);
    layout->addWidget(userTable);
    layout->addLayout(buttonLayout);

    tabWidget->addTab(userTab, "用户管理");
}

void adminMainInterface::setupOrderTab()
{
    QWidget *orderTab = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(orderTab);

    orderSearchBox = new QLineEdit;
    orderSearchBox->setPlaceholderText("搜索订单...");
    QPushButton *searchButton = new QPushButton("搜索订单");
    connect(searchButton, &QPushButton::clicked, this, &adminMainInterface::searchOrder);

    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchLayout->addWidget(orderSearchBox);
    searchLayout->addWidget(searchButton);

    orderTable = new QTableWidget;
    orderTable->setColumnCount(4);
    orderTable->setHorizontalHeaderLabels({"订单ID", "用户ID", "航班ID", "状态"});

    QPushButton *addButton = new QPushButton("添加订单");
    QPushButton *editButton = new QPushButton("编辑订单");
    QPushButton *deleteButton = new QPushButton("删除订单");

    connect(addButton, &QPushButton::clicked, this, &adminMainInterface::addOrder);
    connect(editButton, &QPushButton::clicked, this, &adminMainInterface::editOrder);
    connect(deleteButton, &QPushButton::clicked, this, &adminMainInterface::deleteOrder);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);

    layout->addLayout(searchLayout);
    layout->addWidget(orderTable);
    layout->addLayout(buttonLayout);

    tabWidget->addTab(orderTab, "订单管理");
}

void adminMainInterface::setupFlightTab()
{
    QWidget *flightTab = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(flightTab);

    flightSearchBox = new QLineEdit;
    flightSearchBox->setPlaceholderText("搜索航班...");
    QPushButton *searchButton = new QPushButton("搜索航班");
    connect(searchButton, &QPushButton::clicked, this, &adminMainInterface::searchFlight);

    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchLayout->addWidget(flightSearchBox);
    searchLayout->addWidget(searchButton);

    flightTable = new QTableWidget;
    flightTable->setColumnCount(5);
    flightTable->setHorizontalHeaderLabels({"航班ID", "起点", "终点", "出发时间", "座位数"});

    QPushButton *addButton = new QPushButton("添加航班");
    QPushButton *editButton = new QPushButton("编辑航班");
    QPushButton *deleteButton = new QPushButton("删除航班");

    connect(addButton, &QPushButton::clicked, this, &adminMainInterface::addFlight);
    connect(editButton, &QPushButton::clicked, this, &adminMainInterface::editFlight);
    connect(deleteButton, &QPushButton::clicked, this, &adminMainInterface::deleteFlight);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);

    layout->addLayout(searchLayout);
    layout->addWidget(flightTable);
    layout->addLayout(buttonLayout);

    tabWidget->addTab(flightTab, "航班管理");
}

void adminMainInterface::setupRefundTab()
{
    QWidget *refundTab = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(refundTab);

    refundSearchBox = new QLineEdit;
    refundSearchBox->setPlaceholderText("搜索退款请求...");
    QPushButton *searchButton = new QPushButton("搜索退款请求");
    connect(searchButton, &QPushButton::clicked, this, &adminMainInterface::processRefund);

    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchLayout->addWidget(refundSearchBox);
    searchLayout->addWidget(searchButton);

    refundTable = new QTableWidget;
    refundTable->setColumnCount(3);
    refundTable->setHorizontalHeaderLabels({"退款ID", "订单ID", "状态"});

    layout->addLayout(searchLayout);
    layout->addWidget(refundTable);

    tabWidget->addTab(refundTab, "退款管理");
}

// 示例槽函数实现
void adminMainInterface::addUser() {}
void adminMainInterface::editUser() {}
void adminMainInterface::deleteUser() {}
void adminMainInterface::searchUser() {}

void adminMainInterface::addOrder() {}
void adminMainInterface::editOrder() {}
void adminMainInterface::deleteOrder() {}
void adminMainInterface::searchOrder() {}

void adminMainInterface::addFlight() {}
void adminMainInterface::editFlight() {}
void adminMainInterface::deleteFlight() {}
void adminMainInterface::searchFlight() {}

void adminMainInterface::processRefund() {}

adminMainInterface::~adminMainInterface()
{
    delete ui;
}
