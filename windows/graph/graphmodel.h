#ifndef GRAPHMODEL_H
#define GRAPHMODEL_H

#include "graphdelegate.h"

class GraphModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    GraphModel(QObject *parent = 0);
    ~GraphModel(void);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void setModalDatas(QList< QList<Element>* > *rowlist);
    void refreshModel();

protected:

signals:

    void updateCount(int count);

private:
    QList< QList<Element>* > *arr_row_list;

};
#endif // GRAPHMODEL_H
