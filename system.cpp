#include "system.h"
#include "betawindow.h"
#include "gamawindow.h"
#include "homewindow.h"
#include "loginwindow.h"
#include "maininterface.h"
#include "userwindow.h"
#include "xitawindow.h"
#include "yipuxilong.h"
#include "registerwindow.h"
#include "newhomewindow.h"
#include "flightstatus.h"
#include "orderwindow.h"
#include "interfacemanager.h"
#include <QApplication>
#include <QWidget>
#include <QMainWindow>

System::System():m_mainInterface(nullptr) {}

void System::start()
{
    // 创建子界面,并且注册进调度器
    BetaWindow * beta       = new BetaWindow();
    GamaWindow * gamawindow = new GamaWindow();
    XitaWindow * xitawindow = new XitaWindow();
    Yipuxilong * yipuxilong = new Yipuxilong();
    Userwindow * userWindow = new Userwindow(); //修改个人信息界面
    loginWindow *login = new loginWindow(); //登陆界面
    m_mainInterface = new maininterface(); //应用主界面
    homeWindow *homePage = new homeWindow(); //homeWindow界面
    registerWindow *signUpWindow = new registerWindow(); //注册界面
    newHomeWindow *newHomePage = new newHomeWindow(); //新首页界面
    flightstatus* flightstatusWindow= new flightstatus(); //航班动态界面
    orderwindow* orderPage = new orderwindow(); //订单界面

    // 连接 newHomeWindow 的信号到 mainInterface 的槽
    connect(newHomePage, &newHomeWindow::commandLinkButton4Clicked, m_mainInterface, [this]() {
        // 切换到航班动态界面
        QStackedWidget *stackedWidget = m_mainInterface->findChild<QStackedWidget *>();
        if (stackedWidget) {
            stackedWidget->setCurrentIndex(1); // 切换到航班动态界面
        }
    });

    connect(newHomePage, &newHomeWindow::commandLinkButton5Clicked, m_mainInterface, [this]() {
        // 切换到订单界面
        QStackedWidget *stackedWidget = m_mainInterface->findChild<QStackedWidget *>();
        if (stackedWidget) {
            stackedWidget->setCurrentIndex(2); // 切换到订单界面
        }
    });

    connect(newHomePage, &newHomeWindow::commandLinkButton5Clicked, m_mainInterface, [this]() {
        // 切换到特惠机票界面
        QStackedWidget *stackedWidget = m_mainInterface->findChild<QStackedWidget *>();
        if (stackedWidget) {
            stackedWidget->setCurrentIndex(3); // 切换到特惠机票界面
        }
    });



    InterfaceManager::instance()->registerPage("lxt_loginWindow", login); //登录界面

    InterfaceManager::instance()->registerPage("fzj_window",userWindow); //修改个人信息界面

    InterfaceManager::instance()->registerPage("fzj_flightstatus",flightstatusWindow); //航班动态界面

    InterfaceManager::instance()->registerPage("lxt_mainInterface", m_mainInterface); //应用主界面

    InterfaceManager::instance()->registerPage("lxt_homeWindow",homePage); //homeWindow界面

    InterfaceManager::instance()->registerPage("lxt_registerWindow",signUpWindow); //注册界面

    InterfaceManager::instance()->registerPage("lxt_newHomeWindow",newHomePage); //新首页界面

    InterfaceManager::instance()->registerPage("wmc_orderWindow",orderPage); //订单界面

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
