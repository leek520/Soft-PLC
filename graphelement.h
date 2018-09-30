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

    virtual QPixmap DrawPixMap(QPixmap pix){}

public:
    Element *element;
};

class GraphNumLine : public GraphElement
{
public:
    GraphNumLine(Element *emt = NULL) : GraphElement(emt){}
    QPixmap DrawPixMap(QPixmap pix);

};
class GraphVLine : public GraphElement
{
public:
    GraphVLine(Element *emt = NULL, int phase=0) : GraphElement(emt), m_phase(phase){}
    QPixmap DrawPixMap(QPixmap pix);
private:
    int m_phase;
};
class GraphHLine : public GraphElement
{
public:
    GraphHLine(Element *emt = NULL) : GraphElement(emt){}
    QPixmap DrawPixMap(QPixmap pix);

};
class GraphRLogic : public GraphElement
{
public:
    GraphRLogic(Element *emt = NULL) : GraphElement(emt){}
    QPixmap DrawPixMap(QPixmap pix);

};
class GraphInput : public GraphElement
{
public:
    GraphInput(Element *emt = NULL) : GraphElement(emt){}
    QPixmap DrawPixMap(QPixmap pix);

};
class GraphOutput : public GraphElement
{
public:
    GraphOutput(Element *emt = NULL) : GraphElement(emt){}
    QPixmap DrawPixMap(QPixmap pix);

};
#endif // GRAPHELEMENT_H
