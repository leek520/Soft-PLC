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
#include "graphfb.h"
#include "binarytree.h"
#define GM (GraphModel::GetInstance())

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

    //获取用户已经插入图形的最大个数
    int getCount();
    //获取用户已经插入图形的最大行数
    int getMaxRow();
    void clearAllGraph();

    bool checkGraph(int row, int col, int type);
    void buildGraph();
private:
    QList<GraphFB *> m_graphList;
};

#endif // GRAPHMODEL_H
