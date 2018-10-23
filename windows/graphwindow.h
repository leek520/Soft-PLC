#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H
/**
Copyright (c) 2018 PLT. All Rights Reserved.
    *@file：
    *@brief： 梯形图编辑窗口
    *@author ：李奎
    *@date ：2018/10/01
    *@description：
*/
#include <QTableWidget>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QStyledItemDelegate>
#include <QScrollBar>
#include <QMouseEvent>
#include <QPainter>
#include <QSpinBox>
#include <QDebug>
#include <QMenu>
#include <QMessageBox>
#include <QApplication>
#include "common.h"
#include "graphfb.h"
#include "graphmodel.h"
class GraphItemDelegate : public QStyledItemDelegate
{
public:
    GraphItemDelegate(QObject* parent = 0):QStyledItemDelegate(parent){ }
    //在委托类的paint中画图
    virtual void paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

};
class GraphTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit GraphTable(QWidget *parent = 0);
    virtual ~ GraphTable(){}
    void InitTable();
    void InitParament();

    void RecordOperation(bool *isNew,
                         GraphFB *graph,
                         OptType type,
                         QTableWidgetSelectionRange *range);

    void InsertGraph(Element emt);

    void BuildGraph();
    void RunGraph(bool enable);

protected:
    void wheelEvent(QWheelEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void keyPressEvent(QKeyEvent *event);

signals:
    void sig_InsertBottomRowText(QString text);
    void sig_InsertInst(int row, QString inst);
    void sig_zoomin();
    void sig_zoomout();
    void sig_enableUndo(bool status);
    void sig_enableRedo(bool status);
    void sig_showInputWindow(QString inst, bool isSelected);
private slots:
    void slt_itemDoubleClicked(QTableWidgetItem *item);
private:
    void createActions();
    void insertNewRow(int row);
    void insertSplitLine(int row);
    void maxRowGraphJudge();
    void reDrawGraph(GraphFB *graph);
    void setCurrentUnit(int row, int col, bool direction=true);

    void removeGraphVLine(int row, int col);
    void insertGraphVLine(int row, int col);
    void doOptChcek();
    void spanUnit(int row, int col, int num=1);
    //To do:
    /*
    1.用户插入图形合法性校验
    2.编译时去除空白行；
    5.软件打开时的无边框加载图
    6.如何做类似word样式的菜单
    */

public slots:
    void slt_inputPara(Element emt);
    void slt_removeGraphVLine();
    void slt_insertGraphRow();
    void slt_removeGraphRow();

    void redo();
    void undo();
    void copy();
    void paste();
    void cut();
    void remove();
    void zoom();
    void find();


private:
    QMenu *pop_menu;
    QAction *selectAllAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *cutAct;
    QAction *removeAct;
    QAction *removeVLine;
    QAction *insertRowAct;
    QAction *removeRowAct;

    //操作板：记录操作类型，操作的表格范围以及操作的数据
    struct OperationBorad{
        int curStep;
        QList<QList<Element> *> record;
        QList<QTableWidgetSelectionRange *> recordRange;
        QList<OptType> type;
    }m_OperationBorad;

    //粘贴板：记录是复制还是粘贴，以及操作的表格范围
    struct ClipBorad{
        int type;
        QTableWidgetSelectionRange range;
    }m_ClipBorad;
};



class GraphWindow : public QWidget
{
    Q_OBJECT
public:
    explicit GraphWindow(QWidget *parent = 0);
    virtual ~ GraphWindow(){}
signals:

public slots:

    void SaveGraph(QString name);
    void OpenGraph(QString name);
public:
    GraphTable *m_graphTable;
};

#endif // GRAPHWINDOW_H
