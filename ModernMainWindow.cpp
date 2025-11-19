#include "ModernMainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QApplication>
#include <QScreen>

ModernMainWindow::ModernMainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUI();
    connectSignals();
    applyTheme();
}

ModernMainWindow::~ModernMainWindow() {
}

void ModernMainWindow::setupUI() {
    setWindowTitle("Sky Wings - 现代化航班预订系统");
    setMinimumSize(1200, 800);
    
    // 居中窗口
    QRect screen = QApplication::primaryScreen()->geometry();
    resize(1400, 900);
    move((screen.width() - width()) / 2, (screen.height() - height()) / 2);
    
    // 设置窗口样式
    setStyleSheet(
        "QMainWindow {"
        "   background-color: #F8FAFC;"
        "   color: #1E293B;"
        "   font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', 'Roboto', 'Helvetica', 'Arial', sans-serif;"
        "}"
    );
    
    setupCentralWidget();
}

void ModernMainWindow::setupCentralWidget() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // 创建堆叠窗口用于页面切换
    stackedWidget = new QStackedWidget(centralWidget);
    stackedWidget->setStyleSheet(
        "QStackedWidget {"
        "   background-color: #F8FAFC;"
        "   border: none;"
        "}"
    );
    
    mainLayout->addWidget(stackedWidget);
}

void ModernMainWindow::connectSignals() {
    // 连接页面切换信号
    connect(this, &ModernMainWindow::switchToPage, 
            this, &ModernMainWindow::switchToPage);
}

void ModernMainWindow::applyTheme() {
    // 应用现代化主题
    QFile styleFile(":/modern_theme.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        setStyleSheet(styleFile.readAll());
    }
}

void ModernMainWindow::switchToPage(const QString &pageName) {
    // 这里应该通过InterfaceManager来切换页面
    // 目前作为占位符实现
    qDebug() << "Switching to page:" << pageName;
}

void ModernMainWindow::onThemeChanged() {
    // 处理主题变化
    applyTheme();
}