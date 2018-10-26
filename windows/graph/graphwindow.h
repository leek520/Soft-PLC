#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H
/**
Copyright (c) 2018 PLT. All Rights Reserved.
    *@file：
    *@brief： 梯形图编辑窗口
    *@author ：李奎
    *@date ：2018/10/01
    *@description：
*/
#include <QTableWidget>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QStyledItemDelegate>
#include <QScrollBar>
#include <QMouseEvent>
#include <QPainter>
#include <QSpinBox>
#include <QDebug>
#include <QMenu>
#include <QMessageBox>
#include <QApplication>
#include "common.h"
#include "graphview.h"
class GraphWindow : public QWidget
{
    Q_OBJECT
public:
    explicit GraphWindow(QWidget *parent = 0);
    virtual ~ GraphWindow(){}
signals:

public slots:

    void SaveGraph(QString name);
    void OpenGraph(QString name);
public:
    GraphView *m_graphView;
};

#endif // GRAPHWINDOW_H
