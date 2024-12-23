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

private slots:
    void on_pushButton_clicked();

private:
    Ui::pay_window *ui;
    const QPixmap getPic(const QString &name);
};

#endif // PAY_WINDOW_H
