#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "betawindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QString m_name;           // 当前界面名
    QString m_parentName;     // 父界面名
    BetaWindow *page1;
    BetaWindow *page2;
};

#endif // MAINWINDOW_H
