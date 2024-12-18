#ifndef ADMINDELETEFLIGHTWINDOW_H
#define ADMINDELETEFLIGHTWINDOW_H

#include <QMainWindow>

namespace Ui {
class adminDeleteFlightWindow;
}

class adminDeleteFlightWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit adminDeleteFlightWindow(QWidget *parent = nullptr);
    ~adminDeleteFlightWindow();

private:
    Ui::adminDeleteFlightWindow *ui;
};

#endif // ADMINDELETEFLIGHTWINDOW_H
