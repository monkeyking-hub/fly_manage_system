#include "system.h"
#include "gamawindow.h"
#include "homewindow.h"
#include "maininterface.h"
#include "userwindow.h"
#include "xitawindow.h"
#include "yipuxilong.h"
#include "registerwindow.h"
#include "newhomewindow.h"
#include "flightstatus.h"
#include "orderwindow.h"
#include "interfacemanager.h"
#include "newloginwindow.h"
#include "newregisterwindow.h"
#include "adminloginwindow.h"
#include <QApplication>
#include <QWidget>
#include <QMainWindow>

System::System():m_mainInterface(nullptr) {}

void System::start()
{
    // 创建子界面,并且注册进调度器
    GamaWindow * gamawindow = new GamaWindow();
    XitaWindow * xitawindow = new XitaWindow();
    Yipuxilong * yipuxilong = new Yipuxilong();
    Userwindow * userWindow = new Userwindow(); //修改个人信息界面
    m_mainInterface = new maininterface(); //应用主界面
    homeWindow *homePage = new homeWindow(); //homeWindow界面
    registerWindow *signUpWindow = new registerWindow(); //注册界面
    newHomeWindow *newHomePage = new newHomeWindow(); //新首页界面
    flightstatus* flightstatusWindow= new flightstatus(); //航班动态界面
    orderwindow* orderPage = new orderwindow(); //订单界面
    newLoginWindow *newLogin = new newLoginWindow(); //新用户登录界面
    loginHandler *loginHand = new loginHandler(); //loginHand用于用户登录界面的登录按钮信号与槽连接
    newRegisterWindow *newSignUpWindow = new newRegisterWindow(); //新注册界面
    registerHandler *registerHand = new registerHandler(); //registerHand用于用户注册界面的注册按钮信号与槽连接
    adminLoginWindow *adminLogin = new adminLoginWindow(); //管理员登录界面
    adminLoginHandler *adminLoginHand = new adminLoginHandler(); //adminLoginHand用于管理员登录界面的登录按钮信号与槽连接

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

    connect(newLogin,&newLoginWindow::loginRequested,loginHand,&loginHandler::handleLogin);//用户登录界面的登录按钮信号与槽连接

    connect(newSignUpWindow,&newRegisterWindow::registerRequested,registerHand,&registerHandler::handleRegister);//用户注册界面的注册按钮信号与槽连接

    connect(adminLogin,&adminLoginWindow::adminLoginRequested,adminLoginHand,&adminLoginHandler::handleAdminLogin); //管理员登录界面的登录按钮信号与槽连接

    InterfaceManager::instance()->registerPage("fzj_window",userWindow); //修改个人信息界面

    InterfaceManager::instance()->registerPage("fzj_flightstatus",flightstatusWindow); //航班动态界面

    InterfaceManager::instance()->registerPage("lxt_mainInterface", m_mainInterface); //应用主界面

    InterfaceManager::instance()->registerPage("lxt_homeWindow",homePage); //homeWindow界面

    InterfaceManager::instance()->registerPage("lxt_registerWindow",signUpWindow); //注册界面

    InterfaceManager::instance()->registerPage("lxt_newHomeWindow",newHomePage); //新首页界面

    InterfaceManager::instance()->registerPage("wmc_orderWindow",orderPage); //订单界面

    InterfaceManager::instance()->registerPage("lxt_newLoginWindow",newLogin); //新登录界面

    InterfaceManager::instance()->registerPage("lxt_newRegisterWindow",newSignUpWindow); //新注册界面

    InterfaceManager::instance()->registerPage("lxt_adminLoginWindow",adminLogin); //管理员登录界面

    //注册gama界面
    InterfaceManager::instance()->registerPage("/MainWindow/Beta/Gama", gamawindow);
    //注册西塔
    InterfaceManager::instance()->registerPage("/MainWindow/Beta/Xita", xitawindow);
    //注册伊普西隆
    InterfaceManager::instance()->registerPage("/MainWindow/Beta/Yipuxilong", yipuxilong);

    InterfaceManager::instance()->switchToPage("lxt_newLoginWindow");
}
System::~System()
{
    //解除注册
    InterfaceManager::instance()->unregisterPage("/MainWindow/Beta");
    InterfaceManager::instance()->unregisterPage("/MainWindow/Beta/Gama");
    InterfaceManager::instance()->unregisterPage("/MainWindow/Beta/Xita");
    InterfaceManager::instance()->unregisterPage("/MainWindow/Beta/Yipuxilong");
}
