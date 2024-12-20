#ifndef ADMINHOMEWINDOW_H
#define ADMINHOMEWINDOW_H

#include <QMainWindow>

namespace Ui {
class adminHomeWindow;
}

class adminHomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit adminHomeWindow(QWidget *parent = nullptr);
    ~adminHomeWindow();

private:
    Ui::adminHomeWindow *ui;
};

#endif // ADMINHOMEWINDOW_H
