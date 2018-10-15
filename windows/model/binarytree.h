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
#include <QObject>
enum BTNodePos
{
    Any,
    Left,
    Right
};
template <typename T>
class TreeNode
{
public:
    T value;
    TreeNode<T>* parent;
    TreeNode()
    {
      parent = NULL;
    }
    virtual ~TreeNode() = 0;
};
template <typename T>
class Tree:public Object
{
protected:
    TreeNode<T>* m_root;//根结点
public:
Tree(){m_root = NULL;}
//插入结点
virtual bool insert(TreeNode<T>* node) = 0;
virtual bool insert(const T& value, TreeNode<T>* parent) = 0;
//删除结点
virtual QSharedPointer< Tree<T> > remove(const T& value) = 0;
virtual QSharedPointer< Tree<T> > remove(TreeNode<T>* node) = 0;
//查找结点
virtual TreeNode<T>* find(const T& value)const = 0;
virtual TreeNode<T>* find(TreeNode<T>* node)const = 0;
//根结点访问函数
virtual TreeNode<T>* root()const = 0;
//树的度访问函数
virtual int degree()const = 0;
//树的高度访问函数
virtual int height()const = 0;
//树的结点数目访问函数
virtual int count()const = 0;
//清空树
virtual void clear() = 0;
};
template <typename T>
TreeNode<T>::~TreeNode()
{
}
template <typename T>
class BTreeNode:public TreeNode<T>
{
public:
    BTreeNode<T>* m_left;//左子结点
    BTreeNode<T>* m_right;//右子结点
    BTreeNode()
    {
        m_left = NULL;
        m_right = NULL;
    }

    //工厂方法，创建堆空间的结点
    static BTreeNode<T>* NewNode()
    {
      BTreeNode<T>* ret = new BTreeNode<T>();
      if(ret != NULL)
      {
          //堆空间的结点标识为true
          ret->m_flag = true;
      }
      return ret;
    }
    virtual BTreeNode<T>* find(BTreeNode<T>* node, const T& value) const;
    BTreeNode<T>* find(const T& value) const;

    virtual bool insert(BTreeNode<T>* newnode, BTreeNode<T>* node, BTNodePos pos);
    bool insert(TreeNode<T>* node);
    virtual bool insert(const T& value, TreeNode<T>* parent, BTNodePos pos);

};
template <typename T>
BTreeNode<T> *BTreeNode<T>::find(BTreeNode<T> *node, const T &value) const
{
    BTreeNode<T>* ret = NULL;
    //如果根节点node
    if(node != NULL)
    {
        if(node->value == value)
        {
            ret = node;
        }
        else
        {
            //查找左子树
            if(ret == NULL)
            {
                ret = find(node->m_left, value);
            }
            //如果左子树没有找到，ret返回NULL，查找右子树
            if(ret == NULL)
            {
                ret = find(node->m_right, value);
            }
        }
    }
    return ret;
}
template <typename T>
BTreeNode<T> *BTreeNode<T>::find(const T &value) const
{
    return find(root(), value);
}
template <typename T>
bool BTreeNode<T>::insert(BTreeNode<T> *newnode, BTreeNode<T> *node, BTNodePos pos)
{

}
template <typename T>
bool BTreeNode<T>::insert(TreeNode<T> *node)
{

}
template <typename T>
bool BTreeNode<T>::insert(const T &value, TreeNode<T> *parent, BTNodePos pos)
{

}
#endif // BINARYTREE_H
