#ifndef MODERNLOGINWINDOW_H
#define MODERNLOGINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

class ModernLoginWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ModernLoginWindow(QWidget *parent = nullptr);
    ~ModernLoginWindow();

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onForgotPasswordClicked();
    void onLoginSuccess(const QJsonObject &userData);
    void onLoginFailed(const QString &error);
    void onRememberMeToggled(bool checked);
    void onInputChanged();

private:
    void setupUI();
    void connectSignals();
    void applyTheme();
    void sendLoginRequest(const QString &email, const QString &password);
    void showErrorMessage(const QString &message);
    void showSuccessMessage(const QString &message);
    void animateButton(QPushButton *button);
    void setupAnimations();

    // UI Components
    QWidget *centralWidget;
    QFrame *leftPanel;
    QFrame *rightPanel;
    
    // Left Panel Components
    QLabel *brandLabel;
    QLabel *brandIcon;
    QLabel *brandDescription;
    QLabel *featuresLabel;
    
    // Right Panel Components
    QLabel *welcomeLabel;
    QLabel *subtitleLabel;
    QLineEdit *emailInput;
    QLineEdit *passwordInput;
    QCheckBox *rememberMeCheckBox;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QPushButton *forgotPasswordButton;
    QLabel *errorLabel;
    QLabel *successLabel;
    
    // Navigation
    QLabel *dividerLabel;
    
    // Effects and Animations
    QGraphicsOpacityEffect *opacityEffect;
    QPropertyAnimation *fadeInAnimation;

    QNetworkAccessManager *networkManager;
};

#endif // MODERNLOGINWINDOW_H