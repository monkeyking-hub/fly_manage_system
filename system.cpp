#include "system.h"
#include "betawindow.h"
#include "xitawindow.h"
#include "yipuxilong.h"
#include "gamawindow.h"
System::System() {}
void System::start()
{
    // 创建子界面,并且注册进调度器
    BetaWindow * beta       = new BetaWindow();
    GamaWindow * gamawindow = new GamaWindow();
    XitaWindow * xitawindow = new XitaWindow();
    Yipuxilong * yipuxilong = new Yipuxilong();

    InterfaceManager::instance()->registerPage("/MainWindow/Beta", beta);
    //注册gama界面
    InterfaceManager::instance()->registerPage("/MainWindow/Beta/Gama",gamawindow);
    //注册西塔
    InterfaceManager::instance()->registerPage("/MainWindow/Beta/Xita",xitawindow);
    //注册伊普西隆
    InterfaceManager::instance()->registerPage("/MainWindow/Beta/Yipuxilong",yipuxilong);

}
System::~System()
{
    //解除注册
    InterfaceManager::instance()->unregisterPage("/MainWindow/Beta");
    InterfaceManager::instance()->unregisterPage("/MainWindow/Beta/Gama");
    InterfaceManager::instance()->unregisterPage("/MainWindow/Beta/Xita");
    InterfaceManager::instance()->unregisterPage("/MainWindow/Beta/Yipuxilong");
}
