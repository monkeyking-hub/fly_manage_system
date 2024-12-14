#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QApplication>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QEvent>
#include <QVector>
#include <QStyle>

namespace Ui {
class homeWindow;
}

class homeWindow : public QMainWindow
{
    Q_OBJECT
public:
    QHBoxLayout *scrollLayout;

public:
    explicit homeWindow(QWidget *parent = nullptr);
    void clearCategoryBlocks();
    void onSearchButtonClicked();
    ~homeWindow();

private:
    Ui::homeWindow *ui;
};

class CategoryBlock : public QFrame
{
    Q_OBJECT

public:
    explicit CategoryBlock(const QString &title,
                           const QStringList &routes,
                           const QStringList &prices,
                           QWidget *parent = nullptr)
        : QFrame(parent)
    {
        setFixedSize(300, 300);
        setObjectName("categoryBlock");
        setStyleSheet(R"(
            QFrame#categoryBlock {
                background-color: transparent;
                border-radius: 10px;
                padding: 10px;
                border: 2px solid transparent;
                transition: all 0.3s;
            }
            QFrame#categoryBlock:hover {
                background-color: #f0f5ff;
                border: 2px solid #78a0f7;
                box-shadow: 0px 4px 8px rgba(0, 0, 0, 0.2);
            }
        )");

        QVBoxLayout *layout = new QVBoxLayout(this);

        // 分类标题
        QLabel *titleLabel = new QLabel(this);
        titleLabel->setFixedSize(250,40);
        if(title=="周末省心游")
        {
            QPixmap pixmap(":/homewindow1.png");
            QPixmap scaledPixmap=pixmap.scaled(titleLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            titleLabel->setPixmap(scaledPixmap);
        }
        if(title=="爱上大草原")
        {
            QPixmap pixmap(":/homewindow2.png");
            QPixmap scaledPixmap=pixmap.scaled(titleLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            titleLabel->setPixmap(scaledPixmap);
        }
        if(title=="海边浪一浪")
        {
            QPixmap pixmap(":/homewindow3.png");
            QPixmap scaledPixmap=pixmap.scaled(titleLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            titleLabel->setPixmap(scaledPixmap);
        }
        layout->addWidget(titleLabel);

        QVector<QLabel *> labelsToHighlight;

        // 路线和价格子控件集合
        for (int i = 0; i < routes.size(); ++i) {
            QHBoxLayout *itemLayout = new QHBoxLayout();

            // 创建图片标签
            QLabel *imageLabel = new QLabel(this);
            QString imgPath=":/place"+QString::number(i+1)+".jpg";
            QPixmap imagePixmap(imgPath);
            imageLabel->setPixmap(imagePixmap.scaled(60, 50, Qt::KeepAspectRatio));
            imageLabel->setStyleSheet("border-radius: 5px; background-color: white;");

            // 路线和价格标签
            QLabel *routeLabel = new QLabel(routes[i], this);
            QLabel *priceLabel = new QLabel(prices[i], this);

            routeLabel->setStyleSheet(R"(
                QLabel {
                    font: 14px;
                    color: black;
                    background-color: white;
                    border-radius: 5px;
                }
                QLabel[hover="true"] {
                    background-color: #f0f5ff;
                }
            )");

            priceLabel->setStyleSheet(R"(
                QLabel {
                    font: bold 14px;
                    color: #ff5722;
                    background-color: white;
                    border-radius: 5px;
                }
                QLabel[hover="true"] {
                    background-color: #f0f5ff;
                }
            )");

            labelsToHighlight.append(imageLabel);
            labelsToHighlight.append(routeLabel);
            labelsToHighlight.append(priceLabel);

            itemLayout->addWidget(imageLabel);
            itemLayout->addWidget(routeLabel);
            itemLayout->addWidget(priceLabel, 1, Qt::AlignRight);
            layout->addLayout(itemLayout);
        }

        // 设置高亮标签
        setLabelsToHighlight(labelsToHighlight);
    }

    void setLabelsToHighlight(const QVector<QLabel *> &labels) {
        labelsToHighlight = labels;
    }

protected:
    void enterEvent(QEnterEvent *event) override {
        for (QLabel *label : labelsToHighlight) {
            if (label) {  // 确保指针有效
                label->setProperty("hover", true);
                label->style()->unpolish(label);
                label->style()->polish(label);
            }
        }
        QFrame::enterEvent(event);  // 调用基类方法
    }

    void leaveEvent(QEvent *event) override {
        for (QLabel *label : labelsToHighlight) {
            if (label) {  // 确保指针有效
                label->setProperty("hover", false);
                label->style()->unpolish(label);
                label->style()->polish(label);
            }
        }
        QFrame::leaveEvent(event);  // 调用基类方法
    }

private:
    QVector<QLabel *> labelsToHighlight;
};


#endif // HOMEWINDOW_H
