#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>

namespace Ui {
class homeWindow;
}

class homeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit homeWindow(QWidget *parent = nullptr);
    QWidget *createCategoryBlock(const QString &title, const QStringList &routes, const QStringList &prices);
    ~homeWindow();

private:
    Ui::homeWindow *ui;
};

#endif // HOMEWINDOW_H
