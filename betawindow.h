#ifndef BETAWINDOW_H
#define BETAWINDOW_H

#include <QMainWindow>

namespace Ui {
class betawindow;
}

class betawindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit betawindow(QWidget *parent = nullptr);
    ~betawindow();

private:
    Ui::betawindow *ui;
};

#endif // BETAWINDOW_H
