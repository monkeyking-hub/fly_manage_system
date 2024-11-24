#ifndef INORDER_H
#define INORDER_H

#include <QMainWindow>

namespace Ui {
class inorder;
}

class inorder : public QMainWindow
{
    Q_OBJECT

public:
    explicit inorder(QWidget *parent = nullptr);
    ~inorder();

private:
    Ui::inorder *ui;
};

#endif // INORDER_H
