#include "system.h"
#include "betawindow.h"
#include "gamawindow.h"
#include "homewindow.h"
#include "loginwindow.h"
#include "maininterface.h"
#include "userwindow.h"
#include "xitawindow.h"
#include "yipuxilong.h"

System::System() {}
void System::start()
{
    // 创建子界面,并且注册进调度器
    BetaWindow *beta = new BetaWindow();
    GamaWindow *gamawindow = new GamaWindow();
    XitaWindow *xitawindow = new XitaWindow();
    Yipuxilong *yipuxilong = new Yipuxilong();
    Userwindow *userWindow = new Userwindow();       //注册界面
    loginWindow *login = new loginWindow();          //登陆界面
    maininterface *mainScreen = new maininterface(); //应用主界面
    homeWindow *homePage = new homeWindow();         //首页界面

    InterfaceManager::instance()->registerPage("lxt_loginWindow", login); //登录界面

    InterfaceManager::instance()->registerPage("fzj_window", userWindow); //注册界面

    InterfaceManager::instance()->registerPage("lxt_mainInterface", mainScreen); //应用主界面

    InterfaceManager::instance()->registerPage("lxt_homeWindow", homePage); //首页界面

    InterfaceManager::instance()->registerPage("/MainWindow/Beta", beta);
    //注册gama界面
    InterfaceManager::instance()->registerPage("/MainWindow/Beta/Gama", gamawindow);
    //注册西塔
    InterfaceManager::instance()->registerPage("/MainWindow/Beta/Xita", xitawindow);
    //注册伊普西隆
    InterfaceManager::instance()->registerPage("/MainWindow/Beta/Yipuxilong", yipuxilong);

    InterfaceManager::instance()->switchToPage("lxt_loginWindow");
}
System::~System()
{
    //解除注册
    InterfaceManager::instance()->unregisterPage("/MainWindow/Beta");
    InterfaceManager::instance()->unregisterPage("/MainWindow/Beta/Gama");
    InterfaceManager::instance()->unregisterPage("/MainWindow/Beta/Xita");
    InterfaceManager::instance()->unregisterPage("/MainWindow/Beta/Yipuxilong");
}
