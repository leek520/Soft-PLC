#include "graphdelegate.h"


GraphDelegate::GraphDelegate(QObject *parent)
    : QItemDelegate(parent)
{

}

void GraphDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int row = index.row();
    int col = index.column();

    QVariant dataE = index.model()->data(index,Qt::UserRole);

    Element emt = dataE.value<Element>();

    this->draw(painter, option, &emt);

    QItemDelegate::paint (painter, option, index);
}

void GraphDelegate::draw(QPainter *painter, const QStyleOptionViewItem &option, Element *emt) const
{
    int x = option.rect.x();
    int y = option.rect.y();
    int width = option.rect.width();
    int height = option.rect.height();
    //画功能图
    int fw = width/UNIT_W_FACTOR;
    int fh = height/UNIT_H_FACTOR;

    QPen pen;
    painter->setPen(pen);

    QPointF pLT(x, y), pLB(x, y+height), pRT(x+width-1, y), pRB(x+width-1, y+height);
    QPointF pL(x, y), pR(x, y+height);
    //根据类别画相应的图形
    QPointF p1(0, height-fh);
    QPointF p2(width, height-fh);


    //填充背景
    painter->fillRect(option.rect, QBrush(QColor(emt->backRGB)));

    switch (emt->type) {
    case NumLine:
        pen.setColor(QColor(emt->entRGB));
        painter->drawLine(pRT, pRB);
        break;
    case HorizontalLine:
        painter.drawLine(pL, pR);
        break;
    }
//    int width = g_unitWidth * emt.width;
//    int height = g_unitHeight * emt.height;
//    if (emt->col == 0){
//        QRectF rect;
//        rect.setTopLeft(QPointF(x,y));
//        rect.setBottomRight(QPointF(x + width / 2,y + height / 2));
//        QTextOption textOption;
//        textOption.setAlignment(Qt::AlignCenter);
//        painter->drawText(rect,emt.name,textOption);
//    }
}


