#include "graphmodel.h"

GraphModel* GraphModel::m_instance;
GraphModel::GraphModel()
{
    //初始化数组
    m_graphList.clear();
}
/******************************************************************************
* @brief: 获取指定行列的单元格
* @author:leek
* @date 2018/10/10
*******************************************************************************/
GraphFB* GraphModel::getUnit(int row, int col)
{
    int idx = CalIdx(row, col);
    return getUnit(idx);
}

GraphFB *GraphModel::getUnit(int idx)
{
    //如果超出范围，先插入，再获取
    if (idx >= m_graphList.count()){
        appendUnit(idx);
    }
    return m_graphList[idx];
}
/******************************************************************************
* @brief: 设置graph
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphModel::setUnit(GraphFB *graph)
{
    int row = graph->emt.row;
    int col = graph->emt.col;
    int idx = CalIdx(row, col);
    //如果超出范围，则先添加新单元格到该行的最后一列元素位置
    if (idx >= m_graphList.count()){
        appendUnit(row, col);
    }
    //如果不等于则设置为新的graph
    if (graph != m_graphList[idx]){
        //先删除原来的graph，防止内存泄漏
        delete m_graphList[idx];
        //再设置新的graph
        m_graphList[idx] = graph;
    }
}
/******************************************************************************
* @brief: 在指定行列出插入单元格，如果不够，则补齐到该位置的单元格
* @author:leek
* @date 2018/10/14
*******************************************************************************/
void GraphModel::appendUnit(int row, int col)
{
    int idx = CalIdx(row, col);
    appendUnit(idx);
}
/******************************************************************************
* @brief: 指定位置插入图元
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphModel::appendUnit(int idx)
{
    int curMax = m_graphList.count();
    for(int i=curMax;i<=idx;i++){
        GraphFB *graph = new GraphFB(i/MAX_COL, (i%MAX_COL)+1);
        m_graphList.append(graph);
    }
}
/******************************************************************************
* @brief: 移除指定行列的图元
* @author:leek
* @date 2018/10/10
*******************************************************************************/

void GraphModel::removeUnit(int row, int col)
{
    int idx = CalIdx(row, col);
    if (idx < m_graphList.count()){
        delete m_graphList[idx];
        m_graphList.removeAt(idx);
    }
}
/******************************************************************************
* @brief: 移除指定的图元
* @author:leek
* @date 2018/10/10
*******************************************************************************/

void GraphModel::removeUnit(GraphFB *graph)
{
    removeUnit(graph->emt.row, graph->emt.col);
}
/******************************************************************************
* @brief: 插入新行，竖直线需要继承，插入规则为在当前行的上面插入一行
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphModel::insertRow(int row)
{
    int curIdx, newIdx;
    //1.先插入空行,添加新行的graph列表
    for (int i=1;i<MAX_COL+1;i++){
        newIdx = CalIdx(row, i);
        GraphFB *graph = new GraphFB(row, i);
        m_graphList.insert(newIdx, graph);
    }
    //2.判断是否有竖直线，有的话新添加的行需要继承
    for(int i=1;i<MAX_COL+1;i++){
        newIdx = CalIdx(row, i);
        curIdx = CalIdx(row+1, i);
        if (m_graphList[curIdx]->emt.upFlag){
            m_graphList[newIdx]->emt.dnFlag = true;
            m_graphList[newIdx]->emt.upFlag = true;
        }
    }
    //3.修改插入位置之后的所有graph的行列信息
    curIdx = CalIdx(row+1, 1);
    for(int i=curIdx;i<m_graphList.count();i++){
        m_graphList[i]->emt.row += 1;
    }
}
/******************************************************************************
* @brief: 移除一行单元格
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphModel::removeRow(int row)
{
    //1.先修改移除位置之后的所有graph的行列信息
    int curIdx = CalIdx(row+1, 1);
    for(int i=curIdx;i<m_graphList.count();i++){
        m_graphList[i]->emt.row -= 1;
    }
    //2.移除该行单元格
    for(int i=1;i<MAX_COL+1;i++){
        /*这里要注意，列为1，因为移除一个后剩下的会自动前移，
        因此每次都是移动第一列位置的graph*/
        removeUnit(row, 1);
    }

}
/******************************************************************************
* @brief: 获取列表当前存储的单元数
* @author:leek
* @date 2018/10/10
*******************************************************************************/
int GraphModel::getCount()
{
    return  m_graphList.count();
}
/******************************************************************************
* @brief: 获取列表当前存储的最大行数
* @author:leek
* @date 2018/10/10
*******************************************************************************/
int GraphModel::getMaxRow()
{
    return  m_graphList.last()->emt.row;
}
/******************************************************************************
* @brief: 清除图元记录链表
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphModel::clearAllGraph()
{
    m_graphList.clear();
}
/******************************************************************************
* @brief: 画图检测函数
* @author:leek
* @date 2018/10/10
*******************************************************************************/
bool GraphModel::checkGraph(int row, int col, int type)
{
    switch (type) {
    case verticalLine:

        break;
    default:
        break;
    }

    return true;
}

/**********基于正向深度优先扫描的转换算法***
 *
 * 转换过程：梯形图--->树--->指令表
 * 第一步：从左到右，从上到下，扫描所有梯形图，根据元件位置判断其在树中位置，构建树
 * 树由三个数据结构组成，树（tree）,节点（node），表（list），分别对应根节点，主干节点和叶子节点
 * 第二步：扫描树，生成指令表
*/
void GraphModel::buildGraph()
{
    BTree<int> *tree = new BTree<int>();
}
