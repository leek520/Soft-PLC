/**
Copyright (c) 2018 PLT. All Rights Reserved.
    *@file：
    *@brief：
    *@author ：李奎
    *@date ：2018/10/01
    *@description：绘制程序时，有些功能很难用执行位运算的基本图元
    * 来实现，此时用相对复杂的功能指令编程会更方便。包括二进制加减、
    * 逻辑与、计数器、计时器等
*/
#ifndef GRAPHFB_H
#define GRAPHFB_H
#include <QPixmap>
#include <QPainter>
#include "common.h"

class GraphFB
{

public:
    GraphFB();
    GraphFB(Element element);
    GraphFB(int row, int col);
    virtual ~GraphFB();
    void drawGraph();

    //友元类，用来序列化结构体，保存/加载图元数据
    friend QDataStream& operator<<(QDataStream&,GraphFB&);
    friend QDataStream& operator>>(QDataStream&,GraphFB&);
    static bool zoom(bool in);
public:
    static double g_factor;
    static int g_unitWidth;
    static int g_unitHeight;
    Element emt;
    QPixmap pixMap;  //用作绘图设备
public:
    QColor conColor;
    QColor entColor;
};

#endif // GRAPHFB_H
