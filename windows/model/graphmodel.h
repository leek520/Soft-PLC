#ifndef GRAPHMODEL_H
#define GRAPHMODEL_H
/**
Copyright (c) 2018 PLT. All Rights Reserved.
    *@file：
    *@brief： 梯形图数据模型，负责增删一个单元格/一行单元格等
    *@author ：李奎
    *@date ：2018/10/01
    *@description：
*/
#include <QDebug>
#include "graphfb.h"
#include "binarytree.h"
#define GM (GraphModel::GetInstance())


struct BuildInfo
{
    uchar start[MAX_ROW];
    uchar end[MAX_ROW];
    int startRow;
    QList<bool> outStatus;
    QList<int> blankRow;
};
class GraphModel
{
private:
    static GraphModel *m_instance;
    GraphModel();

public:
    //GetInstance是获得此类实例的唯一全局访问点
    static GraphModel* GetInstance() {
        if (m_instance == NULL) {
            m_instance = new GraphModel();
        }
        return m_instance;
    }
    //获取一个单元格
    GraphFB* getUnit(int row, int col);
    GraphFB* getUnit(int idx);
    //重设一个单元格
    void setUnit(GraphFB* graph);

    //插入一个单元格
    void appendUnit(int row, int col);
    void appendUnit(int idx);
    //删除一个单元格
    void removeUnit(int row, int col);
    void removeUnit(GraphFB* graph);
    //插入一行单元格
    void insertRow(int row);
    //删除一个行单元格
    void removeRow(int row);

    bool isEmptyGraph(int row, int col);
    //获取用户已经插入图形的最大个数
    int getCount();
    //获取用户已经插入图形的最大行数
    int getMaxRow();
    void clearAllGraph();

    QPoint getLadderRange(int row);
    bool checkGraph(int row, int col, int type);
    void buildGraph();
    QStringList getInsts();
    BuildInfo *getBuildInfo();
    QList<int> getNetRow();
private:
    void clearBuild();
    bool checkGraph();
    void createBTree();
    int createInsts();
    int dealBTreeNode(int row, Direction dir = TurnStart,BTreeNode *node = NULL);
    void inOrderTraversal(BTreeNode* node, int index); //中序遍历
    void dealMultOutNode_MPP(BTreeNode *node, int index);
    void dealMultOutNode_MPS(BTreeNode *node, int index);
private:
    QList<GraphFB *> m_graphList;
    QList<BTreeNode *> m_HeadNode;

    QList<QMap<int, BTreeNode *>* > m_OutTree;
    QMap<int, BTreeNode *> *m_OutNode;

    QList<int> m_ladderRow;
    QList<QPoint> m_buildTrail;
    QStringList m_instsList;
    BuildInfo m_buildInfo;
    QList<int> m_netRow;
};

#endif // GRAPHMODEL_H
