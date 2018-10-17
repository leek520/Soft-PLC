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

enum BTNodePos
{
  Any,
  Left,
  Right
};

template <typename T>
class BTreeNode
{
protected:
    T value;
    BTreeNode<T>* left;   //左子结点
    BTreeNode<T>* right;  //右子结点
    BTreeNode<T>* parent; //父节点
public:

    BTreeNode();             //无参构造
    BTreeNode(const T &data,
              BTreeNode<T>* pleft=NULL,
              BTreeNode<T>* pright=NULL);
    ~BTreeNode();

    BTreeNode<T> &operator =(const BTreeNode<T> &copy);

    BTreeNode<T>* root()const;

    BTreeNode<T>* find(BTreeNode<T>* node, const T& value)const;
    BTreeNode<T>* find(const T& value)const;
    BTreeNode<T>* find(BTreeNode<T>* node, BTreeNode<T>* obj)const;
    BTreeNode<T>* find(BTreeNode<T>* node)const;


    bool insert(BTreeNode<T>* newnode, BTreeNode<T>* node, BTNodePos pos=Any);
    bool insert(BTreeNode<T>* node, BTNodePos pos=Any);

    bool insert(const T& value, BTreeNode<T>* parent, BTNodePos pos=Any);

    void free(BTreeNode<T>* node);
    void clear();

};

template <typename T>
BTreeNode<T>::BTreeNode()
{
    left = NULL;
    right = NULL;
    parent = NULL;
}

template <typename T>
BTreeNode<T>::BTreeNode(const T &data, BTreeNode<T> *pleft, BTreeNode<T> *pright)
{
    value = data;
    left = pleft;
    right = pright;
    parent = NULL;
}
template <typename T>
BTreeNode<T>::~BTreeNode()
{

}

template <typename T>
BTreeNode<T> &BTreeNode<T>::operator =(const BTreeNode<T> &copy)
{
    value = copy.value;
    left = copy.left;
    right = copy.right;
    parent = copy.parent;
}


template <typename T>
BTreeNode<T> *BTreeNode<T>::root() const
{
    if (this->parent == NULL){
        return this->parent;
    }else{
        this->root();
    }
}
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
                ret = find(node->left, value);
            }
            //如果左子树没有找到，ret返回NULL，查找右子树
            if(ret == NULL)
            {
                ret = find(node->right, value);
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
BTreeNode<T> *BTreeNode<T>::find(BTreeNode<T> *node, BTreeNode<T> *obj) const
{
    BTreeNode<T>* ret = NULL;
    if(node != NULL)
    {
        //根节点node为目标结点
        if(node == obj)
        {
            ret = node;
        }
        else
        {
            //查找左子树
            if(ret == NULL)
            {
                ret = find(node->left, obj);
            }
            //如果左子树没有找到，ret返回NULL，继续查找右子树
            if(ret == NULL)
            {
                ret = find(node->right, obj);
            }
        }
    }
    return ret;
}
template <typename T>
BTreeNode<T> *BTreeNode<T>::find(BTreeNode<T> *node) const
{
    return find(this->root(), node);
}
/******************************************************************************
* @brief: 在node节点的指定位置插入新节点
* @author:leek
* @date 2018/10/10
*******************************************************************************/
template <typename T>
bool BTreeNode<T>::insert(BTreeNode<T> *newnode, BTreeNode<T> *node, BTNodePos pos)
{
    bool ret = true;
    //插入的位置为Any
    if(pos == Any)
    {
        //如果没有左子结点，插入结点作为左子结点
        if(node->left == NULL)
        {
            node->left = newnode;
        }
        //如果有左子结点，没有右子结点，插入结点作为右子结点
        else if(node->right == NULL)
        {
            node->right = newnode;
        }
        //如果node结点的左右子结点不为空，插入失败
        else
        {
            ret = false;
        }
    }
    else if(pos == Left)
    {
        //如果指定插入左子结点，如果没有左子结点，插入结点
        if(node->left == NULL)
        {
            node->left = newnode;
        }
        else
        {
            ret = false;
        }
    }
    else if(pos == Right)
    {
        //如果指定插入右子结点，如果没有右子结点，插入结点
        if(node->right == NULL)
        {
            node->right = newnode;
        }
        else
        {
            ret = false;
        }
    }
    else
    {
        ret = false;
    }
    return ret;
}
/******************************************************************************
* @brief: 在当前节点的父节点的指定位置插入节点
* @author:leek
* @date 2018/10/10
*******************************************************************************/
template <typename T>
bool BTreeNode<T>::insert(BTreeNode<T> *node, BTNodePos pos)
{
    bool ret = true;
    if(node != NULL)
    {
        if(this->parent == NULL)
        {
            node->parent = NULL;
            this->parent = node;
        }
        else
        {
           BTreeNode<T>* np = find(node->parent);
           if(np != NULL)
           {
               ret = insert(node, np, pos);
           }
           else
           {
               //THROW_EXCEPTION(InvalidParameterException, "Parameter invalid...");
           }
        }
    }
    else
    {
        //THROW_EXCEPTION(InvalidParameterException, "Parameter invalid...");
    }
    return ret;

}

/******************************************************************************
* @brief: 在parent的pos位置插入值为value的节点
* @author:leek
* @date 2018/10/10
*******************************************************************************/
template <typename T>
bool BTreeNode<T>::insert(const T &value, BTreeNode<T> *parent, BTNodePos pos)
{
    bool ret = true;
    BTreeNode<T>* node = new BTreeNode<T>();
    if(node != NULL)
    {
        node->parent = parent;
        node->value = value;
        ret = insert(node, pos);
        if(!ret)
        {
            delete node;
        }
    }
    else
    {
        //THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory...");
    }

    return ret;

}

template <typename T>
void BTreeNode<T>::free(BTreeNode<T> *node)
{
    if(node != NULL)
    {
        free(node->left);
        free(node->right);
    }
    delete node;
}
template <typename T>
void BTreeNode<T>::clear()
{
    free(root());
    this->parent = NULL;
}



#endif // BINARYTREE_H
