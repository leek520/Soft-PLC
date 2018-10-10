#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

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

class GraphItemDelegate : public QStyledItemDelegate
{
public:
    GraphItemDelegate(QObject* parent = 0):QStyledItemDelegate(parent){ }
    //在委托类的paint中画图
    virtual void paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;
//    //创建编辑器，当视图需要一个编辑器时，它通知委托来为被修改的项目提供一个编辑器部件
//    QWidget *createEditor(QWidget *parent,
//                          const QStyleOptionViewItem &/* option */,
//                          const QModelIndex &/* index */) const

//    {
//        QSpinBox *editor = new QSpinBox(parent);
//        editor->setMinimum(0);
//        editor->setMaximum(100);
//        return editor;
//    }
//    //为编辑器设置数据
//    void setEditorData(QWidget *editor, const QModelIndex &index) const
//    {
//        int value = index.model()->data(index,Qt::EditRole).toInt();
//        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
//        spinBox->setValue(value);
//    }

//    //将数据写入模型中
//    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
//    {
//        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
//        spinBox->interpretText();//确保获得的是QSpinBox中最近更新的数值，标准的QItemDelegate会在完成编辑后发射closeEdit信号
//        int value = spinBox->value();
//        model->setData(index,value,Qt::EditRole);
//    }


//    //更新编辑器的几何布局
//    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
//    {
//        editor->setGeometry(option.rect);
//    }

     bool editorEvent(QEvent *event,
                      QAbstractItemModel *model,
                      const QStyleOptionViewItem &option,
                      const QModelIndex &index);


};
class GraphTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit GraphTable(QWidget *parent = 0);
    virtual ~ GraphTable(){}
    void InitTable();
    void InitParament();

    void InsertRecordOpt(Element emt, OptType type, bool *isNew);

    void InsertGraph(Element emt);

    void BuildGraph();
    void RunGraph(bool enable);
protected:
    void contextMenuEvent(QContextMenuEvent *event);
signals:
    void sig_InsertBottomRowText(QString text);

private:
    void createActions();

    void InsertSplitLine(int row);
    void InsertNewRow(int row);
    void RowGraphJudge(int row, int col);
    void RecordGraph(GraphFB *graph);
    void RemoveGraph(int row, int col);
    GraphFB *GetGraph(int row, int col);
    void ReDrawGraph(GraphFB *graph);
    void SetCurrentGraph(int row, int col);
    int DealNode(int row, int col);

    //To do:
    /*
    1.用户插入图形合法性校验
    2.编译时去除空白行
    3.编译时合法性校验

    4.关闭时提示保存，新建时提示保存
    5.软件打开时的无边框加载图
    6.如何做类似word样式的菜单
    */

public slots:
    void slt_inputPara(QString name, int index, QString mark, int type);
    void redo();
    void undo();
    void copy();
    void paste();
    void cut();
    void remove();
    void zoomin();
    void zoomout();
    void find();

    void insertRowGraph();
public:
    QList<GraphFB *> m_graphList;

private:
    QMenu *pop_menu;
    QAction *selectAllAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *cutAct;
    QAction *removeAct;
    QAction *insertRowAct;


    int buildPos[MAX_ROW];
    int buildPreRow;
    QList<QPoint> m_buildTrail;

    struct RecordBorad{
        int curStep;
        QList<QList<Element> > record;
        QList<OptType> type;
    }m_recordBorad;

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
