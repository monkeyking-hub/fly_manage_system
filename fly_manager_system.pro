QT       += core gui
QT       += network
QT       += sql
QT       += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminaddflightwindow.cpp \
    admindeleteflightwindow.cpp \
    adminhomewindow.cpp \
    adminloginwindow.cpp \
    adminmaininterface.cpp \
    adminupdateflightwindow.cpp \
    adminusermanagerwindow.cpp \
    animatedinputfield.cpp \
    boarderitem.cpp \
    chatwindow.cpp \
    flightinfo.cpp \
    flightstatus.cpp \
    gamawindow.cpp \
    homewindow.cpp \
    icondelegate.cpp \
    interfacemanager.cpp \
    listitem.cpp \
    main.cpp \
    maininterface.cpp \
    mainwindow.cpp \
    mfindorderwindow.cpp \
    mreorder.cpp \
    newhomewindow.cpp \
    newloginwindow.cpp \
    order.cpp \
    orderdetailwindow.cpp \
    orderwidget.cpp \
    newregisterwindow.cpp \
    orderwindow.cpp \
    pay_window.cpp \
    registerwindow.cpp \
    system.cpp \
    user.cpp \
    usermanager.cpp \
    userwindow.cpp \
    xitawindow.cpp \
    yipuxilong.cpp

HEADERS += \
    adminaddflightwindow.h \
    admindeleteflightwindow.h \
    adminhomewindow.h \
    adminloginwindow.h \
    adminmaininterface.h \
    adminupdateflightwindow.h \
    adminusermanagerwindow.h \
    animatedInputField.h \
    boarderitem.h \
    chatwindow.h \
    flightinfo.h \
    flightstatus.h \
    gamawindow.h \
    homewindow.h \
    icondelegate.h \
    interfacemanager.h \
    listitem.h \
    maininterface.h \
    mainwindow.h \
    mfindorderwindow.h \
    mreorder.h \
    newhomewindow.h \
    newloginwindow.h \
    order.h \
    orderdetailwindow.h \
    orderwidget.h \
    newregisterwindow.h \
    orderwindow.h \
    pay_window.h \
    registerwindow.h \
    system.h \
    user.h \
    usermanager.h \
    userwindow.h \
    xitawindow.h \
    yipuxilong.h

FORMS += \
    adminaddflightwindow.ui \
    admindeleteflightwindow.ui \
    adminhomewindow.ui \
    adminloginwindow.ui \
    adminmaininterface.ui \
    adminupdateflightwindow.ui \
    adminusermanagerwindow.ui \
    boarderitem.ui \
    flightstatus.ui \
    gamawindow.ui \
    homewindow.ui \
    listitem.ui \
    maininterface.ui \
    mainwindow.ui \
    mfindorderwindow.ui \
    mreorder.ui \
    newhomewindow.ui \
    newloginwindow.ui \
    newregisterwindow.ui \
    orderdetailwindow.ui \
    orderwindow.ui \
    pay_window.ui \
    registerwindow.ui \
    userwindow.ui \
    xitawindow.ui \
    yipuxilong.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    API.md \
    README.md \
    REQUIRE.md  \
    TechnologyStack.md \
    meeting1.md

RESOURCES += \
    fontstyle.qrc \
    images.qrc
