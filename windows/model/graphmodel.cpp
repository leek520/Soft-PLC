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
    if (getCount()>0){
        return  m_graphList.last()->emt.row;
    }else{
        return -1;
    }
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
    clearBuild();

    createBTree();

    createInsts();
}
/******************************************************************************
* @brief: 获取该模型编译好的指令集
* @author:leek
* @date 2018/10/10
*******************************************************************************/
QStringList GraphModel::getInsts()
{
    return m_instsList;
}
/******************************************************************************
* @brief: 递归处理节点
* @author:leek
* @date 2018/10/10
*******************************************************************************/
//此处传入的row和col不是窗口中的实际坐标，而是列-1。
int GraphModel::dealBTreeNode(int row, Direction dir, BTreeNode *node)
{
    BTreeNode *parentNode = node;
    BTreeNode *rightNode = NULL;
    int col = m_buildInfo.start[row];
    int idx = GraIdx(row, col);
    if (idx >= m_graphList.count()) return -1;
    if ((row >= MAX_ROW) | (row < 0)) return -1;
    if ((col >= MAX_COL) | (col < 0)) return -1;

    //qDebug()<< "cur pos:" <<row << m_buildInfo.start[row];
    //是否为空
    if (m_graphList[idx]->isEmpty()){
        dealBTreeNode(row-1);
        return 1;
    }

    //是否要转上一行：条件=upflag和已经处理完，即上一行处理到尾部了
    if ((m_graphList[idx]->isUp()) &&
        (m_buildInfo.start[row-1] < MAX_COL)){
        return 1;
    }
    //水平线直接转右
    if ((m_graphList[idx]->getType() == HorizontalLine) ||
        (m_graphList[idx]->getType() == NoneGraph)){
        m_buildInfo.start[row] += 1;
        dealBTreeNode(row, TurnRight, parentNode);
        return 1;
    }

    //建立节点
    if (dir == TurnRight){
        //右转到最后一列时，保存当前节点和out的对应关系
        if (col == MAX_COL-1){
            // 查找输出的节点位置
            //从最后一列往左查找up位置
            BTreeNode *outNodeTmp = NULL;
            for(int i=MAX_COL-2;i>0;i--){
                int dnIndex = GraIdx(row, i);
                //如果向上标志，则上移一行，继续判断（这里可能是连续竖直向上线）
                int t=0;
                while(m_graphList[dnIndex-t*MAX_COL]->isUp()){
                    t++;
                }
                if (t>0){
                    //找到之后从up位置的上一行往右查找第一个有效图元
                    for(int k=0;k<MAX_COL-i;k++){
                        int upIndex = dnIndex+k-t*MAX_COL;
                        if (m_graphList[upIndex]->getType()>=InputOpen){
                            outNodeTmp = node->find(upIndex, LeafNode);
                            break;
                        }
                    }
                    break;
                }
            }

            if (outNodeTmp != NULL){
                m_OutNode->insert(idx, outNodeTmp->root()->left);
            }else{
                m_OutNode->insert(idx, node->root());
            }

        }else{

            parentNode = new BTreeNode(idx, SerialNode);
            rightNode = new BTreeNode(idx, LeafNode);
            parentNode->insert(node->root(), Left);
            parentNode->insert(rightNode, Right);
        }
    }

    m_buildInfo.start[row] += 1;
    m_buildTrail.append(QPoint(row, col));
    QString text = QString("Pos:(%1,%2), %3%4")
                    .arg(row).arg(col)
                    .arg(m_graphList[idx]->emt.name)
                    .arg(m_graphList[idx]->emt.index);
    qDebug()<<text;


    if  (col == MAX_COL-1){//如果处理到当前行的最后一列，则直接转下一列
        m_buildInfo.end[row+1] = MAX_COL;
        //当该行扫描到最后一个输出时，如果下行没有扫描过，说明这两行没有竖直线连接
        if (m_buildInfo.start[row+1] == 0){
            return 1;
        }else{
            //这里一行处理到out转下一行类似于右转，所有为TurnRight****
            dealBTreeNode(row+1, TurnRight, parentNode);
        }

        return 1;
    }

    if (idx+1 >= m_graphList.count()) return 1;

    if (m_graphList[idx+1]->isDown()){
        //此处记录本次梯级可以扫描到的最大行数
        if (row >= m_buildInfo.startRow){
            m_buildInfo.startRow = row + 1;
        }
        //设置转下一行扫描时可以达到的最大列数，为转下时的列位置
        m_buildInfo.end[row+1] = m_buildInfo.start[row];


        parentNode = new BTreeNode(idx, ParallelNode);

        //计算下一行要处理的idx
        idx = GraIdx(row+1, m_buildInfo.start[row+1]);
        rightNode = new BTreeNode(idx, LeafNode);
        //如果OR节点后只有一个节点，则直接优先使用OR
        if (m_graphList[idx+1]->isUp()){
            //新建并联右节点，处理完后重新赋值二叉树节点进行下一次
            if (node->root() == node){
                parentNode->insert(node, Left);
            }else{
                //这种情况下，下次递归start未执行+1，所以这里提前执行
                m_buildInfo.start[row+1] += 1;

                parentNode->insert(node->root()->right, Left);
                node->root()->right = parentNode;
                parentNode->parent = node->root();
            }

            parentNode->insert(rightNode, Right);
            //转下一行
            dealBTreeNode(row+1, TurnDown, parentNode);
        }else{
            dealBTreeNode(row+1, TurnDown, rightNode);
            //新建并联右节点，处理完后重新赋值二叉树节点进行下一次
            parentNode->insert(node->root(), Left);
            parentNode->insert(rightNode->root(), Right);
        }

    }

    //右转
    dealBTreeNode(row, TurnRight, parentNode);
}

/******************************************************************************
* @brief: 二叉树的中序遍历
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphModel::inOrderTraversal(BTreeNode *node, int index)
{
    QString inst = "";
    GraphFB *graph = NULL;
    BTNodePos pos;
    if(node != NULL)
    {
        //是叶子节点
        if (node->type == LeafNode){
            graph = getUnit(node->value);
            //先判断是否有父节点，如果没有默认为左叶子节点，如果有，判断左右
            if (node->parent == NULL){
                pos = Left;
            }else{
              if (node->parent->left == node){    //左叶子
                  pos = Left;
              }else{
                  pos = Right;
              }
            }

            //左右叶子节点分开处理
            if (pos == Left){    //左叶子
                if (graph->getType() == InputOpen){
                    inst = "LD";
                }else if (graph->getType() == InputClose){
                    inst = "LDI";
                }
            }else{      //右叶子
                if (node->parent->type == SerialNode){
                    if (graph->getType() == InputOpen){
                        inst = "AND";
                    }else if (graph->getType() == InputClose){
                        inst = "ANI";
                    }
                }else if (node->parent->type == ParallelNode){
                    if (graph->getType() == InputOpen){
                        inst = "OR";
                    }else if (graph->getType() == InputClose){
                        inst = "ORI";
                    }
                }
            }
            //生成指令
            m_instsList.append(QString("%1 %2%3")
                               .arg(inst)
                               .arg(graph->getName())
                               .arg(graph->getIndex()));
        }else{
            //并联或者串联节点的遍历
            //先遍历左节点
            inOrderTraversal(node->left, index);

            //再遍历右节点
            inOrderTraversal(node->right, index);

            //如果右节点不是叶子节点，则需要额外输出串并联指令
            if (node->right->type != LeafNode){
                if (node->type == SerialNode){
                    m_instsList.append(QString("ANB"));
                }else{
                    m_instsList.append(QString("ORB"));
                }
            }
        }
        dealMultOutNode_MPP(node, index);
    }

}

/******************************************************************************
* @brief: 输出节点处理
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphModel::dealMultOutNode_MPP(BTreeNode *node, int index)
{
    GraphFB *graph = NULL;
    //判断是否要输出

    m_OutNode = m_OutTree[index];
    QList<int> keys = m_OutNode->keys();
    for(int i=0;i<keys.count();i++)
    {
//        判断是否需要MPS
//        如果该节点有输出，则判断其父节点是否也有输出，并且该输出在本次输出的上方，需要MPS
//        TODO
//        输出节点加入的不对，见eg-11
        if (node == m_OutNode->value(keys[i])){
            //如果上一个输出节点还没有输出，则当前节点暂时不输出
            if ((i != 0) && (m_buildInfo.outStatus[i-1] == false)){
                if (m_OutNode->value(keys[i-1]) != m_OutNode->value(keys[i])){
                    m_instsList.append(QString("MPS"));
                }
            }else{
                m_buildInfo.outStatus[i] = true;
                graph = getUnit(keys[i]);
                m_instsList.append(QString("OUT %1%2")
                                   .arg(graph->getName())
                                   .arg(graph->getIndex()));
            }
        }
    }
}

void GraphModel::dealMultOutNode_MPS(BTreeNode *node, int index)
{
    GraphFB *graph = NULL;
    //判断是否要输出
    m_OutNode = m_OutTree[index];
    QList<int> keys = m_OutNode->keys();
    for(int i=0;i<keys.count();i++)
    {
        if ((i != 0) &&(false == m_buildInfo.outStatus[i])){
            //如果本次输出和上次输出的输出节点指向同一个节点，那么只用输出一次MPP
            if (m_OutNode->value(keys[i-1]) != m_OutNode->value(keys[i])){
                m_instsList.append(QString("MPP"));
            }
            graph = getUnit(keys[i]);
            m_instsList.append(QString("OUT %1%2")
                               .arg(graph->getName())
                               .arg(graph->getIndex()));
        }
    }

}
/******************************************************************************
* @brief: 编译前清理相关变量
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphModel::clearBuild()
{
    memset(m_buildInfo.start, 0, MAX_ROW * sizeof(uchar));
    memset(m_buildInfo.end, MAX_COL, MAX_ROW * sizeof(uchar));
    m_buildInfo.startRow = 0;
    m_buildInfo.outStatus.clear();


    m_buildTrail.clear();
    m_HeadNode.clear();
    m_instsList.clear();
    m_OutNode = NULL;

    for(int i=0;i<m_OutTree.count();i++){
        delete m_OutTree[i];
    }
    m_OutTree.clear();
}
/******************************************************************************
* @brief: 根据梯形图链表生成二叉树森林
* @author:leek
* @date 2018/10/10
*******************************************************************************/
void GraphModel::createBTree()
{
    //编译，生成二叉树森铃
    int ret = 0;
    int row = 0;
    while(row < MAX_ROW)
    {
        //本次梯级递归起始行号为上次扫描到的最大行号
        row = m_buildInfo.startRow;
        if (row <= getMaxRow()){
            m_OutNode = new QMap<int, BTreeNode *>();
            //记录本次梯级第一个叶子节点,value为idx
            BTreeNode *firstNode = new BTreeNode(GraIdx(row, 0), LeafNode);
            m_HeadNode.append(firstNode);
            //递归处理本次梯级
            ret = dealBTreeNode(row, TurnStart, firstNode);

            //该梯级处理完成后自动转下一行
            m_buildInfo.startRow++;

            //记录梯级行号和本梯级内输出节点个数的对应关系
            m_OutTree.append(m_OutNode);

            if (ret == -1){
                break;
            }
        }else{
            break;
        }
    }
}
/******************************************************************************
* @brief: 根据二叉树森林，利用中序遍历，生成指令集
* @author:leek
* @date 2018/10/10
*******************************************************************************/
int GraphModel::createInsts()
{
    for(int i=0;i<m_HeadNode.count();i++){
        //生成指令前按照输出节点个数初始化变量
        int j = m_OutTree[i]->count();
        m_buildInfo.outStatus.clear();
        while(j--){
            m_buildInfo.outStatus.append(false);
        }

        BTreeNode *root = m_HeadNode[i]->root();
        inOrderTraversal(root, i);
        dealMultOutNode_MPS(m_HeadNode[i], i);
    }
}
