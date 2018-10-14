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

INCLUDEPATH += ./include/
INCLUDEPATH += ./windows/
INCLUDEPATH += ./windows/model/

SOURCES +=  main.cpp\
            windows/mainwindow.cpp \
            windows/graphwindow.cpp \
            windows/inputwindow.cpp \
            windows/instswindow.cpp \
            windows/model/graphfb.cpp \
            windows/model/binarytree.cpp \
    windows/model/graphmodel.cpp

HEADERS  += include/common.h \
            windows/mainwindow.h \
            windows/graphwindow.h \
            windows/inputwindow.h \
            windows/instswindow.h \
            windows/model/graphfb.h \
            windows/model/binarytree.h \
    windows/model/graphmodel.h

RESOURCES += application.qrc

RC_ICONS = mamtool.ico
