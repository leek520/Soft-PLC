#include "instswindow.h"
InstsWindow::InstsWindow(QWidget *parent) :
    QTableWidget(parent)
{
    setWindowTitle("指令模式");

    horizontalHeader()->hide();
    verticalHeader()->hide();

    setShowGrid(false);//隐藏表格线
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置多选（可以Ctral+A全选Ctral+Shift多选）获取所选行号：
    setSelectionBehavior(QAbstractItemView::SelectRows);
    horizontalHeader()->setStretchLastSection(true);//最后一列自适应

    initTable();
    setZoom(1);
}

void InstsWindow::setZoom(double factor)
{
    int width = factor * UNIT_WIDTH;
    int height = factor * UNIT_INST_HEIGH;

    for (int i=0; i<columnCount()-1;i++){
        setColumnWidth(i, width);
    }

    for (int i=0; i<rowCount();i++){
        setRowHeight(i, height);
        for (int j=0; j<columnCount()-1;j++){
            setItemFormat(i, j);
        }
    }

    verticalHeader()->setDefaultSectionSize(height);
}

void InstsWindow::buildInsts()
{
    initTable();
}

void InstsWindow::initTable()
{
    clearContents();
    setColumnCount(4);

    for(int i=0;i<INIT_ROW;i++){
        slt_InsertInst(i);
    }
    for (int i=0; i<columnCount()-1;i++){
        setColumnWidth(0, UNIT_WIDTH);
    }
}

void InstsWindow::setItemFormat(int row, int col)
{
//    QTableWidgetItem *unit = item(row, col);
//    if (unit == NULL) return;

//    QFont font;
//    int fontSize = GraphFB::g_factor * 16;
//    font.setPixelSize(fontSize);
//    unit->setFont(font);
}

void InstsWindow::slt_InsertInst(int row, QString inst)
{
    QStringList instStr = inst.split(" ");
    if (row >= rowCount()){
        insertRow(row);
    }
    QTableWidgetItem *item;

    //用0补齐6位十进制数
    QString address = QString("%1").arg(row, 6, 10, QLatin1Char('0'));
    item = new QTableWidgetItem(address);
    setItem(row, 0, item);
    setItemFormat(row, 0);

    item = new QTableWidgetItem(instStr[0]);
    setItem(row, 1, item);
    setItemFormat(row, 1);

    if (instStr.count() >= 2){
        item = new QTableWidgetItem(instStr[1]);
        setItem(row, 2, item);
        setItemFormat(row, 2);
    }
}

