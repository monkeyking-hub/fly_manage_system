#ifndef FLIGHTSTATUS_H
#define FLIGHTSTATUS_H

#include <QCompleter>
#include <QLineEdit>
#include <QListView>
#include <QMainWindow>
#include <QStringListModel>
#include <QPushButton>

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

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void on_departureInput_textChanged(const QString &text);

    void on_destinationInput_textChanged(const QString &text);

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_btn_popuplar_clicked();

    void on_btn_domestic_clicked();

    void on_btn_abroad_clicked();

    void showPickWidget(QLineEdit* qle);

    void cityPicked(const QString city);

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_btn_popuplar_2_clicked();

    void on_btn_domestic_2_clicked();

    void on_btn_abroad_2_clicked();

private:
    void init();
    Ui::flightstatus *ui;
    QStringList cityList;             // 存储所有城市数据
    QStringListModel *cityListModel;  // 用于设置给Completer
    QCompleter *departureCompleter;   // 出发地补全器
    QCompleter *destinationCompleter; // 目的地补全器
    QList<QPushButton*> cityButtons;
    void setupCompleter(); // 设置补全器
    QLineEdit *currentLineEdit;
    void dpcityPicked(const QString city);
    void dscityPicked(const QString city);
    QList<QPushButton*> checkableButtons;
    QList<QPushButton*> checkableButtons_2;
    void setActiveSection(const QString& section);
    void setActiveSection_2(const QString& section);
};

#endif // FLIGHTSTATUS_H
