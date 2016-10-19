#-------------------------------------------------
#
# Project created by QtCreator 2016-10-18T19:07:52
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = serialporthelper
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    console.cpp \
    settingsdialog.cpp \
    showwave.cpp

HEADERS  += mainwindow.h \
    console.h \
    settingsdialog.h \
    showwave.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    showwave.ui

RESOURCES += \
    terminal.qrc

DEFINES += QT_DLL QWT_DLL
LIBS += -L"F:\QT_Development\qt_5.6.1\5.6\mingw49_32\lib" -lqwtd
LIBS += -L"F:\QT_Development\qt_5.6.1\5.6\mingw49_32\lib" -lqwt
INCLUDEPATH += F:\QT_Development\qt_5.6.1\5.6\mingw49_32\include\QWT
