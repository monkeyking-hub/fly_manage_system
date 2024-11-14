#include "InterfaceManager.h"
#include <QStackedWidget>
#include <QDebug>

InterfaceManager* InterfaceManager::m_instance = nullptr;

InterfaceManager::InterfaceManager(QObject *parent)
    : QObject(parent), m_stackedWidget(new QStackedWidget())
{
}

InterfaceManager::~InterfaceManager()
{
    delete m_stackedWidget;
}

InterfaceManager* InterfaceManager::instance()
{
    if (m_instance == nullptr) {
        m_instance = new InterfaceManager();
    }
    return m_instance;
}

void InterfaceManager::registerPage(const QString &fullPath, QWidget *page)
{
    if (m_pages.contains(fullPath)) {
        qWarning() << "Page already registered:" << fullPath;
        return;
    }

    // 添加页面到 QStackedWidget 和映射
    m_pages[fullPath] = page;
    m_stackedWidget->addWidget(page);

    // 记录页面的索引
    m_pageIndexes[fullPath] = m_stackedWidget->count() - 1;
}
void InterfaceManager::unregisterPage(const QString &fullPath)
{
    // 检查页面是否已经注册
    if (m_pages.contains(fullPath)) {
        // 获取页面的索引
        int pageIndex = m_pageIndexes[fullPath];

        // 从 QStackedWidget 中移除页面
        QWidget* page = m_pages[fullPath];
        m_stackedWidget->removeWidget(page);

        // 删除页面的 QWidget 实例
        delete page;

        // 从 m_pages 和 m_pageIndexes 中移除该页面
        m_pages.remove(fullPath);
        m_pageIndexes.remove(fullPath);

        qDebug() << "页面已取消注册：" << fullPath;
    } else {
        qWarning() << "页面未注册，无法取消注册：" << fullPath;
    }
}

void InterfaceManager::switchToPage(const QString &fullPath)
{
    if (m_pages.contains(fullPath)) {
        qDebug() << "准备切换到页面：" << fullPath;

        // 获取页面的索引并切换
        int pageIndex = m_pageIndexes[fullPath];
        m_stackedWidget->setCurrentIndex(pageIndex);
        m_currentPageFullPath = fullPath;
    } else {
        qWarning() << "页面未找到：" << fullPath;
    }
}

