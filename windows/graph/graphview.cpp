#include "graphview.h"

GraphView::GraphView(QWidget *parent)
    : QTableView(parent)
{
    this->setStyleSheet("QTableView{background-color: white;}"
                        "QTableView{selection-background-color: rgb(0,0,255,100);}");

    this->horizontalHeader()->setVisible(false);
    this->verticalHeader()->setVisible(false);


    this->setShowGrid(false);   //隐藏表格线

    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置多选（可以Ctral+A全选Ctral+Shift多选）获取所选行号：
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);

    this->horizontalHeader()->setDefaultSectionSize(UNIT_WIDTH);
    this->verticalHeader()->setDefaultSectionSize(UNIT_HEIGH);

    graph_model = new GraphModel();
    graph_model->setModalDatas(&grid_data_list);
    this->setModel(graph_model);

    graph_delegate = new GraphDelegate();
    this->setItemDelegate(graph_delegate);


    //connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClickItem(QModelIndex)));
}

GraphView::~GraphView()
{
    if(graph_delegate) {
        delete graph_delegate;
        graph_delegate = NULL;
    }

    if(graph_model) {
        delete graph_model;
        graph_model = NULL;
    }
    grid_data_list.clear();
}

void GraphView::initTable()
{
    grid_data_list.clear();

    for(int i=0;i<INIT_ROW;i++){
        insertRow(i);
    }
}

void GraphView::insertRow(int row)
{
    QList<Element> *row_emt = new QList<Element>();
    Element emt(row, 0, NumLine);
    row_emt->append(emt);
    for(int i=1;i<MAX_COL;i++){
        Element emti(row, i);
        row_emt->append(emti);
    }
    grid_data_list.insert(row, row_emt);
}

void GraphView::addRow(QList<Element> *rowList)
{
    grid_data_list.append(rowList);
    graph_model->refreshModel();
}
