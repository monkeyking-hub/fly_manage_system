#ifndef INTERFACEMANAGER_H
#define INTERFACEMANAGER_H

#include <QWidget>
#include <QStackedWidget>
#include <QMap>

\
//界面调度类
class InterfaceManager : private QObject
{
    Q_OBJECT

public:
    static InterfaceManager* instance();  // 获取单例实例

    // 注册页面
    void registerPage(const QString &fullPath, QWidget *page);
    // 解除注册
    void unregisterPage(const QString &fullPath);

    // 切换到指定页面
    void switchToPage(const QString &fullPath);

    QStackedWidget *m_stackedWidget;     // 堆叠控件
private:
    explicit InterfaceManager(QObject *parent = nullptr);
    ~InterfaceManager();

    // 禁止拷贝和赋值
    InterfaceManager(const InterfaceManager&) = delete;
    InterfaceManager& operator=(const InterfaceManager&) = delete;

private:
    static InterfaceManager* m_instance;  // 单例实例

    QMap<QString, QWidget*> m_pages;     // 存储注册的页面
    QMap<QString, int> m_pageIndexes;    // 存储页面全路径与索引的映射
    QString m_currentPageFullPath;       // 当前页面的全路径
};

#endif // INTERFACEMANAGER_H
