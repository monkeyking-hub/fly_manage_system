#ifndef MODERNMAINWINDOW_H
#define MODERNMAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

class ModernMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ModernMainWindow(QWidget *parent = nullptr);
    ~ModernMainWindow();

private slots:
    void switchToPage(const QString &pageName);
    void onThemeChanged();

private:
    void setupUI();
    void connectSignals();
    void applyTheme();
    void setupCentralWidget();

    QStackedWidget *stackedWidget;
    QWidget *centralWidget;
};

#endif // MODERNMAINWINDOW_H