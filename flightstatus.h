#ifndef FLIGHTSTATUS_H
#define FLIGHTSTATUS_H

#include <QCompleter>
#include <QLineEdit>
#include <QListView>
#include <QMainWindow>
#include <QStringListModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class flightstatus;
}
QT_END_NAMESPACE

// 主窗口类
class flightstatus : public QMainWindow
{
    Q_OBJECT

public:
    explicit flightstatus(QWidget *parent = nullptr);
    ~flightstatus();

private slots:
    void on_departureInput_textChanged(const QString &text);
    void on_destinationInput_textChanged(const QString &text);

private:
    Ui::flightstatus *ui;
    QStringList cityList;             // 存储所有城市数据
    QStringListModel *cityListModel;  // 用于设置给Completer
    QCompleter *departureCompleter;   // 出发地补全器
    QCompleter *destinationCompleter; // 目的地补全器

    void setupCompleter(); // 设置补全器
};

#endif // FLIGHTSTATUS_H
