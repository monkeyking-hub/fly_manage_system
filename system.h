#ifndef SYSTEM_H
#define SYSTEM_H
#include <QObject>
#include "maininterface.h"
#include <orderwindow.h>

class System : public QObject
{
    Q_OBJECT;

public:
    System();
    ~System();
    void start();

private:
    maininterface *m_mainInterface; // 主界面指针
};

#endif // SYSTEM_H
