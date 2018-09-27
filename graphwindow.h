#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QMouseEvent>
#include "common.h"

class MyStandardItemModel:public QStandardItemModel
{
public:
    MyStandardItemModel(QObject * parent=0)
        :QStandardItemModel(parent){}
    virtual ~ MyStandardItemModel(){}
    QVariant data(const QModelIndex & index, int role=Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;
};
class GraphTable : public QTableView
{
    Q_OBJECT
public:
    explicit GraphTable(QWidget *parent = 0);
    virtual ~ GraphTable(){}
    void SetLeftLine();
protected:
    void mouseMoveEvent(QMouseEvent * event);

private:
 //   MyItemDelegate * delegate;
    MyStandardItemModel * model;

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
