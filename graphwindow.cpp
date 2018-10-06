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
    setRowCount(INIT_ROW);
    setColumnCount(MAX_COL+1);

    for (int i=0;i<rowCount();i++){
        InsertSplitLine(i);
    }
    setCurrentCell(0,1);
}

void GraphTable::InsertRowGraph(int row)
{
    //多两个空行
    if (row >= INIT_ROW-2){
        this->insertRow(row);
        InsertSplitLine(row);
    }

}


void GraphTable::SetItemPixmap(GraphElement *graph)
{
    int type = graph->element.graphType;
    int row = graph->element.row;
    int col = graph->element.col;

    QString mark = graph->element.mark;

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
        int oldType = m_emtList[row*MAX_COL+col-1].graphType;
        //如果画的是竖直线，则需要保留原有的图像
        if ((type == verticalLine1)
             || (type == verticalLine2)
             || (oldType== verticalLine1)
             || (oldType == verticalLine2)){
            oldPix = item->data(Qt::DisplayRole).value<QPixmap>();
        }else{
            QPixmap pix(UNIT_WIDTH, UNIT_HEIGH);
            pix.fill(Qt::white);
            oldPix = pix;
        }
    }
    QPixmap pixmap = graph->DrawPixMap(oldPix);
    item->setData(Qt::DisplayRole,
                  QVariant::fromValue<QPixmap>(pixmap));
    item->setToolTip(mark);

    //画完图自动转下一个单元格
    if (col == columnCount()-1){
       setCurrentCell(row+1, 1);
    }else{
        if ((type == verticalLine1)
           || (type == verticalLine2)){

        }else{
            setCurrentCell(row, col+1);
        }
    }
}

void GraphTable::RecordGraph(GraphElement *graph)
{
    int row = graph->element.row;
    int col = graph->element.col;
    if (col==0) return;
    //动态申请二维数组内存
    int emtCnt = m_emtList.count();
    int emtCur = row * MAX_COL + (col-1);
    for(int i=emtCnt;i<=emtCur;i++){
        Element emt={i/MAX_COL,i%MAX_COL+1,0,0,0,false,false,"",""};
        m_emtList.append(emt);
    }
    //是实际的图形
    graph->element.dnFlag = graph->element.dnFlag | m_emtList[emtCur].dnFlag;
    graph->element.upFlag = graph->element.upFlag | m_emtList[emtCur].upFlag;
    int type = graph->element.graphType;
    if (m_emtList[emtCur].graphType != 0){
        if ((type == verticalLine1)
            || (type == verticalLine2)){
            m_emtList[emtCur].dnFlag = graph->element.dnFlag;
            m_emtList[emtCur].upFlag = graph->element.upFlag;
            return;
        }
    }

    //记录当前单元格
    memcpy(&m_emtList[emtCur], &(graph->element), sizeof(Element));


}

void GraphTable::SelectionChanged()
{

}

void GraphTable::redo()
{

}

void GraphTable::undo()
{

}

void GraphTable::copy()
{
    QList<QTableWidgetSelectionRange> selectRange = this->selectedRanges();
    if (selectRange.count() == 1){
        m_ClipBorad = selectRange[0];
    }
    m_ClipType = 0;
}

void GraphTable::paste()
{
    int curRow = currentRow();
    int curCol = currentColumn();
    int row = curRow;
    int col = curCol;
    int i, j;
    for (i=m_ClipBorad.topRow();i<=m_ClipBorad.bottomRow();i++){
        for(j=m_ClipBorad.leftColumn();j<=m_ClipBorad.rightColumn();j++){
            int idx = i*MAX_COL+j-1;
            if (idx >= m_emtList.count()){
                qDebug()<<"Paste blank";
                break;
            }
            row = curRow + i - m_ClipBorad.topRow();
            col = curCol + j - m_ClipBorad.leftColumn();
            qDebug()<<QString("Paste (%1,%2) to (%3,%4)").arg(i).arg(j).arg(row).arg(col);
            m_emtList[idx].row = row;
            m_emtList[idx].col = col;
            InsertGraphElement(m_emtList[idx]);

            //如果是剪切，则删除原来位置内容
            if (m_ClipType == 1){
                Element emt={i,j,0,0,0,false,false,"",""};
                InsertGraphElement(emt);
                if (j == columnCount()-1){
                    setRangeSelected(QTableWidgetSelectionRange(i+1, 1, i+2, 2), false);
                }else{
                    setRangeSelected(QTableWidgetSelectionRange(i, j, i+1, j+1), false);
                }
            }
        }
    }

    //重新设置当前选中区域
    setCurrentCell(row, col);
    if (col == columnCount()-1){
        setRangeSelected(QTableWidgetSelectionRange(row+1, 1, row+2, 2), false);
    }else{
        setRangeSelected(QTableWidgetSelectionRange(row, col, row+1, col+1), false);
    }
    //设置粘贴后的区域全部选中
    setRangeSelected(QTableWidgetSelectionRange(curRow, curCol, row, col), true);

    if (m_ClipType == 1){
        m_ClipType = 0;
    }
}

void GraphTable::cut()
{
    copy();
    m_ClipType = 1;
}
void GraphTable::remove()
{
    int row = currentRow();
    int col = currentColumn();
    RemoveGraphElement(row, col);
}
void GraphTable::InsertSplitLine(int row)
{
    Element emt={0,0,0,0,0,false,false,"",""};
    emt.col = 0;
    emt.row = row;
    emt.name = "";
    emt.graphType = NumLine;

    InsertGraphElement(emt);
}

//此处传入的row和col不是窗口中的实际坐标，而是列-1。
int GraphTable::DealNode(int row, int col)
{
    int idx = row * MAX_COL + col;
    if (idx >= m_emtList.count()) return -1;
    if ((row >= MAX_ROW) | (row < 0)) return -1;
    if ((col >= MAX_COL) | (col < 0)) return -1;


    //是否为结尾
    if ((m_emtList[idx].graphType == 0)
        || (m_emtList[idx].graphType == verticalLine1)
        || (m_emtList[idx].graphType == verticalLine2)){
        DealNode(row-1, buildPos[row-1]);
        return 1;
    }

    //是否要转上一行：条件=upflag和已经处理完
    if (m_emtList[idx].upFlag){
        if (buildPos[row-1] <= col){
            DealNode(row-1, buildPos[row-1]);
            if (buildPos[row-1] > col){
                return 1;
            }
        }
    }


    buildPos[row] += 1;
    m_buildTrail.append(QPoint(row, col));
    QString text = QString("Pos:(%1,%2), %3%4")
                    .arg(row).arg(col)
                    .arg(m_emtList[idx].name)
                    .arg(m_emtList[idx].index);
    qDebug()<<text;

    emit sig_InsertBottomRowText(text + "\n");
    qApp->processEvents();


    if  (col == MAX_COL-1){//如果处理到当前行的最后一列，则直接转下一列
        DealNode(row+1, buildPos[row+1]);
        return 1;
    }

    if (idx+1 >= m_emtList.count()) return 1;

    if (m_emtList[idx+1].dnFlag){
        buildPreRow = row+1;
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
    }

}
void GraphTable::keyPressEvent(QKeyEvent *event)
{
    //如果按下delete
    if(event->key() == Qt::Key_Delete){

        return;
    }

}
void GraphTable::InsertGraphElement(Element emt)
{
    //1.创建对象
    GraphElement *graph = new GraphElement(emt);
    //2.画图
    SetItemPixmap(graph);
    //3.记录画的每一个图
    RecordGraph(graph);

}

void GraphTable::RemoveGraphElement(int row, int col)
{
    int idx = row*MAX_COL+col-1;
    if (idx >= m_emtList.count()) return;

    if (m_emtList[idx].dnFlag){
        //删除下一行的半个竖直线
        m_emtList[idx+MAX_COL].upFlag = false;
        if (m_emtList[idx+MAX_COL].graphType == verticalLine2){
            m_emtList[idx+MAX_COL].graphType = 0;
        }
        InsertGraphElement(m_emtList[idx+MAX_COL]);

    }

    m_emtList[idx].graphType = 0;
    m_emtList[idx].funInsType = 0;
    m_emtList[idx].index = 0;
    m_emtList[idx].name = "";
    m_emtList[idx].mark = "";
    m_emtList[idx].dnFlag = false;
    InsertGraphElement(m_emtList[idx]);
    setCurrentCell(row, col);
}

GraphWindow::GraphWindow(QWidget *parent) :
    QWidget(parent)
{
    setMouseTracking(true);   //开启鼠标跟踪功能
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
    int curRow = m_graphTable->currentRow();
    int curCol = m_graphTable->currentColumn();

    m_graphTable->InsertRowGraph(curRow);
    switch (type) {
    //如果要画竖直线，则分两步，先画下一行
    case verticalLine1:
        if (curCol < 2) return;
        emt.upFlag = true;
        emt.dnFlag = false;
        emt.col = curCol;
        emt.row = curRow + 1;
        emt.graphType =  verticalLine2;
        m_graphTable->InsertGraphElement(emt);
        emt.upFlag = false;
        emt.dnFlag = true;
        break;
    case OutputNode:
    {
        for(int i=curCol;i<MAX_COL;i++){
            emt={0,0,0,0,0,false,false,"",""};
            emt.col = i;
            emt.row = curRow;
            emt.graphType =  HorizontalLine;
            m_graphTable->InsertGraphElement(emt);
        }
        curRow = m_graphTable->currentRow();
        curCol = m_graphTable->currentColumn();
        break;
    }
    default:
        break;
    }
    emt.col = curCol;
    emt.row = curRow;
    emt.name = name;
    emt.index = index;
    emt.graphType = type;
    emt.mark = mark;
    m_graphTable->InsertGraphElement(emt);


}

void GraphWindow::BuildGraph()
{
    //打印
    for (int i=0;i<m_graphTable->m_emtList.count();i++){
        qDebug()<<m_graphTable->m_emtList[i].row
                <<m_graphTable->m_emtList[i].col
                <<m_graphTable->m_emtList[i].upFlag
                <<m_graphTable->m_emtList[i].dnFlag
                <<m_graphTable->m_emtList[i].graphType;
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
    m_graphTable->clear();
    m_graphTable->InitTable();
    while(!fileRead.atEnd()){
        readDataStream  >> pEmt.row
                        >> pEmt.col
                        >> pEmt.graphType
                        >> pEmt.funInsType
                        >> pEmt.index
                        >> pEmt.upFlag
                        >> pEmt.dnFlag
                        >> pEmt.name
                        >> pEmt.mark;
        m_graphTable->m_emtList.append(pEmt);
        m_graphTable->InsertGraphElement(pEmt);

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
        writeDataStream << pEmt.row
                        << pEmt.col
                        << pEmt.graphType
                        << pEmt.funInsType
                        << pEmt.index
                        << pEmt.upFlag
                        << pEmt.dnFlag
                        << pEmt.name
                        << pEmt.mark;
    }
    fileWrite.close();
}







