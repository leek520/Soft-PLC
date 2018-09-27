#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QTableWidget>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QStyledItemDelegate>
#include <QScrollBar>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include "common.h"
#include "graphelement.h"


class GraphItemDelegate : public QStyledItemDelegate
{
public:
    GraphItemDelegate(QObject* parent = 0):QStyledItemDelegate(parent){ }
    //在委托类的paint中画图
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        if(index.data(Qt::DisplayRole).canConvert<QPixmap>())
        {
            QPixmap pix = index.data(Qt::DisplayRole).value<QPixmap>();
            painter->drawPixmap(option.rect,pix);
        }

        QStyledItemDelegate::paint(painter,option,index);
    }
    //选中时，展示该图



};
class GraphTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit GraphTable(QWidget *parent = 0);
    virtual ~ GraphTable(){}
    void InitTable();


    void InsertGraphElement(Element *emt);
    void InsertSplitLine(int row);
private:
    void SetItemPixmap(int row, int col, QPixmap pixmap);

};



class GraphWindow : public QWidget
{
    Q_OBJECT
public:
    explicit GraphWindow(QWidget *parent = 0);
    virtual ~ GraphWindow(){}
signals:

public slots:

private:
    GraphTable *m_graphTable;
};

#endif // GRAPHWINDOW_H
