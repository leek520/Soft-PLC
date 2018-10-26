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
INCLUDEPATH += ./compile/
INCLUDEPATH += ./comm/
INCLUDEPATH += ./windows/
INCLUDEPATH += ./windows/instruction/
INCLUDEPATH += ./windows/graph/
INCLUDEPATH += ./windows/input/

SOURCES +=  main.cpp\
            windows/mainwindow.cpp \
            windows/graph/graphwindow.cpp \
            windows/input/inputwindow.cpp \
            windows/instruction/instswindow.cpp \
    windows/graph/graphmodel.cpp \
    windows/graph/graphdelegate.cpp \
    windows/graph/graphview.cpp


HEADERS  += include/common.h \
            windows/mainwindow.h \
            windows/graph/graphwindow.h \
            windows/input/inputwindow.h \
            windows/instruction/instswindow.h \
    windows/graph/graphmodel.h \
    windows/graph/graphdelegate.h \
    windows/graph/graphview.h


RESOURCES += application.qrc

RC_ICONS = mamtool.ico
