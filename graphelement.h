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
    GraphElement(Element *emt = NULL) : element(emt){}

    virtual ~GraphElement(){}

    virtual QPixmap DrawPixMap(){}
public:
    Element *element;
};

class GraphLine : public GraphElement
{
public:
    GraphLine(Element *emt = NULL) : GraphElement(emt){}
    QPixmap DrawPixMap();

};
class GraphBlank : public GraphElement
{
public:
    GraphBlank(Element *emt = NULL) : GraphElement(emt){}
    QPixmap DrawPixMap();

};
class GraphX : public GraphElement
{
public:
    GraphX(Element *emt = NULL) : GraphElement(emt){}
    QPixmap DrawPixMap();

};
#endif // GRAPHELEMENT_H
