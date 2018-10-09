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
}

void GraphTable::InitTable()
{
    InitParament();
    setRowCount(INIT_ROW);
    setColumnCount(MAX_COL+1);

    for (int i=0;i<rowCount();i++){
        InsertSplitLine(i);
    }
    setCurrentCell(0,1);
}

void GraphTable::InitParament()
{
    m_recordBorad.curStep = 0;
    m_recordBorad.record.clear();
    m_recordBorad.type.clear();

    m_ClipBorad.type = CopyPaste;
}

void GraphTable::InsertRecordOpt(Element emt, OptType type, bool *isNew)
{
    //检测是否覆盖
    while (m_recordBorad.curStep > m_recordBorad.record.count()){
        m_recordBorad.record.removeLast();
        m_recordBorad.type.removeLast();
    }
    //记录
    if (*isNew){
        m_recordBorad.curStep++;
        QList<Element> newOpt;
        newOpt.append(emt);
        m_recordBorad.record.append(newOpt);
        m_recordBorad.type.append(type);
    }else{
        m_recordBorad.record.last().append(emt);
    }

    *isNew = false;
}

void GraphTable::InsertGraph(Element emt)
{
    GraphFB *graph = GetGraph(emt.row, emt.col);
    graph->emt = emt;
    DrawGraph(graph);
}
void GraphTable::InsertSplitLine(int row)
{
    GraphFB *graph = GetGraph(row, 0);
    graph->emt.graphType = NumLine;
    DrawGraph(graph);
}
GraphFB *GraphTable::GetGraph(int row, int col)
{
    InsertNewRow(row, col);

    GraphFB *graph = NULL;
    int idx = CalIdx(row, col);
    if (col == 0){
        graph = new GraphFB(row, col);
    }else{
        if (idx < m_graphList.count()){
            graph = m_graphList[idx];
        }else{
            graph = new GraphFB(row, col);
            RecordGraph(graph);
        }
    }
    return graph;
}

void GraphTable::DrawGraph(GraphFB *graph)
{
    graph->reDraw();

    int row = graph->emt.row;
    int col = graph->emt.col;

    QString mark = graph->emt.mark;

    QTableWidgetItem *item;

    item = this->item(row, col);
    if (item == NULL){
        item = new QTableWidgetItem();
        setItem(row, col, item);
    }
    item->setData(Qt::DisplayRole,
                  QVariant::fromValue<QPixmap>(graph->pixMap));
    item->setToolTip(mark);
}

void GraphTable::SetCurrentGraph(int row, int col)
{
    if (col == MAX_COL){
        setCurrentCell(row+1, 1);
    }else{
        setCurrentCell(row, col+1);
    }
}

void GraphTable::InsertNewRow(int row, int col)
{
    if (col == 0) return;
    //多两个空行
    if (row >= INIT_ROW-2){
        this->insertRow(row);
        InsertSplitLine(row);
    }

}

void GraphTable::RecordGraph(GraphFB *graph)
{
    int row = graph->emt.row;
    int col = graph->emt.col;
    if (col == 0) return;
    //动态申请二维数组内存
    int emtCnt = m_graphList.count();
    int emtCur = CalIdx(row, col);;
    for(int i=emtCnt;i<emtCur;i++){
        Element emt={i/MAX_COL,i%MAX_COL+1,1,1,0,0,0,false,false,"",""};
        GraphFB *gfb = new GraphFB(emt);
        m_graphList.append(gfb);
    }
    m_graphList.append(graph);
}

void GraphTable::RemoveGraph(int row, int col)
{
    if (col == 0) return;
    int idx = CalIdx(row, col);
    if (idx >= m_graphList.count()) return;

    GraphFB *graph = m_graphList[idx];
    graph->emt.graphType = NoneGraph;
    if (graph->emt.dnFlag){
        graph->emt.dnFlag = false;
        m_graphList[idx+MAX_COL]->emt.upFlag = false;
        DrawGraph(m_graphList[idx+MAX_COL]);
    }
    DrawGraph(graph);
    //如果是最后一个，则直接删除
    if (idx == m_graphList.count()-1){
        m_graphList.removeAt(idx);
    }
}


void GraphTable::slt_inputPara(QString name, int index, QString mark, int type)
{
    int i = 0;
    int curRow = currentRow();
    int curCol = currentColumn();
    GraphFB *graph = NULL;

    switch (type) {
    //如果要画竖直线，则分两步，先画下一行
    case verticalLine:
        if (curCol < 2) return;
        graph = GetGraph(curRow, curCol);
        graph->emt.dnFlag = true;
        DrawGraph(graph);
        graph = GetGraph(curRow+1, curCol);
        graph->emt.upFlag = true;
        DrawGraph(graph);
        SetCurrentGraph(curRow, curCol-1);
        break;
    case HorizontalLine:
        graph = GetGraph(curRow, curCol);
        graph->emt.graphType =  type;
        DrawGraph(graph);
        SetCurrentGraph(curRow, curCol);
        break;
    case InputOpen:
    case InputClose:
        graph = GetGraph(curRow, curCol);
        graph->emt.graphType =  type;
        graph->emt.name =  name;
        graph->emt.index =  index;
        graph->emt.mark =  mark;
        DrawGraph(graph);
        SetCurrentGraph(curRow, curCol);
        break;
    case OutputNode:
        for(i=curCol;i<MAX_COL;i++){
            graph = GetGraph(curRow, i);
            graph->emt.graphType =  HorizontalLine;
            DrawGraph(graph);
        }
        graph = GetGraph(curRow, i);
        graph->emt.graphType =  type;
        graph->emt.name =  name;
        graph->emt.index =  index;
        graph->emt.mark =  mark;
        DrawGraph(graph);
        SetCurrentGraph(curRow, i);
        break;
    default:
        break;
    }

}


void GraphTable::redo()
{
    int i = 0;
    QList<Element> emtList;
    if (m_recordBorad.curStep == m_recordBorad.record.count()) return;
    int step = m_recordBorad.curStep;
    switch (m_recordBorad.type[step]) {
    case Insert:
        emtList =  m_recordBorad.record[step];
        for (i=0;i<emtList.count();i++){
           //InsertGraphElement(emtList[i]);
        }

        break;
    default:
        break;
    }
    m_recordBorad.curStep++;
}

void GraphTable::undo()
{
    int i = 0;
    int row, col;
    QList<Element> emtList;
    if (m_recordBorad.curStep == 0) return;
    int step = m_recordBorad.curStep-1;
    switch (m_recordBorad.type[step]) {
    case Insert:
        emtList =  m_recordBorad.record[step];
        for (i=0;i<emtList.count();i++){
           row = emtList[i].row;
           col = emtList[i].col;
           //RemoveGraphElement(row, col);
        }

        break;
    default:
        break;
    }
    m_recordBorad.curStep--;
}

void GraphTable::copy()
{
    QList<QTableWidgetSelectionRange> selectRange = this->selectedRanges();
    if (selectRange.count() == 1){
        m_ClipBorad.range = selectRange[0];
        m_ClipBorad.type = CopyPaste;
    }
}

void GraphTable::paste()
{
    int curRow = currentRow();
    int curCol = currentColumn();
    int row = curRow;
    int col = curCol;
    int i, j;
    for (i=m_ClipBorad.range.topRow();i<=m_ClipBorad.range.bottomRow();i++){
        for(j=m_ClipBorad.range.leftColumn();j<=m_ClipBorad.range.rightColumn();j++){
            int idx = CalIdx(i, j);
            if (idx >= m_graphList.count()){
                qDebug()<<"Paste blank";
                break;
            }
            row = curRow + i - m_ClipBorad.range.topRow();
            col = curCol + j - m_ClipBorad.range.leftColumn();
            if (col >= MAX_COL) break;
            qDebug()<<QString("Paste (%1,%2) to (%3,%4)").arg(i).arg(j).arg(row).arg(col);

            GraphFB *graph = GetGraph(row, col);
            graph->emt = m_graphList[idx]->emt;
            graph->emt.row = row;
            graph->emt.col = col;
            graph->emt.upFlag = false;  //upflag不复制
            DrawGraph(graph);

            if (graph->emt.dnFlag){
                GraphFB *graph = GetGraph(row+1, col);
                graph->emt.upFlag = true;
                DrawGraph(graph);
            }

            //如果是剪切，则删除原来位置内容
            if (m_ClipBorad.type == CutPaste){
                RemoveGraph(i, j);
            }
        }
    }

//    //重新设置当前选中区域
//    setCurrentCell(row, col);
//    if (col == columnCount()-1){
//        setRangeSelected(QTableWidgetSelectionRange(row+1, 1, row+2, 2), false);
//    }else{
//        setRangeSelected(QTableWidgetSelectionRange(row, col, row+1, col+1), false);
//    }
//    //设置粘贴后的区域全部选中
//    setRangeSelected(QTableWidgetSelectionRange(curRow, curCol, row, col), true);

    if (m_ClipBorad.type == CutPaste){
        m_ClipBorad.type = CopyPaste;
    }
}

void GraphTable::cut()
{
    copy();
    m_ClipBorad.type = CutPaste;
}
void GraphTable::remove()
{

    int row = currentRow();
    int col = currentColumn();
    RemoveGraph(row, col);
}

//此处传入的row和col不是窗口中的实际坐标，而是列-1。
int GraphTable::DealNode(int row, int col)
{
    int idx = row * MAX_COL + col;
    if (idx >= m_graphList.count()) return -1;
    if ((row >= MAX_ROW) | (row < 0)) return -1;
    if ((col >= MAX_COL) | (col < 0)) return -1;


    //是否为结尾
    if (m_graphList[idx]->emt.graphType == 0){
        DealNode(row-1, buildPos[row-1]);
        return 1;
    }

    //是否要转上一行：条件=upflag和已经处理完
    if (m_graphList[idx]->emt.upFlag){
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
                    .arg(m_graphList[idx]->emt.name)
                    .arg(m_graphList[idx]->emt.index);
    qDebug()<<text;

    emit sig_InsertBottomRowText(text + "\n");
    qApp->processEvents();


    if  (col == MAX_COL-1){//如果处理到当前行的最后一列，则直接转下一列
        DealNode(row+1, buildPos[row+1]);
        return 1;
    }

    if (idx+1 >= m_graphList.count()) return 1;

    if (m_graphList[idx+1]->emt.dnFlag){
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


void GraphWindow::BuildGraph()
{
    //打印
    m_graphTable->BuildGraph();
}
void GraphWindow::OpenGraph(QString name)
{
    QFile fileRead(name);
    fileRead.open(QIODevice::ReadOnly);
    QDataStream  readDataStream(&fileRead);
    Element pEmt;
    m_graphTable->m_graphList.clear();
    m_graphTable->clear();
    m_graphTable->InitTable();
    while(!fileRead.atEnd()){
        readDataStream  >> pEmt.row
                        >> pEmt.col
                        >> pEmt.width
                        >> pEmt.height
                        >> pEmt.graphType
                        >> pEmt.funInsType
                        >> pEmt.index
                        >> pEmt.upFlag
                        >> pEmt.dnFlag
                        >> pEmt.name
                        >> pEmt.mark;
        m_graphTable->InsertGraph(pEmt);

    }
    fileRead.close();

    m_graphTable->InitParament();
}
void GraphWindow::SaveGraph(QString name)
{
    QFile fileWrite(name);
    fileWrite.open(QIODevice::WriteOnly);
    QDataStream  writeDataStream(&fileWrite);
    Element pEmt;
    for (int i=0;i<m_graphTable->m_graphList.count();i++){
        pEmt =  m_graphTable->m_graphList[i]->emt;
        writeDataStream << pEmt.row
                        << pEmt.col
                        << pEmt.width
                        << pEmt.height
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







