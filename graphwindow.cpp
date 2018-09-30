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


    for (int i=0;i<rowCount();i++){
        InsertSplitLine(i);
    }
    setCurrentCell(0,1);
}


void GraphTable::SetItemPixmap(GraphElement *graph)
{
    int type = graph->element->graphType;
    int row = graph->element->row;
    int col = graph->element->col;
    QString mark = graph->element->mark;
    QTableWidgetItem *item;
    QPixmap oldPix;

    item = this->item(row, col);
    if (item == NULL){
        item = new QTableWidgetItem();
        setItem(row, col, item);
        QPixmap pix(UNIT_WIDTH, UNIT_HEIGH);
        pix.fill(Qt::white);
        oldPix = pix;
    }else{
        oldPix = item->data(Qt::DisplayRole).value<QPixmap>();
    }
    QPixmap pixmap = graph->DrawPixMap(oldPix);
    item->setData(Qt::DisplayRole,
                  QVariant::fromValue<QPixmap>(pixmap));
    item->setToolTip(mark);


    //画完图自动转下一个单元格
    if (col == columnCount()-1){
       setCurrentCell(row+1, 1);
    }else if (type != verticalLine){
       setCurrentCell(row, col+1);
    }
}

void GraphTable::SelectionChanged()
{

}

void GraphTable::InsertSplitLine(int row)
{
    Element emt;
    emt.col = 0;
    emt.row = row;
    emt.name = "";
    emt.graphType = NumLine;

    InsertGraphElement(&emt);
}

void GraphTable::InsertGraphElement(Element *emt)
{
    GraphElement *graph = NULL;
    switch (emt->graphType) {
    case NumLine:
        if (emt->col != 0) return;
        graph = new GraphNumLine(emt);
        break;
    case HorizontalLine:
        graph = new GraphHLine(emt);
        break;
    case verticalLine:
    {
        Element emt2;
        emt2.row = emt->row + 1;
        emt2.col = emt->col;
        emt2.graphType = verticalLine;
        emt2.mark = "";

        graph = new GraphVLine(&emt2,1);
        SetItemPixmap(graph);
        graph = new GraphVLine(emt);
        break;
    }
    case InputOpen:
    case InputClose:
        if (emt->col == 0) return;
        graph = new GraphInput(emt);
        break;
    case ReverseLogic:
        if (emt->col == 0) return;
        graph = new GraphRLogic(emt);
        break;
    case StepNode:
        if (emt->col == 0) return;
        graph = new GraphRLogic(emt);
        break;
    case OutputNode:
    {
        if (emt->col == 0) return;
        Element emt2;
        for(int j=emt->col;j<columnCount()-1;j++){
            emt2.row = emt->row;
            emt2.col = j;
            emt2.graphType = HorizontalLine;
            emt2.mark = "";
            graph = new GraphHLine(&emt2);
            SetItemPixmap(graph);
        }
        emt->col = columnCount()-1;
        graph = new GraphOutput(emt);
        break;
    }
    default:
        graph = new GraphHLine(emt);
        break;
    }
    SetItemPixmap(graph);


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
    emt.graphType = type;
    emt.mark = mark;
    m_graphTable->InsertGraphElement(&emt);

}







