#-------------------------------------------------
#
# Project created by QtCreator 2016-10-18T19:07:52
#
#-------------------------------------------------

QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = serialporthelper
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    console.cpp \
    settingsdialog.cpp \
    showwave.cpp \
    videoshow.cpp \
    common.cpp

HEADERS  += mainwindow.h \
    console.h \
    settingsdialog.h \
    showwave.h \
    common.h \
    videoshow.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    showwave.ui \
    videoshow.ui

RESOURCES += \
    terminal.qrc

DEFINES += QT_DLL QWT_DLL

INCLUDEPATH += F:\QT_Development\qt_5.6.1\5.6\mingw49_32\include\QWT
INCLUDEPATH += F:\OpenCV_Development\opencv\build\x86\mingw\install\include
INCLUDEPATH += F:\OpenCV_Development\opencv\build\x86\mingw\install\include\opencv\
INCLUDEPATH += F:\OpenCV_Development\opencv\build\x86\mingw\install\include\opencv2\

LIBS += -L"F:\QT_Development\qt_5.6.1\5.6\mingw49_32\lib" -lqwtd
#LIBS += -L"F:\QT_Development\qt_5.6.1\5.6\mingw49_32\lib" -lqwt
LIBS += F:\OpenCV_Development\opencv\build\x86\mingw\install\x86\mingw\lib\libopencv_core300.dll.a
LIBS += F:\OpenCV_Development\opencv\build\x86\mingw\install\x86\mingw\lib\libopencv_highgui300.dll.a
LIBS += F:\OpenCV_Development\opencv\build\x86\mingw\install\x86\mingw\lib\libopencv_imgproc300.dll.a
LIBS += F:\OpenCV_Development\opencv\build\x86\mingw\install\x86\mingw\lib\libopencv_imgcodecs300.dll.a
LIBS += F:\OpenCV_Development\opencv\build\x86\mingw\install\x86\mingw\lib\libopencv_calib3d300.dll.a
LIBS += F:\OpenCV_Development\opencv\build\x86\mingw\install\x86\mingw\lib\libopencv_features2d300.dll.a
LIBS += F:\OpenCV_Development\opencv\build\x86\mingw\install\x86\mingw\lib\libopencv_flann300.dll.a
LIBS += F:\OpenCV_Development\opencv\build\x86\mingw\install\x86\mingw\lib\libopencv_hal300.a
LIBS += F:\OpenCV_Development\opencv\build\x86\mingw\install\x86\mingw\lib\libopencv_ml300.dll.a
LIBS += F:\OpenCV_Development\opencv\build\x86\mingw\install\x86\mingw\lib\libopencv_objdetect300.dll.a
LIBS += F:\OpenCV_Development\opencv\build\x86\mingw\install\x86\mingw\lib\libopencv_photo300.dll.a
LIBS += F:\OpenCV_Development\opencv\build\x86\mingw\install\x86\mingw\lib\libopencv_shape300.dll.a
LIBS += F:\OpenCV_Development\opencv\build\x86\mingw\install\x86\mingw\lib\libopencv_stitching300.dll.a
LIBS += F:\OpenCV_Development\opencv\build\x86\mingw\install\x86\mingw\lib\libopencv_superres300.dll.a
LIBS += F:\OpenCV_Development\opencv\build\x86\mingw\install\x86\mingw\lib\libopencv_ts300.a
LIBS += F:\OpenCV_Development\opencv\build\x86\mingw\install\x86\mingw\lib\libopencv_video300.dll.a
LIBS += F:\OpenCV_Development\opencv\build\x86\mingw\install\x86\mingw\lib\libopencv_videoio300.dll.a
LIBS += F:\OpenCV_Development\opencv\build\x86\mingw\install\x86\mingw\lib\libopencv_videostab300.dll.a
