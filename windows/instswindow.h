#ifndef INSTSWINDOW_H
#define INSTSWINDOW_H
/**
Copyright (c) 2018 PLT. All Rights Reserved.
    *@file：
    *@brief： 指令模式窗口
    *@author ：李奎
    *@date ：2018/10/01
    *@description：
*/
#include <QTableWidget>
#include <QTableView>
#include <QHeaderView>
#include "common.h"
#include "graphfb.h"
class InstsWindow : public QTableWidget
{
    Q_OBJECT
public:
    explicit InstsWindow(QWidget *parent = 0);
    void setZoom(double factor);
signals:

private:
    void setItemFormat(int row, int col);
public slots:
    void slt_IsertInst(int row, QString inst="NOP", QString opt="");
};

#endif // INSTSWINDOW_H
