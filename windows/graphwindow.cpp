﻿#include "graphwindow.h"
void GraphItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.data(Qt::DisplayRole).canConvert<QPixmap>())
    {
        QPixmap pix = index.data(Qt::DisplayRole).value<QPixmap>();
        painter->drawPixmap(option.rect,pix);
    }
    QStyledItemDelegate::paint(painter, option, index);

}

GraphTable::GraphTable(QWidget *parent) :
    QTableWidget(parent)
{
    horizontalHeader()->hide();
    verticalHeader()->hide();

    horizontalHeader()->setDefaultSectionSize(UNIT_WIDTH);
    verticalHeader()->setDefaultSectionSize(UNIT_HEIGH);

    setShowGrid(false);//隐藏表格线
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置多选（可以Ctral+A全选Ctral+Shift多选）获取所选行号：
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    setStyleSheet("QTableWidget {background-color: white;}"
//                  "QTableWidget::item {padding:0px 0px 0px 0px; margin:0px 0px 0px 0px;}"
                  "QTableWidget::item:selected {background-color: rgb(0,0,255,100);}");

    connect(this, SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
            this, SLOT(slt_itemDoubleClicked(QTableWidgetItem*)));
    setItemDelegate(new GraphItemDelegate());

    createActions();
}

void GraphTable::InitTable()
{
    GM->clearAllGraph();
    clear();
    InitParament();
    setRowCount(INIT_ROW);
    setColumnCount(MAX_COL+1);

    for (int i=0;i<INIT_ROW;i++){
        insertSplitLine(i);

    }
    setCurrentCell(0,1);
}

void GraphTable::InitParament()
{
    m_OperationBorad.curStep = 0;
    m_OperationBorad.record.clear();
    m_OperationBorad.type.clear();

    m_ClipBorad.type = RedoCopyPaste;

    memset(m_ladderBack, 0, sizeof(int) * MAX_ROW);

}
/******************************************************************************
* @brief: 记录用户操作，以便撤销和恢复功能使用
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphTable::RecordOperation(bool *isNew,
                                 GraphFB *graph,
                                 OptType type,
                                 QTableWidgetSelectionRange *range)
{
    //检测是否需要覆盖
    while (m_OperationBorad.curStep < m_OperationBorad.record.count()){
        delete m_OperationBorad.record.last();
        delete m_OperationBorad.recordRange.last();
        m_OperationBorad.record.removeLast();
        m_OperationBorad.recordRange.removeLast();
        m_OperationBorad.type.removeLast();
    }

    //记录
    if (*isNew){
        m_OperationBorad.curStep++;
        QList<Element> *newOpt = new QList<Element>;
        newOpt->append(graph->emt);
        m_OperationBorad.record.append(newOpt);
        m_OperationBorad.recordRange.append(range);
        m_OperationBorad.type.append(type);
    }else{
        m_OperationBorad.record.last()->append(graph->emt);
    }

    *isNew = false;

    doOptChcek();

}
/******************************************************************************
* @brief: 打开文件时使用，创建一个图元
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphTable::InsertGraph(Element emt)
{
    GraphFB *graph = GM->getUnit(emt.row, emt.col);
    graph->emt = emt;
    reDrawGraph(graph);
}
/******************************************************************************
* @brief: 在表格中插入新行，并绘制左侧第一列竖直线
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphTable::insertNewRow(int row)
{
    insertRow(row);
    insertSplitLine(row);
}
/******************************************************************************
* @brief: 插入左侧第一列竖直线
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphTable::insertSplitLine(int row)
{
    //填充 item
    for(int j=1;j<MAX_COL+1;j++){
        QTableWidgetItem *item = new QTableWidgetItem();
        setItem(row, j, item);
    }

    GraphFB graph(row, 0);
    graph.emt.graphType = NumLine;
    reDrawGraph(&graph);
}

/******************************************************************************
* @brief: 判断最大行数，始终保持空行多两个
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphTable::maxRowGraphJudge()
{
    int curMaxRow = GM->getMaxRow();
    //始终末尾多两个空行
    int rowCnt = rowCount();
    for (int i=rowCnt;i<=curMaxRow+2;i++){
        insertNewRow(i);
    }
}
/******************************************************************************
* @brief: 重绘graph
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphTable::reDrawGraph(GraphFB *graph)
{
    graph->drawGraph();

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

    //item->setToolTip(mark);
    item->setToolTip(QString("(%1,%2)").arg(row).arg(col));
    if (col == 0) return;
    maxRowGraphJudge();

}

void GraphTable::reDrawGraphNet(int row, DrawType draw)
{
    GraphFB *graph = NULL;
    QPoint range;
    if (draw == RedoDraw){
        m_ladderBack[row] += 1;
    }else if (draw == UndoDraw){
        m_ladderBack[row] -= 1;
    }

    if (draw == RePaint){
        range.setX(row);
        range.setY(row+1);
    }else{
        range = GM->getLadderRange(row);
    }
    for(int i=range.x();i<range.y();i++){
        for(int j=1;j<=MAX_COL;j++){
            graph = GM->getUnit(i, j);
            if (draw == RePaint){
               graph->setBackColor(false);
            }else{
                if (m_ladderBack[row] > 0){
                    graph->setBackColor(true);
                }else{
                    graph->setBackColor(false);
                }
            }
            reDrawGraph(graph);
        }
    }
}

/******************************************************************************
* @brief: 绘图完成后自动转下一个单元格
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphTable::setCurrentUnit(int row, int col, bool direction)
{
    //取消所有选中
    QList<QTableWidgetSelectionRange> selectRange = this->selectedRanges();
    for (int i=0;i<selectRange.count();i++){
        setRangeSelected(selectRange[i], false);
    }
    //设置当前单元格
    if (direction){
        if ((col == MAX_COL) && (row < MAX_ROW-1)){
            setCurrentCell(row+1, 1);
        }else{
            setCurrentCell(row, col+1);
        }
    }else{
        setCurrentCell(row, col);
    }
}

void GraphTable::setFirstColText()
{
    QList<QStringList> insts = GM->getInsts();
    int sum = 0;

    //先绘制第0行第0列的text
    GraphFB graphS(0, 0);
    graphS.emt.graphType = NumLine;
    graphS.emt.name = QString::number(0);
    reDrawGraph(&graphS);

    for(int i=0;i<insts.count();i++){
        //根据梯级索引 获取其所在的行号
        GraphFB graph(GM->getLadderRow(i), 0);
        graph.emt.graphType = NumLine;
        graph.emt.name = QString::number(sum+1);
        reDrawGraph(&graph);
        //发送该梯级的所有指令
        for(int j=0;j<insts[i].count();j++){
            emit sig_InsertInst(sum, insts[i][j]);
            sum++;
        }
    }
    //根据梯级索引 获取其所在的行号
    GraphFB graphE(GM->getMaxRow()+1, 0);
    graphE.emt.graphType = NumLine;
    graphE.emt.name = QString::number(sum+1);
    reDrawGraph(&graphE);
}
void GraphTable::removeGraphVLine(int row, int col)
{
    GraphFB *graph = NULL;
    //清除上半部竖直线
    graph = GM->getUnit(row, col);
    graph->clearVdnLine();
    //清除下半部竖直线
    graph = GM->getUnit(row+1, col);
    graph->clearVupLine();

}

void GraphTable::insertGraphVLine(int row, int col)
{
    GraphFB *graph = NULL;
    if (col < 2) return;
    //插入下半部分
    graph = GM->getUnit(row+1, col);
    graph->setVupLine();
    //插入上半部分
    graph = GM->getUnit(row, col);
    graph->setVdnLine();
}

void GraphTable::doOptChcek()
{
    //发送给主界面相应信号，是否使能action
    if (m_OperationBorad.curStep <= 0){
        if (m_OperationBorad.record.count() == 0){
            emit sig_enableUndo(false);
            emit sig_enableRedo(false);
        }else{
            emit sig_enableUndo(false);
            emit sig_enableRedo(true);
        }
    }else{
        if (m_OperationBorad.record.count() == m_OperationBorad.curStep){
            emit sig_enableUndo(true);
            emit sig_enableRedo(false);
        }else{
            emit sig_enableUndo(true);
            emit sig_enableRedo(true);
        }
    }
}

void GraphTable::spanUnit(int row, int col, int num)
{
    setSpan(row, col, 1, num);
}



/******************************************************************************
* @brief: 根据用户指令画相应图形
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphTable::slt_inputPara(Element emt)
{
    int i = 0;
    int curRow = currentRow();
    int curCol = currentColumn();
    GraphFB *graph = NULL;
    QTableWidgetSelectionRange *range = new QTableWidgetSelectionRange();
    bool isNew = true;
    //画图check，如果该处不允许画图，则退出
    if (!GM->checkGraph(curRow, curCol, emt.graphType)) return;

    switch (emt.graphType) {
    //如果要画竖直线，则分两步，先画下一行
    case verticalLine:
        if (emt.index<0){
            removeGraphVLine(curRow, curCol);
            RecordOperation(&isNew, GM->getUnit(curRow, curCol), RedoDeleteVLine, range);
        }else{
            insertGraphVLine(curRow, curCol);
            RecordOperation(&isNew, GM->getUnit(curRow, curCol), RedoVLineInsert, range);
        }
        reDrawGraphNet(curRow);
        reDrawGraphNet(curRow+1);
        setCurrentUnit(curRow, curCol-1);
        break;
    case HorizontalLine:
    case InputOpen:
    case InputClose:
    case InputPedge:
    case StepGraph:
    case InputNedge:
    case ReverseLogic:
        graph = GM->getUnit(curRow, curCol);
        graph->emt.graphType = emt.graphType;
        graph->emt.funInsType = emt.funInsType;
        graph->emt.name = emt.name;
        graph->emt.index = emt.index;
        graph->emt.mark = emt.mark;
        reDrawGraphNet(curRow);
        RecordOperation(&isNew, graph, RedoGraphInsert, range);
        setCurrentUnit(curRow, curCol);
        break;
    case OutputGraph:
        for(i=curCol;i<MAX_COL;i++){
            graph = GM->getUnit(curRow, i);
            graph->emt.graphType =  HorizontalLine;
            reDrawGraphNet(curRow);
            RecordOperation(&isNew, graph, RedoGraphInsert, range);
        }

        graph = GM->getUnit(curRow, MAX_COL);
        graph->emt.graphType = emt.graphType;
        graph->emt.funInsType = emt.funInsType;
        graph->emt.name = emt.name;
        graph->emt.index = emt.index;
        graph->emt.mark = emt.mark;
        reDrawGraphNet(curRow);
        RecordOperation(&isNew, graph, RedoGraphInsert, range);
        setCurrentUnit(curRow, i);
        break;
    case LogicGraph:
    {
        for(i=curCol;i<MAX_COL-1;i++){
            graph = GM->getUnit(curRow, i);
            graph->emt.graphType =  HorizontalLine;
            reDrawGraphNet(curRow);
            RecordOperation(&isNew, graph, RedoGraphInsert, range);
        }
        int spanCol = 2;
        spanUnit(curRow, MAX_COL-1, spanCol);
        graph = GM->getUnit(curRow, MAX_COL);
        graph->emt.graphType = CompGraph;
        graph = GM->getUnit(curRow, MAX_COL-1);
        graph->emt.width = spanCol;
        graph->emt.height = 1;
        graph->emt.graphType = emt.graphType;
        graph->emt.funInsType = emt.funInsType;
        graph->emt.name = emt.name;
        graph->emt.index = emt.index;
        graph->emt.mark = emt.mark;
        reDrawGraphNet(curRow);
        RecordOperation(&isNew, graph, RedoGraphInsert, range);
        setCurrentUnit(curRow, i);
        break;
    }
    case EndGraph:
        curRow = GM->getMaxRow() + 1;
        for(i=1;i<MAX_COL;i++){
            graph = GM->getUnit(curRow, i);
            graph->emt.graphType =  HorizontalLine;
        }
        graph = GM->getUnit(curRow, i);
        graph->emt.graphType = emt.graphType;
        graph->emt.funInsType = emt.funInsType;
        graph->emt.name = emt.name;
        graph->emt.index = emt.index;
        graph->emt.mark = emt.mark;
        reDrawGraphNet(curRow, RePaint);
        break;
    default:
        break;
    }
}


void GraphTable::redo()
{
    int step = m_OperationBorad.curStep;
    if ((step < 0) | (step >= m_OperationBorad.record.count())) return;

    Element emt;
    GraphFB *graph = NULL;
    QList<Element> *optList =  m_OperationBorad.record[step];
    switch (m_OperationBorad.type[step]) {
    case UndoGraphInsert:
        for (int i=0;i<optList->count();i++){
            emt = optList->at(i);
            graph = GM->getUnit(emt.row, emt.col);
            graph->setEelment(emt);
            reDrawGraphNet(emt.row);
        }
        m_OperationBorad.type[step] = RedoGraphInsert;
        setCurrentUnit(emt.row, emt.col, true);
        break;
    case UndoVLineInsert:
        for (int i=0;i<optList->count();i++){
            emt = optList->at(i);
            insertGraphVLine(emt.row, emt.col);
            reDrawGraphNet(emt.row);
            reDrawGraphNet(emt.row+1);
        }
        m_OperationBorad.type[step] = RedoVLineInsert;
        break;
    case UndoCopyPaste:
        for (int i=0;i<optList->count();i=i+2){
            emt = optList->at(i+1);
            graph = GM->getUnit(emt.row, emt.col);
            graph->setEelment(emt);
            reDrawGraphNet(emt.row);
        }
        m_OperationBorad.type[step] = RedoCopyPaste;
        break;
    case UndoCutPaste:
    {
        int startRow = m_OperationBorad.recordRange[step]->topRow();
        int startCol = m_OperationBorad.recordRange[step]->leftColumn();
        int width = m_OperationBorad.recordRange[step]->rightColumn()-
                    m_OperationBorad.recordRange[step]->leftColumn() + 1;
        for (int i=0;i<optList->count();i=i+2){
            //先把src清空
            int srcRow = startRow + (i / 2) / width;
            int srcCol = startCol + (i / 2) % width;
            graph = GM->getUnit(srcRow, srcCol);
            graph->clearAll();
            reDrawGraphNet(emt.row);
            //重置dst
            emt = optList->at(i+1);
            graph = GM->getUnit(emt.row, emt.col);
            graph->setEelment(emt);
            reDrawGraphNet(emt.row);
        }
        m_OperationBorad.type[step] = RedoCutPaste;
        break;
    }
    case UndoDelete:
        for (int i=0;i<optList->count();i++){
            emt = optList->at(i);
            graph = GM->getUnit(emt.row, emt.col);
            graph->clearEelment();
            reDrawGraphNet(emt.row);
        }
        m_OperationBorad.type[step] = RedoDelete;
        break;
    case UndoDeleteVLine:
        for (int i=0;i<optList->count();i++){
            emt = optList->at(i);
            removeGraphVLine(emt.row, emt.col);
            reDrawGraphNet(emt.row);
            reDrawGraphNet(emt.row+1);
        }
        m_OperationBorad.type[step] = RedoDeleteVLine;
        break;
    default:
        break;
    }
    m_OperationBorad.curStep++;
    doOptChcek();
}

void GraphTable::undo()
{
    int step = m_OperationBorad.curStep - 1;
    if ((step < 0) | (step >= m_OperationBorad.record.count())) return;

    Element emt;
    GraphFB *graph = NULL;
    QList<Element> *optList =  m_OperationBorad.record[step];

    switch (m_OperationBorad.type[step]) {
    case RedoGraphInsert:
        for (int i=optList->count()-1;i>-1;i--){
            emt = optList->at(i);
            graph = GM->getUnit(emt.row, emt.col);
            graph->clearEelment();
            reDrawGraphNet(emt.row, UndoDraw);
        }
        m_OperationBorad.type[step] = UndoGraphInsert;
        break;
    case RedoVLineInsert:
        for (int i=optList->count()-1;i>-1;i--){
            emt = optList->at(i);
            removeGraphVLine(emt.row, emt.col);
            reDrawGraphNet(emt.row, UndoDraw);
            reDrawGraphNet(emt.row+1, UndoDraw);
        }
        m_OperationBorad.type[step] = UndoVLineInsert;
        break;
    case RedoCopyPaste:
        for (int i=optList->count()-1;i>-1;i=i-2){
            emt = optList->at(i-1);
            graph = GM->getUnit(emt.row, emt.col);
            graph->setEelment(emt);
            reDrawGraphNet(emt.row, UndoDraw);
        }
        m_OperationBorad.type[step] = UndoCopyPaste;
        break;
    case RedoCutPaste:
    {
        int startRow = m_OperationBorad.recordRange[step]->topRow();
        int startCol = m_OperationBorad.recordRange[step]->leftColumn();
        int width = m_OperationBorad.recordRange[step]->rightColumn()-
                    m_OperationBorad.recordRange[step]->leftColumn() + 1;
        for (int i=optList->count()-1;i>-1;i=i-2){
            //先把dst还原src
            int srcRow = startRow + (i / 2) / width;
            int srcCol = startCol + (i / 2) % width;
            emt = optList->at(i);
            emt.row = srcRow;
            emt.col = srcCol;
            graph = GM->getUnit(srcRow, srcCol);
            graph->setEelment(emt);
            reDrawGraphNet(emt.row, UndoDraw);
            //再把old放到dst
            emt = optList->at(i-1);
            graph = GM->getUnit(emt.row, emt.col);
            graph->setEelment(emt);
            reDrawGraphNet(emt.row, UndoDraw);
        }
        m_OperationBorad.type[step] = UndoCutPaste;
        break;
    }
    case RedoDelete:
        for (int i=optList->count()-1;i>-1;i--){
            emt = optList->at(i);
            graph = GM->getUnit(emt.row, emt.col);
            graph->setEelment(emt);
            reDrawGraphNet(emt.row, UndoDraw);
        }
        m_OperationBorad.type[step] = UndoDelete;
        break;
    case RedoDeleteVLine:
        for (int i=optList->count()-1;i>-1;i--){
            emt = optList->at(i);
            insertGraphVLine(emt.row, emt.col);
            reDrawGraphNet(emt.row, UndoDraw);
            reDrawGraphNet(emt.row+1, UndoDraw);
        }
        m_OperationBorad.type[step] = UndoDeleteVLine;
        break;
    default:
        break;
    }
    setCurrentUnit(emt.row, emt.col, false);
    m_OperationBorad.curStep--;
    doOptChcek();
}

void GraphTable::copy()
{
    QList<QTableWidgetSelectionRange> selectRange = this->selectedRanges();
    if (selectRange.count() == 1){
        m_ClipBorad.range = selectRange[0];
        m_ClipBorad.type = RedoCopyPaste;
    }
}

void GraphTable::paste()
{
    int curRow = currentRow();
    int curCol = currentColumn();
    int row = curRow;
    int col = curCol;
    int i, j;
    bool isNew = true;
    for (i=m_ClipBorad.range.topRow();i<=m_ClipBorad.range.bottomRow();i++){
        for(j=m_ClipBorad.range.leftColumn();j<=m_ClipBorad.range.rightColumn();j++){
//            int idx = CalIdx(i, j);
//            if (idx >= GM->getCount()){
//                qDebug()<<"Paste blank";
//                break;
//            }
            row = curRow + i - m_ClipBorad.range.topRow();
            col = curCol + j - m_ClipBorad.range.leftColumn();
            GraphFB *oldGraph = GM->getUnit(row, col);
            GraphFB *dstGraph = GM->getUnit(row, col);
            GraphFB *srcGraph = GM->getUnit(i, j);

            //记录目的位置原始的graph以及新粘贴来的graph
            RecordOperation(&isNew, oldGraph, (OptType)m_ClipBorad.type, &m_ClipBorad.range);

            if (col >= MAX_COL+1) break;
            if (row >= MAX_ROW) break;
            qDebug()<<QString("Paste (%1,%2) to (%3,%4)").arg(i).arg(j).arg(row).arg(col);
            dstGraph->emt = srcGraph->emt;
            dstGraph->emt.row = row;
            dstGraph->emt.col = col;
            reDrawGraphNet(dstGraph->emt.row);

            //如果是剪切，则删除原来位置内容
            if (m_ClipBorad.type == RedoCutPaste){
                srcGraph->clearAll();
                reDrawGraphNet(srcGraph->emt.row);
            }
            //记录新粘贴来的graph
            RecordOperation(&isNew, dstGraph, (OptType)m_ClipBorad.type, &m_ClipBorad.range);

        }
    }
    if (m_ClipBorad.type == RedoCutPaste){
        m_ClipBorad.type = RedoCopyPaste;
    }
}

void GraphTable::cut()
{
    copy();
    m_ClipBorad.type = RedoCutPaste;
}
void GraphTable::remove()
{
    bool isNew = true;
    //获取当前选定的区域
    GraphFB *graph = NULL;
    QList<QTableWidgetSelectionRange> selectRange = this->selectedRanges();
    for(int i=0;i<selectRange.count();i++){
        for (int j=selectRange[i].topRow();j<=selectRange[i].bottomRow();j++){
            for(int k=selectRange[i].leftColumn();k<=selectRange[i].rightColumn();k++){              
                if (k == 0) continue;
                graph = GM->getUnit(j, k);
                RecordOperation(&isNew, graph, RedoDelete, &selectRange[0]);
                if ((graph->column() > selectRange[i].leftColumn()) &&
                    (graph->isDown() || graph->isUp())){
                    graph->clearAll();
                }else{
                    graph->clearEelment();
                }

                reDrawGraph(graph);
            }
        }
    }
}

void GraphTable::zoom()
{

    //设置表格默认的生成单元格尺寸
    horizontalHeader()->setDefaultSectionSize(GraphFB::g_unitWidth);
    verticalHeader()->setDefaultSectionSize(GraphFB::g_unitHeight);

    //设置表格的行列大小
    for (int i=0; i<columnCount();i++){
        setColumnWidth(i, GraphFB::g_unitWidth);
    }
    for (int i=0; i<rowCount();i++){
        setRowHeight(i, GraphFB::g_unitHeight);
    }

    //重绘所有graph
    for (int i=0; i<GM->getCount();i++){
        reDrawGraph(GM->getUnit(i));
    }
}

void GraphTable::find()
{

}
/******************************************************************************
* @brief: 删除竖直线，会重绘
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphTable::slt_removeGraphVLine()
{
    GraphFB *graph = NULL;
    bool isNew = true;
    //获取当前选定的区域
    QList<QTableWidgetSelectionRange> selectRange = this->selectedRanges();
    for(int i=0;i<selectRange.count();i++){
        for (int j=selectRange[i].topRow();j<=selectRange[i].bottomRow();j++){
            for(int k=selectRange[i].leftColumn();k<=selectRange[i].rightColumn();k++){
                graph = GM->getUnit(j, k);
                RecordOperation(&isNew, graph, RedoDeleteVLine, &selectRange[0]);
                removeGraphVLine(j, k);
                reDrawGraphNet(j);
                reDrawGraphNet(j+1);
            }
        }
    }
}
/******************************************************************************
* @brief: 插入一行空白图形，会重绘
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphTable::slt_insertGraphRow()
{
    int row = currentRow();
    //在当前行的上一行添加新行
    insertNewRow(row);
    if (row > GM->getMaxRow()) return;
    GM->insertRow(row);
    //刷新绘图
    for (int i=1;i<MAX_COL+1;i++){
        reDrawGraph(GM->getUnit(row, i));
    }
}
/******************************************************************************
* @brief: 删除一行图形，会重绘
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphTable::slt_removeGraphRow()
{
    int row = currentRow();
    this->removeRow(row);

    if (row > GM->getMaxRow()) return;

    GM->removeRow(row);
    //刷新绘图
    for (int i=1;i<MAX_COL+1;i++){
        reDrawGraph(GM->getUnit(row, i));
    }
}


void GraphTable::BuildGraph()
{
    int maxIdx = GM->getCount();
    if (maxIdx == 0) return;

    int ret = GM->buildGraph();
    if (ret == -1){
        QPoint pt = GM->getErrorUnit();
        sig_InsertBottomRowText(QString("(%1, %2)处编译错误！").arg(pt.x()).arg(pt.y()));
        return;
    }
    //重绘所有图，并清除编辑过的行的底色
    //清屏重绘
    int curRow = GM->getMaxRow();
    for(int i=0;i<=curRow;i++){
        insertSplitLine(i);
        reDrawGraphNet(i, RePaint);
    }

    setFirstColText();

    //加入End行
    Element emt;
    emt.row = GM->getMaxRow()+1;
    emt.col = 1;
    emt.name = "END";
    emt.graphType = EndGraph;
    slt_inputPara(emt);

}

void GraphTable::RunGraph(bool enable)
{

}

void GraphTable::wheelEvent(QWheelEvent *event)
{
    //首先检测是否按下CTRL
    if (QApplication::keyboardModifiers() == Qt::ControlModifier){
        if(event->delta()>0){//如果滚轮往上滚
            emit sig_zoomin();
        }else{
            emit sig_zoomout();
        }
    }else{
        QTableWidget::wheelEvent(event);
    }
}
void GraphTable::contextMenuEvent(QContextMenuEvent *event)
{
    pop_menu->clear(); //清除原有菜单
    QPoint point = event->pos(); //得到窗口坐标

    pop_menu->addAction(selectAllAct);
    pop_menu->addSeparator();
    pop_menu->addAction(undoAct);
    pop_menu->addAction(redoAct);
    pop_menu->addSeparator();
    pop_menu->addAction(copyAct);
    pop_menu->addAction(cutAct);
    pop_menu->addAction(pasteAct);
    pop_menu->addAction(removeAct);
    pop_menu->addSeparator();
    pop_menu->addAction(removeVLine);
    pop_menu->addAction(insertRowAct);
    pop_menu->addAction(removeRowAct);
    //菜单出现的位置为当前鼠标的位置
    pop_menu->exec(QCursor::pos());
    event->accept();

    QTableWidgetItem *item = this->itemAt(point);
    if(item != NULL){

    }
}

void GraphTable::keyPressEvent(QKeyEvent *event)
{
    int  key = event->key();

    if (((key >= Qt::Key_A) && (key <= Qt::Key_Z))
            || (key == Qt::Key_Backspace)){
        emit sig_showInputWindow(event->text(), false);
    }
    QTableWidget::keyPressEvent(event);
}

void GraphTable::slt_itemDoubleClicked(QTableWidgetItem *item)
{
    int row = item->row();
    int col = item->column();
    if (col==0) return;

    GraphFB *graph = GM->getUnit(row, col);
    QString text;
    switch (graph->getType()) {
    case InputOpen:
        text = "LD";
        break;
    case InputClose:
        text = "LDI";
        break;
    case OutputGraph:
        text = "OUT";
        break;
    default:
        break;
    }
    if (!text.isEmpty()){
        text = QString("%1 %2%3").arg(text).arg(graph->getName()).arg(graph->getIndex());
    }
    emit sig_showInputWindow(text, true);
}



/******************************************************************************
* @brief: 创建右键菜单的action
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphTable::createActions()
{
    //创建菜单项
    pop_menu = new QMenu();

    selectAllAct = new QAction(tr("全选"), this);
    selectAllAct->setShortcut(QKeySequence::SelectAll);
    connect(selectAllAct, SIGNAL(triggered()), this, SLOT(selectAll()));

    undoAct = new QAction(tr("撤销"), this);
    undoAct->setShortcut(QKeySequence::Undo);
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    redoAct = new QAction(tr("恢复"), this);
    redoAct->setShortcut(QKeySequence::Redo);
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));

    copyAct = new QAction(tr("复制"), this);
    copyAct->setShortcut(QKeySequence::Copy);
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(tr("粘贴"), this);
    pasteAct->setShortcut(QKeySequence::Paste);
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    cutAct = new QAction(tr("剪切"), this);
    cutAct->setShortcut(QKeySequence::Cut);
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    removeAct = new QAction(tr("删除"), this);
    removeAct->setShortcut(QKeySequence::Delete);
    connect(removeAct, SIGNAL(triggered()), this, SLOT(remove()));

    removeVLine = new QAction(tr("清除垂直线"), this);
    connect(removeVLine, SIGNAL(triggered()), this, SLOT(slt_removeGraphVLine()));

    insertRowAct = new QAction(tr("插入新行"), this);
    connect(insertRowAct, SIGNAL(triggered()), this, SLOT(slt_insertGraphRow()));

    removeRowAct = new QAction(tr("删除该行"), this);
    connect(removeRowAct, SIGNAL(triggered()), this, SLOT(slt_removeGraphRow()));

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


void GraphWindow::OpenGraph(QString name)
{
    QFile fileRead(name);
    fileRead.open(QIODevice::ReadOnly);
    QDataStream  readDataStream(&fileRead);
    GraphFB gfb;
    m_graphTable->InitTable();
    while(!fileRead.atEnd()){
        readDataStream  >> gfb;
        m_graphTable->InsertGraph(gfb.emt);
    }
    fileRead.close();

    m_graphTable->InitParament();
}
void GraphWindow::SaveGraph(QString name)
{
    QFile fileWrite(name);
    fileWrite.open(QIODevice::WriteOnly);
    QDataStream  writeDataStream(&fileWrite);
    GraphFB gfb;
    for (int i=0;i<GM->getCount();i++){
        GraphFB *graph = GM->getUnit(i);
        gfb.emt = graph->emt;
        gfb.entColor = graph->entColor;
        gfb.conColor = graph->conColor;
        writeDataStream << gfb;
    }
    fileWrite.close();
}







