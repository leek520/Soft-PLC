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
#include <QPainter>

#include "common.h"

class GraphElement
{
public:
    GraphElement(Element emt) : element(emt){}

    ~GraphElement(){}

    QPixmap DrawPixMap(QPixmap pix);


public:
    Element element;
};

#endif // GRAPHELEMENT_H
