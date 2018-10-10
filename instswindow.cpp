#include "instswindow.h"

InstsWindow::InstsWindow(QWidget *parent) :
    QTableWidget(parent)
{
    setWindowTitle("指令模式");
    setRowCount(INIT_ROW);
    setColumnCount(2);

    horizontalHeader()->hide();
    verticalHeader()->hide();



    setShowGrid(false);//隐藏表格线
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置多选（可以Ctral+A全选Ctral+Shift多选）获取所选行号：
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}
