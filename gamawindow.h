#ifndef GAMAWINDOW_H
#define GAMAWINDOW_H

#include <QMainWindow>

namespace Ui {
class GamaWindow;
}

class GamaWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GamaWindow(QWidget *parent = nullptr);
    ~GamaWindow();

private slots:
    void on_toBetaButton_clicked();

private:
    Ui::GamaWindow *ui;
};

#endif // GAMAWINDOW_H
