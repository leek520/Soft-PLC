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
    graphfb.cpp \
    graphwindow.cpp \
    inputwindow.cpp \
    instswindow.cpp

HEADERS  += mainwindow.h \
    graphfb.h \
    common.h \
    graphwindow.h \
    inputwindow.h \
    instswindow.h

RESOURCES += \
    application.qrc

RC_ICONS = mamtool.ico
