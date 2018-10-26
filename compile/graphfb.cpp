#include "graphfb.h"
double GraphFB::g_factor = 1.0;
int GraphFB::g_unitWidth = UNIT_WIDTH;
int GraphFB::g_unitHeight = UNIT_HEIGH;

GraphFB::GraphFB()
{

}
/******************************************************************************
* @brief: 根据图元结构体数据初始化图元
* @author:leek
* @date 2018/10/10
*******************************************************************************/
GraphFB::GraphFB(Element element) : emt(element)
{
    conColor = QColor(Qt::black);
    entColor = QColor(Qt::black);
    backColor = QColor(Qt::white);
}
/******************************************************************************
* @brief: 根据行列初始化图元
* @author:leek
* @date 2018/10/10
*******************************************************************************/
GraphFB::GraphFB(int row, int col)
{
    emt.row = row;
    emt.col = col;
    emt.width = 1;
    emt.height = 1;
    emt.funInsType = 0;
    emt.graphType = 0;
    emt.index = 0;
    emt.dnFlag = false;
    emt.upFlag = false;
    emt.name = "";
    emt.mark = "";
    conColor = QColor(Qt::black);
    entColor = QColor(Qt::black);
    backColor = QColor(Qt::white);
}

GraphFB::~GraphFB()
{

}
/******************************************************************************
* @brief: 根据图源数据绘制图形
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphFB::drawGraph()
{
    int width = g_unitWidth * emt.width;
    int height = g_unitHeight * emt.height;

    QPixmap pix(width, height);
    pix.fill(backColor);

    QPainter painter(&pix);
    painter.setPen(QPen(entColor, g_unitWidth/UNIT_WIDTH));
    //painter.setPen(entColor);
    painter.setRenderHint(QPainter::Antialiasing, true);

    //竖直线
    if (emt.dnFlag){
        painter.drawLine(1, height-height/UNIT_H_FACTOR,
                            1, height);
    }
    if (emt.upFlag){
        painter.drawLine(1, 0,
                            1, height-height/UNIT_H_FACTOR);
    }
    //画功能图
    int fw = width/UNIT_W_FACTOR;
    int fh = height/UNIT_H_FACTOR;

    //写字，设置字体
    QFontMetrics fm = painter.fontMetrics();
    QString text = QString("%1%2").arg(emt.name).arg(emt.index);
    QFont font;
    font.setPixelSize(height/4); //80-10
    painter.setFont(font);

    //根据类别画相应的图形
    QPointF p1(0, height-fh);
    QPointF p2(width, height-fh);

    switch (emt.graphType) {
    case NumLine:
        painter.drawLine(width-1, 0, width-1, height);
        break;

    case HorizontalLine:
        painter.setPen(QPen(conColor, g_unitWidth/UNIT_WIDTH));
        painter.drawLine(p1, p2);
        break;
    case ReverseLogic:
    {
        QPointF offset(width/6, height/6);
        QPointF p0(width/2, height-fh);
        painter.setPen(QPen(conColor, g_unitWidth/UNIT_WIDTH));
        painter.drawLine(p1, p2);
        painter.drawLine(p0-offset, p0+offset);
        break;
    }
    case InputOpen:
    case InputClose:
    {
        QPointF p3(fw, height-fh);
        QPointF p4(width-fw, height-fh);

        QPointF p5(fw, height-2*fh);
        QPointF p6(fw, height);
        QPointF p7(width-fw, height-2*fh);
        QPointF p8(width-fw, height);

        QPointF offset(width/30, -1*height/20);

        painter.setPen(QPen(conColor, g_unitWidth/UNIT_WIDTH));
        painter.drawLine(p1, p3);
        painter.drawLine(p2, p4);
        painter.setPen(QPen(entColor, g_unitWidth/UNIT_WIDTH));
        painter.drawLine(p5, p6);
        painter.drawLine(p7, p8);

        if (emt.graphType == InputClose)
            painter.drawLine(p6 + offset, p7 - offset);

        QPoint pt((width-fm.width(text))/2,
                  (height/2-(fm.ascent()+fm.descent()))/2+fm.ascent());
        painter.drawText(pt, text);
        break;
    }
    case InputPedge:
    case InputNedge:
    {
        QPointF p3(fw, height-fh);
        QPointF p4(width-fw, height-fh);

        QPointF p5(fw, height-2*fh);
        QPointF p6(fw, height);
        QPointF p7(width-fw, height-2*fh);
        QPointF p8(width-fw, height);

        QPointF p09(width/2, height-2*fh);
        QPointF p10(width/2, height);

        painter.setPen(QPen(conColor, g_unitWidth/UNIT_WIDTH));
        painter.drawLine(p1, p3);
        painter.drawLine(p2, p4);
        painter.setPen(QPen(entColor, g_unitWidth/UNIT_WIDTH));
        painter.drawLine(p5, p6);
        painter.drawLine(p7, p8);

        painter.drawLine(p09, p10);
        if (emt.graphType == InputPedge){
            painter.drawLine(p3 + QPointF(width/25, 1*height/20), p09);
            painter.drawLine(p4 + QPointF(-width/25, 1*height/20), p09);
        }else{
            painter.drawLine(p3 + QPointF(width/25, -1*height/20), p10);
            painter.drawLine(p4 + QPointF(-width/25, -1*height/20), p10);
        }
        QPoint pt((width-fm.width(text))/2,
                  (height/2-(fm.ascent()+fm.descent()))/2+fm.ascent());
        painter.drawText(pt, text);
        break;
    }
    case StepGraph:
    {
        fw = width/5;
        QPointF p3(fw, height-fh);
        QPointF p4(width-fw, height-fh);

        QPointF p5(2*fw, height-2*fh);
        QPointF p6(2*fw, height);

        QPointF p7(width-2*fw, height-2*fh);
        QPointF p8(width-2*fw, height);


        painter.drawLine(p1, p3);
        painter.drawLine(p2, p4);

        painter.drawLine(p3, p5);
        painter.drawLine(p3, p6);

        painter.drawLine(p4, p7);
        painter.drawLine(p4, p8);

        QPoint pt2((width-fm.width(text))/2,
                  (height/2-(fm.ascent()+fm.descent()))/2+fm.ascent());
        painter.drawText(pt2, text);

        text = "S";
        QPoint pt1((width-fm.width(text))/2,
                  (height*3/2-(fm.ascent()+fm.descent()))/2+fm.ascent());
        painter.drawText(pt1, text);
        break;
    }
    case OutputGraph:
    {
        int xof1 = width / 40;
        int xof2 = width / 20;
        QPointF p1(xof1, 3*fh-fh/2);
        QPointF p2(xof1, 3*fh+fh/2);
        QPointF p3(xof2, 3*fh-fh/2-(xof2-xof1));
        QPointF p4(xof2, 3*fh+fh/2+(xof2-xof1));

        QPointF p5(width-xof1, 3*fh-fh/2);
        QPointF p6(width-xof1, 3*fh+fh/2);
        QPointF p7(width-xof2, 3*fh-fh/2-(xof2-xof1));
        QPointF p8(width-xof2, 3*fh+fh/2+(xof2-xof1));



        painter.drawLine(p1, p2);
        painter.drawLine(p1, p3);
        painter.drawLine(p2, p4);

        painter.drawLine(p5, p6);
        painter.drawLine(p5, p7);
        painter.drawLine(p6, p8);

        QPoint pt((width-fm.width(text))/2,
                  (height*3/2-(fm.ascent()+fm.descent()))/2+fm.ascent());
        painter.drawText(pt, text);
        break;
    }
    case LogicGraph:
    {
        QPointF p1(1, height-3*fh);
        QPointF p2(1, height-1);
        QPointF p3(width-1, height-3*fh);
        QPointF p4(width-1, height-1);
        painter.drawLine(p3, p4);
        painter.drawLine(p1, p2);
        painter.drawLine(p1, p3);
        painter.drawLine(p2, p4);

        text = QString("%1").arg(emt.mark);
        QPoint pt((width-fm.width(text)*g_factor)/2,
                  (height*4/3-(fm.ascent()+fm.descent()))/2+fm.ascent());
        painter.drawText(pt, text);
        break;
    }
    case EndGraph:
    {
        QPointF p1(1, height-2*fh);
        QPointF p2(1, height-1);
        QPointF p3(width-1, height-2*fh);
        QPointF p4(width-1, height-1);
        painter.drawLine(p1, p2);
        painter.drawLine(p3, p4);
        painter.drawLine(p1, p3);
        painter.drawLine(p2, p4);

        text = QString("%1").arg(emt.name);
        QPoint pt((width-fm.width(text))/2,
                  (height*3/2-(fm.ascent()+fm.descent()))/2+fm.ascent());
        painter.drawText(pt, text);
        break;
    }
    default:
        break;
    }
    pixMap = pix;
}
/******************************************************************************
* @brief: 设置图源数据
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphFB::setEelment(Element element)
{
    emt = element;
}
/******************************************************************************
* @brief: 获取图源数据
* @author:leek
* @date 2018/10/10
*******************************************************************************/
Element GraphFB::getEelment()
{
    return emt;
}
/******************************************************************************
* @brief: 清空图源数据，包括图形和竖直线
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphFB::clearAll()
{
    clearEelment();
    clearVupLine();
    clearVdnLine();
}
/******************************************************************************
* @brief: 清空图源数据
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphFB::clearEelment()
{
    emt.graphType = NoneGraph;
    emt.funInsType = 0;
    emt.height = 1;
    emt.width = 1;
    emt.index = 0;
    emt.name = "";
    emt.mark = "";

}
/******************************************************************************
* @brief: 清理竖直线下半部分
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphFB::clearVupLine()
{
    emt.upFlag = false;
}
/******************************************************************************
* @brief: 清理竖直线上半部分
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphFB::clearVdnLine()
{
    emt.dnFlag = false;
}

void GraphFB::setVupLine(bool status)
{
    emt.upFlag = status;
}

void GraphFB::setVdnLine(bool status)
{
    emt.dnFlag = status;
}

void GraphFB::setBackColor(QColor color)
{
    backColor = color;
}

bool GraphFB::isDown()
{
    return emt.dnFlag;
}

bool GraphFB::isUp()
{
    return emt.upFlag;
}

bool GraphFB::isEmpty()
{
    if (emt.dnFlag || emt.upFlag || emt.graphType){
        return false;
    }else{
        return true;
    }

}

int GraphFB::getType()
{
    return emt.graphType;
}

QString GraphFB::getName()
{
    return emt.name;
}

int GraphFB::getIndex()
{
    return emt.index;
}

int GraphFB::row()
{
    return emt.row;
}

int GraphFB::column()
{
    return emt.col;
}
/******************************************************************************
* @brief: 放大缩小函数，计算比例尺，in=true为放大，否则缩小
* @author:leek
* @date 2018/10/10
*******************************************************************************/
bool GraphFB::zoom(bool in)
{
    if (in){    //放大
        if ((g_unitWidth / UNIT_WIDTH >= 4) |
           (g_unitHeight / UNIT_HEIGH >= 4))
            return false;

        g_unitWidth = g_unitWidth * UNIT_ZOOM_FACTOR;
        g_unitHeight = g_unitHeight * UNIT_ZOOM_FACTOR;
    }else{
            if ((UNIT_WIDTH / g_unitWidth >= 2) |
               (UNIT_HEIGH / g_unitHeight >= 2))
                return false;
        g_unitWidth = g_unitWidth / UNIT_ZOOM_FACTOR;
        g_unitHeight = g_unitHeight / UNIT_ZOOM_FACTOR;
    }
    g_factor = (double)g_unitWidth / UNIT_WIDTH;
    return true;
}
/******************************************************************************
* @brief: 保存文件时调用，导出图元结构体数据
* @author:leek
* @date 2018/10/10
*******************************************************************************/
QDataStream & operator<<(QDataStream &stream, GraphFB &graph)
{
    Element pEmt = graph.emt;

    stream  << pEmt.row
            << pEmt.col
            << pEmt.width
            << pEmt.height
            << pEmt.graphType
            << pEmt.funInsType
            << pEmt.index
            << pEmt.upFlag
            << pEmt.dnFlag
            << pEmt.name
            << pEmt.mark;

    return stream;
}
/******************************************************************************
* @brief: 打开文件时调用，导入图元结构体数据
* @author:leek
* @date 2018/10/10
*******************************************************************************/
QDataStream & operator>>(QDataStream &stream, GraphFB &graph)
{
    Element pEmt;
    stream  >> pEmt.row
            >> pEmt.col
            >> pEmt.width
            >> pEmt.height
            >> pEmt.graphType
            >> pEmt.funInsType
            >> pEmt.index
            >> pEmt.upFlag
            >> pEmt.dnFlag
            >> pEmt.name
            >> pEmt.mark;
    graph.emt = pEmt;
    return stream;
}
