#ifndef betawindow_H
#define betawindow_H

#include <QWidget>
#include <QMainWindow>
#include "InterfaceManager.h"
namespace Ui {
class BetaWindow;
}


class BetaWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BetaWindow(QWidget *parent = nullptr);
    ~BetaWindow();

private slots:

private:
    Ui::BetaWindow *ui;
};

#endif // betawindow_H
