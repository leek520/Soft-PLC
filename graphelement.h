/**
Copyright (c) 2018 PLT. All Rights Reserved.
    *@file：
    *@brief：
    *@author ：李奎
    *@date ：2018/10/01
    *@description：基本图元结构，包括常开触点，常闭触点，
    * （取反）输出线圈
*/
#ifndef GRAPHELEMENT_H
#define GRAPHELEMENT_H
#include <QPixmap>
class GraphElement
{
public:
    GraphElement(QPixmap *pix1);

    virtual ~GraphElement();
protected:
    virtual void draw();
public:
    QString address;    //对应的输入地址
    QPoint point;       //所画图元的起始坐标
    QPixmap *pix2;      //用作绘图设备
};

#endif // GRAPHELEMENT_H
