#ifndef BINARYTREE_H
#define BINARYTREE_H

/***
 * 梯形图转指令表
 *(1)从梯形图的第一个梯级的母线开始自左往右，自上往下扫描，直到有垂直线为止
 * 根据扫描经过的节点数，建立逻辑二叉树。具体方法：每扫描到两个节点，则新建
 * 一个AND逻辑节点，它为父节点，扫描到的两个为子节点，先扫描到的位左子节点
 * 后扫描到的为右子节点。新建的AND节点作为一个左子节点，继续参与后面新节点的
 * 建立，并假定最后新建的AND节点为L1
 * (2)从上步保存的向下线的位置开始，自右向左，自下向上扫描，直到扫描到向上线
 * 为止。每扫描到两个节点，则新建一个AND节点，它为父节点，扫描到的两个为子节点
 * 先扫描到的为右子节点，后扫描到的为左子节点。新建的AND节点作为一个右子节点
 * 继续参与后面新节点的建立，假定新建的AND节点为L2，然后再新建OR节点，它为
 * 父节点，L1为左子节点，L2为右子节点
 * (3)如果当前行还有向下线，则递归第(2)步，直到扫描完这一梯级，这样一个逻辑
 * 二叉树就构建起来了，依照上面步骤继续循环构建后面梯级，得到整个逻辑二叉树森林
 * 对二叉树森林进行后序遍历可以得到指令表。
 *https://max.book118.com/html/2018/0829/8032072141001121.shtm
 * 指令表转梯形图：
 *
 *
 *
 *
 * 步骤：
 * 1.检查是否有错误，语法逻辑检测，短路i，断路，逻辑错误
 * 2.编译：梯形图->二叉树->指令表
 * 3.指令表的修正优化
 * http://www.docin.com/p-638846728.html

*/


//https://www.xzbu.com/8/view-8721464.htm
#include <QObject>
#include <QDebug>
enum BTNodePos
{
  Any,
  Left,
  Right,
};
enum BTNodeTyep
{
  SerialNode,
  ParallelNode,
  LeafNode,
  OutNode,
};

class BTreeNode
{
public:
    int value;
    BTNodeTyep type;
    BTreeNode* left;   //左子结点
    BTreeNode* right;  //右子结点
    BTreeNode* parent; //父节点
public:

    BTreeNode();             //无参构造
    BTreeNode(const int &data,
              BTNodeTyep type=LeafNode,
              BTreeNode* pleft=NULL,
              BTreeNode* pright=NULL);
    ~BTreeNode();

    BTreeNode &operator =(const BTreeNode &copy);

    BTreeNode* root();

    BTreeNode* find(BTreeNode* node, const int& pos);
    BTreeNode* find(const int& pos);
    BTreeNode* find(BTreeNode* node, BTreeNode* obj);
    BTreeNode* find(BTreeNode* node);

    BTreeNode* find(const int &value, BTNodeTyep type=LeafNode);

    bool hasNode(BTreeNode* node);
    static bool insert(BTreeNode* newnode, BTreeNode* node, BTNodePos pos=Any);
    bool insert(BTreeNode* node, BTNodePos pos=Any);

    bool insert(const int& value, BTreeNode* parent, BTNodePos pos=Any);

    void free(BTreeNode* node);
    void clear();

    void print(BTreeNode* node);
    void printStr(BTreeNode *node);
};






#endif // BINARYTREE_H
