#ifndef GRAPHDELEGATE_H
#define GRAPHDELEGATE_H

#include <QWidget>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QItemDelegate>
#include <QHeaderView>
#include <QTableView>
#include <QPainter>
#include <QDebug>

#include "common.h"

Q_DECLARE_METATYPE(Element)
class GraphDelegate : public QItemDelegate
{
    Q_OBJECT

public:

    GraphDelegate(QObject* parent = 0);
    virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

protected:

private:
    void draw(QPainter* painter, const QStyleOptionViewItem &option, Element *emt) const;
 };
#endif // GRAPHDELEGATE_H
