#include "graphmodel.h"

GraphModel::GraphModel(QObject *parent)
    : QAbstractTableModel(parent), arr_row_list(NULL)
{

}

GraphModel::~GraphModel()
{
    arr_row_list = NULL;
}

int GraphModel::rowCount(const QModelIndex &parent) const
{
    if(NULL == arr_row_list)
        return 0;
    else
        return arr_row_list->size();
}

int GraphModel::columnCount(const QModelIndex &parent) const
{
    if(NULL == arr_row_list)
        return 0;
    else if(arr_row_list->size() < 1)
        return 0;
    else
        return arr_row_list->at(0)->size();
}

QVariant GraphModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(NULL == arr_row_list)
        return QVariant();

    if(arr_row_list->size() < 1)
        return QVariant();

    if (role == Qt::TextAlignmentRole)
    {
        return int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    else if (role == Qt::UserRole)
    {
        if(index.row() >= arr_row_list->size())
            return QVariant();
        if(index.column() >= arr_row_list->at(0)->size())
            return QVariant();
        return QVariant::fromValue(arr_row_list->at(index.row())->at(index.column()));
    }
    return QVariant();
}

bool GraphModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
    {
        return false;
        qDebug()<<"index is valid";
    }
}
Qt::ItemFlags GraphModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flag = QAbstractItemModel::flags(index);

    flag|=Qt::ItemIsEditable;
    return flag;
}

void GraphModel::setModalDatas(QList<QList<Element> *> *rowlist)
{
    arr_row_list = rowlist;
}

void GraphModel::refreshModel()
{
    beginResetModel();
    endResetModel();
}

