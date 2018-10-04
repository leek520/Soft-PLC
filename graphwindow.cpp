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
    setColumnCount(MAX_COL+1);


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

    QPixmap pix(UNIT_WIDTH, UNIT_HEIGH);
    pix.fill(Qt::white);
    oldPix = pix;

    item = this->item(row, col);
    if (item == NULL){
        item = new QTableWidgetItem();
        setItem(row, col, item);
    }else{
        if (type == verticalLine){
            oldPix = item->data(Qt::DisplayRole).value<QPixmap>();
        }
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

    //记录画的每一个图

    RecordGraph(graph);

}

void GraphTable::RecordGraph(GraphElement *graph)
{
    int row = graph->element->row;
    int col = graph->element->col;
    if (col==0) return;
    //动态申请二维数组内存
    int emtCnt = m_emtList.count();
    int emtCur = row * MAX_COL + (col-1);
    for(int i=emtCnt;i<=emtCur;i++){
        Element emt={0,0,0,0,0,false,false,"",""};
        m_emtList.append(emt);
    }
    //是实际的图形
    graph->element->dnFlag = (graph->element->dnFlag) | (m_emtList[emtCur].dnFlag);
    graph->element->upFlag = (graph->element->upFlag) | (m_emtList[emtCur].upFlag);
    if (graph->element->graphType == verticalLine){
        m_emtList[emtCur].dnFlag = graph->element->dnFlag;
        m_emtList[emtCur].upFlag = graph->element->upFlag;
        return;
    }
    //记录当前单元格
    memcpy(&m_emtList[emtCur], graph->element, sizeof(Element));


}

void GraphTable::SelectionChanged()
{

}

void GraphTable::InsertSplitLine(int row)
{
    Element emt={0,0,0,0,0,false,false,"",""};
    emt.col = 0;
    emt.row = row;
    emt.name = "";
    emt.graphType = NumLine;

    InsertGraphElement(&emt);
}
int GraphTable::DealNode(int row, int col)
{

    int idx = row * MAX_COL + col;
    if (idx >= m_emtList.count()) return -1;
    if ((row >= MAX_ROW) | (row < 0)) return -1;
    if ((col >= MAX_COL) | (col < 0)) return -1;


    //是否为结尾
    if ((m_emtList[idx].graphType == verticalLine) ||
        (m_emtList[idx].graphType == 0)){
        DealNode(row-1, buildPos[row-1]);
        return 1;
    }

    //是否要转上一行：条件=upflag和已经处理完
    if (m_emtList[idx].upFlag){
        if (buildPos[row-1] <= col){
            DealNode(row-1, buildPos[row-1]);
//            if (buildPos[row-1] > col+1){
//                return 1;
//            }
        }
    }


    buildPos[row] += 1;
    m_trail.append(QPoint(row, col));
    qDebug()<<QString("Pos:(%1,%2), %3%4").arg(row).arg(col)
              .arg(m_emtList[idx].name)
              .arg(m_emtList[idx].index);

    if (col == MAX_COL-1) return 1;
    if (m_emtList[idx+1].dnFlag){
        buildPreRow = row;
        DealNode(row+1, buildPos[row+1]);
    }else{
        DealNode(row, buildPos[row]);

    }
}
void GraphTable::BuildGraph()
{
    int i = 0;
    int j = 0;
    buildPreRow = 0;
    memset(buildPos, 0, MAX_ROW * sizeof(int));
    while(i < MAX_ROW)
    {
        int ret = DealNode(i, buildPos[i]);
        if (ret == -1){
            break;
        }
        for(j=0;j<MAX_ROW;j++){
            if (buildPos[j] == 0){
                i = j;
                break;
            }
        }
        if (i == buildPreRow){
            break;
        }
    }

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
        Element emt2 = {0,0,0,0,0,false,false,"",""};
        emt2.row = emt->row + 1;
        emt2.col = emt->col;
        emt2.upFlag = true;
        emt2.graphType = verticalLine;
        emt2.mark = "";

        graph = new GraphVLine(&emt2,1);
        SetItemPixmap(graph);
        emt->dnFlag = true;
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
        Element emt2={0,0,0,0,0,false,false,"",""};
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
    setWindowTitle("梯形图模式");
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(1);
    m_graphTable = new GraphTable();
    layout->addWidget(m_graphTable);

    m_graphTable->InitTable();

}


void GraphWindow::slt_inputPara(QString name, int index, QString mark, int type)
{
    Element emt={0,0,0,0,0,false,false,"",""};
    emt.col = m_graphTable->currentColumn();
    emt.row = m_graphTable->currentRow();
    emt.name = name;
    emt.index = index;
    emt.graphType = type;
    emt.mark = mark;
    m_graphTable->InsertGraphElement(&emt);

}

void GraphWindow::BuildGraph()
{
    //打印
    for (int i=0;i<m_graphTable->m_emtList.count();i++){
        qDebug()<<m_graphTable->m_emtList[i].row
                <<m_graphTable->m_emtList[i].col
                <<m_graphTable->m_emtList[i].upFlag
                <<m_graphTable->m_emtList[i].dnFlag
                <<(GraphType)m_graphTable->m_emtList[i].graphType;
    }
    m_graphTable->BuildGraph();
}
void GraphWindow::OpenGraph(QString name)
{
    QFile fileRead(name);
    fileRead.open(QIODevice::ReadOnly);
    QDataStream  readDataStream(&fileRead);
    Element pEmt;
    m_graphTable->m_emtList.clear();
    while(!fileRead.atEnd()){
        readDataStream  >> pEmt.col
                        >> pEmt.row
                        >> pEmt.dnFlag
                        >> pEmt.upFlag
                        >> pEmt.funInsType
                        >> pEmt.graphType
                        >> pEmt.name
                        >> pEmt.index
                        >> pEmt.mark;
        m_graphTable->m_emtList.append(pEmt);
        m_graphTable->InsertGraphElement(&pEmt);

    }
    fileRead.close();
}
void GraphWindow::SaveGraph(QString name)
{
    QFile fileWrite(name);
    fileWrite.open(QIODevice::WriteOnly);
    QDataStream  writeDataStream(&fileWrite);
    Element pEmt;
    for (int i=0;i<m_graphTable->m_emtList.count();i++){
        pEmt =  m_graphTable->m_emtList[i];
        writeDataStream <<pEmt.col
                        <<pEmt.row
                        <<pEmt.dnFlag
                        <<pEmt.upFlag
                        <<pEmt.funInsType
                        <<pEmt.graphType
                        <<pEmt.name
                        <<pEmt.index
                        <<pEmt.mark;
    }
    fileWrite.close();
}







