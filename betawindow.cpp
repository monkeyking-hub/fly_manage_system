#include "betawindow.h"
#include "InterfaceManager.h"
#include "ui_betawindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

BetaWindow::BetaWindow(const QString &parentName, const QString &name, QWidget *parent)
    : QMainWindow(parent), m_name(name)
    , m_parentName(parentName)
    , ui(new Ui::BetaWindow)
{
     ui->setupUi(this);

    // 构造全路径并注册到界面调度类中
    QString fullPath = m_parentName + "/" + m_name;
    InterfaceManager::instance()->registerPage(fullPath, this);


}

BetaWindow::~BetaWindow()
{
    delete ui;
}

