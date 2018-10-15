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

//http://blog.51cto.com/9291927/2083190
//https://www.cnblogs.com/ljwTiey/p/4284784.html

#include <QObject>
template <typename T>
class BTreeNode
{
public:
    T data;
    BTreeNode<T>* left;   //左子结点
    BTreeNode<T>* right;  //右子结点
    BTreeNode<T>* parent;   //父节点

    BTreeNode();
    BTreeNode(const T &value,
              BTreeNode<T>* pleft=NULL,
              BTreeNode<T>* pright=NULL);

    BTreeNode<T> &operator =(const BTreeNode<T> &copy);
};


template <typename T>
class BTree
{
protected:
    BTreeNode<T>* root;   //根节点

public:

    BTree(){ root = NULL; }     //无参构造
    BTree(const T &value);      //建立以value元素为值的二叉树
    BTree(BTreeNode<T>* r);      //建立以r为根的二叉树
    ~BTree();

};



#endif // BINARYTREE_H
