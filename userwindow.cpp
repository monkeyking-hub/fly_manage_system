#include "userwindow.h"
#include <QFileDialog>
#include <QImage>
#include <QMessageBox>
#include <QPixmap>
#include "interfacemanager.h"
#include "ui_userwindow.h"
Userwindow::Userwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Userwindow)
{
    ui->setupUi(this);
}

Userwindow::~Userwindow()
{
    delete ui;
}

void Userwindow::on_pushButton_clicked()
{
    ui->userNameStackedWidget->setCurrentIndex(1);
    ui->mailStackedWidget->setCurrentIndex(1);
    ui->phoneNumberStackedWidget->setCurrentIndex(1);
    ui->sexStackedWidget->setCurrentIndex(1);
    ui->oiStackedWidget->setCurrentIndex(1);
    ui->ProfileStackedWidget->setCurrentIndex(1);
    ui->ageStackedWidget->setCurrentIndex(1);
}

void Userwindow::on_saveButton_clicked()
{
    ui->useNameLabel->setText(ui->userNameSetBox->text());
    ui->mailLabel_2->setText(ui->mailSetBox->text());
    ui->phoneNumberLabel->setText(ui->phoneNumberSetBox->text());
    ui->agelabel->setText(ui->ageSetBox->text());
    if (ui->maleButton->isChecked())
        ui->sexLabel->setText(ui->maleButton->text());
    else if (ui->famelButton->isChecked())
        ui->sexLabel->setText(ui->famelButton->text());
    else
        ui->sexLabel->setText("unselected");
    ui->userNameStackedWidget->setCurrentIndex(0);
    ui->mailStackedWidget->setCurrentIndex(0);
    ui->phoneNumberStackedWidget->setCurrentIndex(0);
    ui->sexStackedWidget->setCurrentIndex(0);
    ui->oiStackedWidget->setCurrentIndex(0);
    ui->ProfileStackedWidget->setCurrentIndex(0);
    ui->ageStackedWidget->setCurrentIndex(0);
}

void Userwindow::on_uploadProfileButton_clicked()
{
    QString filePath
        = QFileDialog::getOpenFileName(this,
                                       "选择头像",
                                       "",
                                       "Images (*.png *.jpg *.jpeg *.bmp);;All Files (*)");
    if (filePath.isEmpty())
        return;

    // 加载图片
    QPixmap avatar(filePath);
    if (avatar.isNull()) {
        QMessageBox::warning(this, "错误", "无法加载图片，请选择有效的图片文件！");
        return;
    }

    // 将图片显示到 QLabel 并调整大小
    ui->profileLbl->setPixmap(
        avatar.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Userwindow::on_logOutButton_clicked()
{
    InterfaceManager::instance()->switchToPage("/MainWindow/Beta");
}

void Userwindow::on_returnButton_clicked()
{
    InterfaceManager::instance()->switchToPage("lxt_mainInterface");
}
