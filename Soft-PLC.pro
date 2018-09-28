#-------------------------------------------------
#
# Project created by QtCreator 2018-09-27T11:31:28
#
#-------------------------------------------------

QT       += core gui
QT       += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Soft-PLC
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphelement.cpp \
    graphfb.cpp \
    graphwindow.cpp \
    inputwindow.cpp

HEADERS  += mainwindow.h \
    graphelement.h \
    graphfb.h \
    common.h \
    graphwindow.h \
    inputwindow.h

RESOURCES += \
    application.qrc

RC_ICONS = mamtool.ico
