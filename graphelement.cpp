#include "graphelement.h"

QPixmap GraphLine::DrawPixMap()
{
    QPixmap pix(LINE_WIDTH, UNIT_HEIGH);
    pix.fill(Qt::white);
    QPainter painterPix(&pix);

    if (element == NULL) return pix;
    if (element->graphType == VerticalLine){
        painterPix.drawLine(LINE_WIDTH/2, 0, LINE_WIDTH/2, UNIT_HEIGH);
    }

    return pix;
}


QPixmap GraphBlank::DrawPixMap()
{

    QPixmap pix(UNIT_WIDTH, UNIT_HEIGH);
    pix.fill(Qt::white);
    QPainter painterPix(&pix);
    if (element == NULL) return pix;
    painterPix.drawLine(0, UNIT_HEIGH*2/3, UNIT_WIDTH, UNIT_HEIGH*2/3);
    return pix;

}


QPixmap GraphX::DrawPixMap()
{
    QPixmap pix(UNIT_WIDTH, UNIT_HEIGH);
    pix.fill(Qt::white);
    QPainter painterPix(&pix);
    if (element == NULL) return pix;
    painterPix.drawLine(0, UNIT_HEIGH*2/3, UNIT_WIDTH, UNIT_HEIGH*2/3);
    return pix;
}
