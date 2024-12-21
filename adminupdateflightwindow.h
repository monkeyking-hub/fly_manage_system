#ifndef ADMINUPDATEFLIGHTWINDOW_H
#define ADMINUPDATEFLIGHTWINDOW_H

#include <QMainWindow>
#include <QPushButton>

namespace Ui {
class adminUpdateFlightWindow;
}

class adminUpdateFlightWindow : public QMainWindow
{
    Q_OBJECT

public:
    QPushButton *selectedBtn;
    int id;

public:
    explicit adminUpdateFlightWindow(QWidget *parent = nullptr);
    ~adminUpdateFlightWindow();

public slots:
    void showCalendar(QPushButton *button);
    void selectDate(const QDate &date);
    void onSearchButtonClicked();
    void onCancelButtonClicked();
    void onSaveButtonClicked();

private:
    Ui::adminUpdateFlightWindow *ui;
};

#endif // ADMINUPDATEFLIGHTWINDOW_H
