#include "graphfb.h"

int GraphFB::g_unitWidth = UNIT_WIDTH;
int GraphFB::g_unitHeight = UNIT_HEIGH;

GraphFB::GraphFB()
{

}

GraphFB::GraphFB(Element element) : emt(element)
{
    conColor = QColor(Qt::black);
    entColor = QColor(Qt::black);
}

GraphFB::GraphFB(int row, int col)
{
    emt = {row, col, 1, 1, 0, 0, 0, false, false, "", ""};
}

GraphFB::~GraphFB()
{

}

void GraphFB::drawGraph()
{
    int width = g_unitWidth * emt.width;
    int height = g_unitHeight * emt.height;

    QPixmap pix(width, height);
    pix.fill(Qt::white);

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
    font.setPixelSize(width/6); //80-10
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
    case OutputNode:
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
    }
        break;
    default:
        break;
    }
    pixMap = pix;
}

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
