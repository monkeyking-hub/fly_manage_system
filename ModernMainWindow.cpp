#include "ModernMainWindow.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QScreen>

ModernMainWindow::ModernMainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUI();
}

ModernMainWindow::~ModernMainWindow() {
}

void ModernMainWindow::setupUI() {
    setWindowTitle("Sky Wings - Modern Airline Booking System");
    setWindowIcon(QIcon());
    
    resize(1400, 900);
    QRect screen = QApplication::primaryScreen()->geometry();
    move((screen.width() - width()) / 2, (screen.height() - height()) / 2);

    // 创建QStackedWidget作为中央widget
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setStyleSheet("QStackedWidget { background-color: #F9FAFB; }");
    setCentralWidget(stackedWidget);

    setStyleSheet(
        "QMainWindow {"
        "   background-color: #F9FAFB;"
        "}"
    );
}

QStackedWidget* ModernMainWindow::getStackedWidget() {
    return stackedWidget;
}
