#-------------------------------------------------
#
# Project created by QtCreator 2018-09-27T11:31:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Soft-PLC
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphelement.cpp \
    graphfb.cpp \
    graphwindow.cpp

HEADERS  += mainwindow.h \
    graphelement.h \
    graphfb.h \
    common.h \
    graphwindow.h

RESOURCES += \
    application.qrc
