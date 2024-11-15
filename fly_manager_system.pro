QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    betawindow.cpp \
    gamawindow.cpp \
    interfacemanager.cpp \
    main.cpp \
    mainwindow.cpp \
    xitawindow.cpp \
    yipuxilong.cpp

HEADERS += \
    betawindow.h \
    gamawindow.h \
    interfacemanager.h \
    mainwindow.h \
    xitawindow.h \
    yipuxilong.h

FORMS += \
    betawindow.ui \
    gamawindow.ui \
    mainwindow.ui \
    xitawindow.ui \
    yipuxilong.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md \
    REQUIRE.md  \
    TechnologyStack.md
