#include "graphwindow.h"


GraphTable::GraphTable(QWidget *parent) :
    QTableWidget(parent)
{
    horizontalHeader()->hide();
    horizontalScrollBar()->hide();
    verticalHeader()->hide();
    verticalScrollBar()->hide();


    verticalHeader()->setDefaultSectionSize(UNIT_HEIGH);
    horizontalHeader()->setDefaultSectionSize(UNIT_WIDTH);
    setShowGrid(false);//隐藏表格线
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置多选（可以Ctral+A全选Ctral+Shift多选）获取所选行号：
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setStyleSheet("QTableWidget{background-color: white}"
                  /*"QTableWidget::item:selected {background-color: white}"*/);

    setItemDelegate(new GraphItemDelegate());
}

void GraphTable::InitTable()
{
    setRowCount(10);
    setColumnCount(10);

    setColumnWidth(1, LINE_WIDTH);

    for (int i=0;i<rowCount();i++){
        InsertSplitLine(i);
    }

}


void GraphTable::SetItemPixmap(int row, int col, QPixmap pixmap)
{
    QTableWidgetItem *item = this->item(row, col);
    if (item == NULL){
        item = new QTableWidgetItem();
        setItem(row, col, item);
    }
    item->setData(Qt::DisplayRole,
                  QVariant::fromValue<QPixmap>(pixmap));
}

void GraphTable::InsertSplitLine(int row)
{
    Element emt;
    emt.col = 1;
    emt.row = row;
    emt.graphType = VerticalLine;

    InsertGraphElement(&emt);
}

void GraphTable::InsertGraphElement(Element *emt)
{
    GraphElement *graph = NULL;
    switch (emt->graphType) {
    case VerticalLine:
        graph = new GraphLine(emt);
        break;
    case HorizontalLine:
        graph = new GraphBlank(emt);
        break;
    case LogicX:
        graph = new GraphX(emt);
        break;
    default:
        graph = new GraphBlank(emt);
        break;
    }
    SetItemPixmap(emt->row, emt->col, graph->DrawPixMap());

}

GraphWindow::GraphWindow(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(1);
    m_graphTable = new GraphTable();

    layout->addWidget(m_graphTable);

    int width = this->width();
    int heigh = this->height();


    m_graphTable->InitTable();


    Element emt;
    emt.col = 2;
    emt.row = 2;
    emt.graphType = HorizontalLine;

    m_graphTable->InsertGraphElement(&emt);

}




