QT       += core gui qmqtt sql serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    login.cpp \
    logindata.cpp \
    main.cpp \
    mainwindow.cpp \
    registration.cpp \
    sqldata.cpp

HEADERS += \
    login.h \
    logindata.h \
    mainwindow.h \
    registration.h \
    sqldata.h

FORMS += \
    login.ui \
    mainwindow.ui \
    registration.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
