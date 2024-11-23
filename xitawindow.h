#ifndef XITAWINDOW_H
#define XITAWINDOW_H

#include <QMainWindow>

namespace Ui {
class XitaWindow;
}

class XitaWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit XitaWindow(QWidget *parent = nullptr);
    ~XitaWindow();
private slots:
    void on_toBetaButton_clicked();

private:
    Ui::XitaWindow *ui;
};

#endif // XITAWINDOW_H
