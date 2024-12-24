#ifndef PAY_WINDOW_H
#define PAY_WINDOW_H

#include <QWidget>

namespace Ui {
class pay_window;
}

class pay_window : public QWidget
{
    Q_OBJECT

public:
    explicit pay_window(QWidget *parent = nullptr);
    ~pay_window();
    void loadFliInfo();
    int price;
    Ui::pay_window *ui;
signals:
    void itemCountChanged();
    void countChanged();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_btn_1_clicked();

    void on_btn_2_clicked();

    void on_btn_3_clicked();

    void on_btn_f1_clicked();

    void on_btn_f2_clicked();

    void on_btn_f3_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_3_clicked();

private:
    const QPixmap getPic(const QString &name);
};

#endif // PAY_WINDOW_H
