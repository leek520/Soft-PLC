#include "graphwindow.h"

QVariant MyStandardItemModel::data(const QModelIndex & index, int role) const
{
    int column=index.column();

    if(role==Qt::DisplayRole && column!=0)
        return column+1;
    if(role==Qt::ToolTipRole && column==0)
        return tr("love");
    return QStandardItemModel::data(index,role);
}

QVariant MyStandardItemModel::headerData(int section,
                                         Qt::Orientation orientation,
                                         int role) const
{
    if(section==0 && orientation==Qt::Horizontal){
        if(role==Qt::DecorationRole)
            return QIcon(":/favourite.png");
        if(role==Qt::DisplayRole)
            return "";
        if(role==Qt::ToolTipRole)
            return tr("love");
    }
    return QStandardItemModel::headerData(section,orientation,role);
}

GraphTable::GraphTable(QWidget *parent) :
    QTableView(parent)
{
    model = new MyStandardItemModel;
    model->setRowCount(6);
    model->setColumnCount(8);
    this->setModel(model);


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



}

void GraphTable::SetLeftLine()
{
    int row = model->rowCount();
    int col = model->columnCount();

    this->setSpan(0, 1, row, 1);
    setColumnWidth(1, 10);


}

void GraphTable::mouseMoveEvent(QMouseEvent *event)
{
    int column = this->columnAt(event->x());
    int row = this->rowAt(event->y());
    if(column == 0 && row != -1){
        this->setCursor(Qt::PointingHandCursor);
    }
    else{
        this->setCursor(Qt::ArrowCursor);
    }
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


    m_graphTable->SetLeftLine();
}




