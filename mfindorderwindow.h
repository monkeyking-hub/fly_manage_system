#ifndef M_FIND_ORDER_WINDOW_H
#define M_FIND_ORDER_WINDOW_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QScrollArea>

// 前向声明，减少头文件依赖
class QWidget;

class MFindOrderWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MFindOrderWindow(QWidget *parent = nullptr);
    ~MFindOrderWindow();

private slots:
    // 槽函数：处理搜索按钮点击事件
    void onSearchButtonClicked();

private:
    // 成员变量：界面组件
    QLineEdit *userIdLineEdit;   // 用户ID输入框
    QPushButton *searchButton;  // 搜索按钮
    QWidget *resultWidget;      // 显示搜索结果的区域
    QVBoxLayout *resultLayout;  // 搜索结果布局

    // 成员函数：清空搜索结果
    void clearResults();
};

#endif // M_FIND_ORDER_WINDOW_H
