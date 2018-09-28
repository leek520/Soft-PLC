#include "graphelement.h"

QPixmap GraphLine::DrawPixMap()
{
    QPixmap pix(LINE_WIDTH, UNIT_HEIGH);
    pix.fill(Qt::transparent);
    QPainter painterPix(&pix);
    painterPix.setRenderHint(QPainter::Antialiasing, true);
    if (element == NULL) return pix;
    if (element->graphType == VerticalLine){
        painterPix.drawLine(LINE_WIDTH-1, 0, LINE_WIDTH-1, UNIT_HEIGH);
    }

    return pix;
}


QPixmap GraphBlank::DrawPixMap()
{

    QPixmap pix(UNIT_WIDTH, UNIT_HEIGH);
    pix.fill(Qt::transparent);//用透明色填充
    QPainter painterPix(&pix);
    painterPix.setRenderHint(QPainter::Antialiasing, true);
    if (element == NULL) return pix;
    painterPix.drawLine(0, UNIT_HEIGH*2/3, UNIT_WIDTH, UNIT_HEIGH*2/3);
    return pix;

}


QPixmap GraphInput::DrawPixMap()
{
    QPixmap pix(UNIT_WIDTH, UNIT_HEIGH);
    pix.fill(Qt::transparent);

    QPainter painterPix(&pix);
    if (element == NULL) return pix;
    painterPix.setRenderHint(QPainter::Antialiasing, true);
    painterPix.setPen(QPen(Qt::black,1,Qt::SolidLine) ) ;//设置画笔的类型：笔头大小、划线类型、颜色

    int fw = UNIT_WIDTH/UNIT_W_FACTOR;
    int fh = UNIT_HEIGH/UNIT_H_FACTOR;

    QPointF p1(0, UNIT_HEIGH-fh);
    QPointF p2(fw, UNIT_HEIGH-fh);
    QPointF p3(UNIT_WIDTH-fw, UNIT_HEIGH-fh);
    QPointF p4(UNIT_WIDTH, UNIT_HEIGH-fh);

    QPointF p5(fw, UNIT_HEIGH-2*fh);
    QPointF p6(fw, UNIT_HEIGH);
    QPointF p7(UNIT_WIDTH-fw, UNIT_HEIGH-2*fh);
    QPointF p8(UNIT_WIDTH-fw, UNIT_HEIGH);



    painterPix.drawLine(p1, p2);
    painterPix.drawLine(p3, p4);
    painterPix.drawLine(p5, p6);
    painterPix.drawLine(p7, p8);
    if (element->graphType == InputClose)
        painterPix.drawLine(p6 + QPointF(2,-2), p7 - QPointF(2,-2));

    //return pix;

    QPoint pt1(fw+5, 5);
    QPoint pt2(UNIT_WIDTH-fw, 2*fh);
    QRect rect(pt1, pt2);
    QFont font;
    QString text = QString("%1%2").arg(element->name).arg(element->index);
    font.setPointSize(10);

    painterPix.setFont(font);
    painterPix.setPen(Qt::black);
    painterPix.drawText(rect, text);

    return pix;
}
