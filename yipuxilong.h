#ifndef YIPUXILONG_H
#define YIPUXILONG_H

#include <QMainWindow>

namespace Ui {
class Yipuxilong;
}

class Yipuxilong : public QMainWindow
{
    Q_OBJECT

public:
    explicit Yipuxilong(QWidget *parent = nullptr);
    ~Yipuxilong();

private slots:
    void on_toBetaButton_clicked();

    void on_toGameButton_clicked();

    void on_RequestButton_clicked();

private:
    Ui::Yipuxilong *ui;
};

#endif // YIPUXILONG_H
