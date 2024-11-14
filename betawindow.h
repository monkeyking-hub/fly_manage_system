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
    explicit BetaWindow(const QString &parentName, const QString &name, QWidget *parent = nullptr);
    ~BetaWindow();

private slots:

private:
    QString m_name;           // 当前界面名
    QString m_parentName;     // 父界面名
    Ui::BetaWindow *ui;
};

#endif // betawindow_H
