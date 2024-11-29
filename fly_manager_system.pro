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
    flightstatus.cpp \
    gamawindow.cpp \
    homewindow.cpp \
    inorder.cpp \
    interfacemanager.cpp \
    main.cpp \
    maininterface.cpp \
    mainwindow.cpp \
    newhomewindow.cpp \
    newloginwindow.cpp \
    orderwindow.cpp \
    registerwindow.cpp \
    system.cpp \
    userwindow.cpp \
    xitawindow.cpp \
    yipuxilong.cpp

HEADERS += \
    flightstatus.h \
    gamawindow.h \
    homewindow.h \
    inorder.h \
    interfacemanager.h \
    maininterface.h \
    mainwindow.h \
    newhomewindow.h \
    newloginwindow.h \
    orderwindow.h \
    registerwindow.h \
    system.h \
    userwindow.h \
    xitawindow.h \
    yipuxilong.h

FORMS += \
    flightstatus.ui \
    gamawindow.ui \
    homewindow.ui \
    inorder.ui \
    maininterface.ui \
    mainwindow.ui \
    newhomewindow.ui \
    newloginwindow.ui \
    orderwindow.ui \
    registerwindow.ui \
    userwindow.ui \
    xitawindow.ui \
    yipuxilong.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md \
    REQUIRE.md  \
    TechnologyStack.md \
    meeting1.md

RESOURCES += \
    fontstyle.qrc \
    images.qrc
