#include "graphelement.h"

QPixmap GraphNumLine::DrawPixMap(QPixmap pix)
{
    QPainter painterPix(&pix);
    painterPix.setRenderHint(QPainter::Antialiasing, true);
    if (element == NULL) return pix;
    if (element->graphType == NumLine){
        painterPix.drawLine(UNIT_WIDTH-1, 0, UNIT_WIDTH-1, UNIT_HEIGH);
    }
    return pix;
}


QPixmap GraphVLine::DrawPixMap(QPixmap pix)
{
    QPainter painterPix(&pix);
    painterPix.setRenderHint(QPainter::Antialiasing, true);
    if (element == NULL) return pix;

    if (m_phase==0){
       painterPix.drawLine(1, UNIT_HEIGH-UNIT_HEIGH/UNIT_H_FACTOR,
                           1, UNIT_HEIGH);
    }else{
        painterPix.drawLine(1, 0,
                            1, UNIT_HEIGH-UNIT_HEIGH/UNIT_H_FACTOR);
    }
    return pix;

}
QPixmap GraphHLine::DrawPixMap(QPixmap pix)
{
    QPainter painterPix(&pix);
    painterPix.setRenderHint(QPainter::Antialiasing, true);
    if (element == NULL) return pix;
    painterPix.drawLine(0,UNIT_HEIGH-UNIT_HEIGH/UNIT_H_FACTOR,
                        UNIT_WIDTH, UNIT_HEIGH-UNIT_HEIGH/UNIT_H_FACTOR);
    return pix;

}
QPixmap GraphRLogic::DrawPixMap(QPixmap pix)
{
    QPainter painterPix(&pix);
    painterPix.setRenderHint(QPainter::Antialiasing, true);
    if (element == NULL) return pix;
    painterPix.drawLine(0,UNIT_HEIGH-UNIT_HEIGH/UNIT_H_FACTOR,
                        UNIT_WIDTH, UNIT_HEIGH-UNIT_HEIGH/UNIT_H_FACTOR);
    painterPix.drawLine(UNIT_WIDTH/2-10, UNIT_HEIGH-UNIT_HEIGH/UNIT_H_FACTOR-10,
                        UNIT_WIDTH/2+10, UNIT_HEIGH-UNIT_HEIGH/UNIT_H_FACTOR+10);
    return pix;
}

QPixmap GraphInput::DrawPixMap(QPixmap pix)
{
    QPainter painterPix(&pix);
    if (element == NULL) return pix;
    painterPix.setRenderHint(QPainter::Antialiasing, true);

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


    QFontMetrics fm = painterPix.fontMetrics();
    QString text = QString("%1%2").arg(element->name).arg(element->index);
    QPoint pt1(UNIT_WIDTH/2-fm.width(text)/2-1, 5);
    QPoint pt2(UNIT_WIDTH/2+fm.width(text)/2+1, 2*fh);
    QRect rect(pt1, pt2);
    QFont font;

    font.setPointSize(10);

    painterPix.setFont(font);
    painterPix.setPen(Qt::black);
    painterPix.drawText(rect, text);

    return pix;
}




QPixmap GraphOutput::DrawPixMap(QPixmap pix)
{
    QPainter painterPix(&pix);
    if (element == NULL) return pix;
    painterPix.setRenderHint(QPainter::Antialiasing, true);

    int fh = UNIT_HEIGH/UNIT_H_FACTOR;

    QPointF p1(2, 3*fh-fh/2);
    QPointF p2(2, 3*fh+fh/2);
    QPointF p3(5, 3*fh-fh/2-2);
    QPointF p4(5, 3*fh+fh/2+2);

    QPointF p5(UNIT_WIDTH-2, 3*fh-fh/2);
    QPointF p6(UNIT_WIDTH-2, 3*fh+fh/2);
    QPointF p7(UNIT_WIDTH-5, 3*fh-fh/2-2);
    QPointF p8(UNIT_WIDTH-5, 3*fh+fh/2+2);



    painterPix.drawLine(p1, p2);
    painterPix.drawLine(p1, p3);
    painterPix.drawLine(p2, p4);

    painterPix.drawLine(p5, p6);
    painterPix.drawLine(p5, p7);
    painterPix.drawLine(p6, p8);

    QFontMetrics fm = painterPix.fontMetrics();
    QString text = QString("%1%2").arg(element->name).arg(element->index);
    QPoint pt1(UNIT_WIDTH/2-fm.width(text)/2-1, 2*fh+5);
    QPoint pt2(UNIT_WIDTH/2+fm.width(text)/2+1, 4*fh);
    QRect rect(pt1, pt2);
    QFont font;

    font.setPointSize(10);


    painterPix.setFont(font);
    painterPix.setPen(Qt::black);
    painterPix.drawText(rect, text);

    return pix;
}
