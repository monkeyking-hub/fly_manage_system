#ifndef adminMainInterface_H
#define adminMainInterface_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>

namespace Ui {
class adminMainInterface;
}

class adminMainInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit adminMainInterface(QWidget *parent = nullptr);
    ~adminMainInterface();

private slots:
    void addUser();
    void editUser();
    void deleteUser();
    void searchUser();

    void addOrder();
    void editOrder();
    void deleteOrder();
    void searchOrder();

    void addFlight();
    void editFlight();
    void deleteFlight();
    void searchFlight();

    void processRefund();

private:
    void setupUI();
    void setupUserTab();
    void setupOrderTab();
    void setupFlightTab();
    void setupRefundTab();

    QTabWidget *tabWidget;

    // User Management Tab
    QTableWidget *userTable;
    QLineEdit *userSearchBox;

    // Order Management Tab
    QTableWidget *orderTable;
    QLineEdit *orderSearchBox;

    // Flight Management Tab
    QTableWidget *flightTable;
    QLineEdit *flightSearchBox;

    // Refund Management Tab
    QTableWidget *refundTable;
    QLineEdit *refundSearchBox;

private:
    Ui::adminMainInterface *ui;
};

#endif // adminMainInterface_H
