#include <QComboBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class MReorderWindow : public QWidget
{
    Q_OBJECT

public:
    MReorderWindow(QWidget *parent = nullptr);
    ~MReorderWindow();

private slots:
    void onSubmitClicked();
    void onReplyFinished(QNetworkReply *reply);

private:
    QLineEdit *orderIdEdit;
    QComboBox *statusCombo;
    QLineEdit *paymentTimeEdit;
    QPushButton *submitButton;
    QNetworkAccessManager *networkManager;
};

MReorderWindow::MReorderWindow(QWidget *parent)
    : QWidget(parent)
{
    orderIdEdit = new QLineEdit(this);
    orderIdEdit->setPlaceholderText("Enter Order ID");

    statusCombo = new QComboBox(this);
    statusCombo->addItem("Paid");
    statusCombo->addItem("Refunded");
    statusCombo->addItem("Pending");

    paymentTimeEdit = new QLineEdit(this);
    paymentTimeEdit->setPlaceholderText("Enter Payment Time (Unix Timestamp)");

    submitButton = new QPushButton("Submit", this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QLabel("Order ID:"));
    layout->addWidget(orderIdEdit);
    layout->addWidget(new QLabel("Status:"));
    layout->addWidget(statusCombo);
    layout->addWidget(new QLabel("Payment Time:"));
    layout->addWidget(paymentTimeEdit);
    layout->addWidget(submitButton);

    setLayout(layout);

    networkManager = new QNetworkAccessManager(this);

    connect(submitButton, &QPushButton::clicked, this, &MReorderWindow::onSubmitClicked);
}

MReorderWindow::~MReorderWindow() {}

void MReorderWindow::onSubmitClicked()
{
    int orderId = orderIdEdit->text().toInt();
    QString status = statusCombo->currentText();
    int paymentTime = paymentTimeEdit->text().toInt();

    if (orderId == 0 || paymentTime == 0) {
        QMessageBox::warning(this, "Input Error", "Please enter valid Order ID and Payment Time.");
        return;
    }

    QJsonObject json;
    json["orderId"] = orderId;
    json["status"] = status;
    json["paymentTime"] = paymentTime;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QNetworkRequest request(QUrl("http://your-api-url/api/orders/update"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    networkManager->post(request, data);
    connect(networkManager,
            &QNetworkAccessManager::finished,
            this,
            &MReorderWindow::onReplyFinished);
}

void MReorderWindow::onReplyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject jsonResponse = doc.object();

        if (jsonResponse["code"].toInt() == 200) {
            QMessageBox::information(this, "Success", "Order updated successfully.");
        } else {
            QMessageBox::warning(this, "Error", jsonResponse["message"].toString());
        }
    } else {
        QMessageBox::warning(this, "Network Error", "Failed to update order.");
    }

    reply->deleteLater();
}
