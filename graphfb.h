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
class GraphFB
{
public:
    GraphFB();
    virtual ~GraphFB();
protected:
    virtual void draw();
public:
    int FBtype;     //用来标识不通的功能指令
    QPoint point;   //绘制图元的起点坐标
    QPixmap *pix2;  //用作绘图设备
};

#endif // GRAPHFB_H
