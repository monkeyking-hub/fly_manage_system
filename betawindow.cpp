#include "betawindow.h"
#include "InterfaceManager.h"
#include "ui_betawindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

BetaWindow::BetaWindow( QWidget *parent)
    : QMainWindow(parent)
    , m_parentName(parentName)
    , ui(new Ui::BetaWindow)
{
     ui->setupUi(this);
}

BetaWindow::~BetaWindow()
{
    delete ui;
}

