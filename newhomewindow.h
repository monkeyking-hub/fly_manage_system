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
    void onCommandLinkeButton3Clicked();
    ~newHomeWindow();

signals:
    void commandLinkButton4Clicked();
    void commandLinkButton5Clicked();
    void commandLinkButton6Clicked();

private:
    Ui::newHomeWindow *ui;
};

#endif // NEWHOMEWINDOW_H
