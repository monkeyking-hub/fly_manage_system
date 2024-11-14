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

    void on_toGamabutton_clicked();

    void on_toXitabutton_clicked();

    void on_toYipuxilongbutton_clicked();

private:
    Ui::BetaWindow *ui;
};

#endif // betawindow_H
