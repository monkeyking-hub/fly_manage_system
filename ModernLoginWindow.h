#ifndef MODERNLOGINWINDOW_H
#define MODERNLOGINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class ModernLoginWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ModernLoginWindow(QWidget *parent = nullptr);
    ~ModernLoginWindow();

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onLoginSuccess(const QJsonObject &userData);
    void onLoginFailed(const QString &error);
    void onCheckBoxToggled(bool checked);

private:
    void setupUI();
    void connectSignals();
    void sendLoginRequest(const QString &email, const QString &password);
    void showErrorMessage(const QString &message);
    void showSuccessMessage(const QString &message);

    // UI Components
    QLineEdit *emailInput;
    QLineEdit *passwordInput;
    QCheckBox *rememberCheckBox;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QLabel *errorLabel;
    QLabel *brandLabel;
    QLabel *welcomeLabel;

    QNetworkAccessManager *networkManager;
};

#endif // MODERNLOGINWINDOW_H
