#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <QMainWindow>

namespace Ui {
class maininterface;
}

class maininterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit maininterface(QWidget *parent = nullptr);
    ~maininterface();

private:
    Ui::maininterface *ui;
};

#endif // MAININTERFACE_H
