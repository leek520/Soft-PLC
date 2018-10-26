#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include "graphmodel.h"

class GraphView : public QTableView
{
    Q_OBJECT

public:

    GraphView(QWidget *parent=0);
    ~GraphView(void);

    void initTable();
    void insertRow(int row);
    void addRow(QList<Element>* rowList);


private:
    GraphModel *graph_model;
    QList< QList<Element>* > grid_data_list;
    GraphDelegate *graph_delegate;

};
#endif // TABLEVIEW_H
