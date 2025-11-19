#ifndef MODERNMAINWINDOW_H
#define MODERNMAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

class ModernMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ModernMainWindow(QWidget *parent = nullptr);
    ~ModernMainWindow();

    QStackedWidget* getStackedWidget();

private:
    void setupUI();
    QStackedWidget *stackedWidget;
};

#endif // MODERNMAINWINDOW_H
