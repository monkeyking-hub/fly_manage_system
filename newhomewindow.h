#ifndef NEWHOMEWINDOW_H
#define NEWHOMEWINDOW_H

#include <QMainWindow>

namespace Ui {
class newHomeWindow;
}

class newHomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit newHomeWindow(QWidget *parent = nullptr);
    ~newHomeWindow();

private:
    Ui::newHomeWindow *ui;
};

#endif // NEWHOMEWINDOW_H
