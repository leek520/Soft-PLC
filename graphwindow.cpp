#include "graphwindow.h"
void GraphItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.data(Qt::DisplayRole).canConvert<QPixmap>())
    {
        QPixmap pix = index.data(Qt::DisplayRole).value<QPixmap>();
        painter->drawPixmap(option.rect,pix);
    }
    QStyledItemDelegate::paint(painter, option, index);
//    QStyleOptionViewItem viewOption(option);
//    initStyleOption(&viewOption, index);
//    if (option.state.testFlag(QStyle::State_Selected))
//    {
//        viewOption.state = viewOption.state ^ QStyle::State_Selected;
//    }
//    QStyledItemDelegate::paint(painter, viewOption, index);

}

bool GraphItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(index.data(Qt::DisplayRole).canConvert<QPixmap>())
    {
        QPixmap pix = index.data(Qt::DisplayRole).value<QPixmap>();
    }


   //QStyledItemDelegate::editorEvent(event, model, option, index);
}
GraphTable::GraphTable(QWidget *parent) :
    QTableWidget(parent)
{
    horizontalHeader()->hide();
    verticalHeader()->hide();

    verticalHeader()->setDefaultSectionSize(UNIT_HEIGH);
    horizontalHeader()->setDefaultSectionSize(UNIT_WIDTH);
    setShowGrid(false);//隐藏表格线
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置多选（可以Ctral+A全选Ctral+Shift多选）获取所选行号：
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    setStyleSheet("QTableWidget {background-color: white;}"
//                  "QTableWidget::item {padding:0px 0px 0px 0px; margin:0px 0px 0px 0px;}"
                  "QTableWidget::item:selected {background-color: rgb(0,0,255,100);}");

    setItemDelegate(new GraphItemDelegate());

    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(SelectionChanged()));
}

void GraphTable::InitTable()
{
    setRowCount(10);
    setColumnCount(14);

    setColumnWidth(1, LINE_WIDTH);
    setSpan(0,13,10,14);

    QTableWidgetItem *item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);
    setItem(0,13,item);

    for (int i=0;i<rowCount();i++){
        InsertSplitLine(i);
    }
    setCurrentCell(0,2);
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

void GraphTable::SelectionChanged()
{

}

void GraphTable::InsertSplitLine(int row)
{
    Element emt;
    emt.col = 1;
    emt.row = row;
    emt.name = "";
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
    case InputOpen:
    case InputClose:
        if (emt->col < 2) return;
        graph = new GraphInput(emt);
        break;
    default:
        graph = new GraphBlank(emt);
        break;
    }
    SetItemPixmap(emt->row, emt->col, graph->DrawPixMap());

    if (emt->name != "")
        graph->DrawPixMap().save(QString("%1.png").arg(emt->name));
}

GraphWindow::GraphWindow(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(1);
    m_graphTable = new GraphTable();
    layout->addWidget(m_graphTable);

    m_graphTable->InitTable();

}


void GraphWindow::slt_inputPara(QString name, int index, QString mark, int type)
{
    Element emt;
    emt.col = m_graphTable->currentColumn();
    emt.row = m_graphTable->currentRow();
    emt.name = name;
    emt.index = index;
    emt.graphType = type + 10;

    m_graphTable->InsertGraphElement(&emt);
}







