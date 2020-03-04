#-------------------------------------------------
#
# Project created by QtCreator 2012-09-02T22:28:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SiemensScreenshotTool
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    crc16.cpp \
    comport.cpp \
    bfc.cpp \
    comconnection.cpp \
    about.cpp \
    screenwidget.cpp

HEADERS  += mainwindow.h \
    crc16.h \
    comport.h \
    bfc.h \
    comconnection.h \
    about.h \
    screenwidget.h

FORMS    += mainwindow.ui \
    about.ui

RESOURCES += \
    resources.qrc
