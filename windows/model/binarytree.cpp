#include "binarytree.h"


BTreeNode::BTreeNode()
{
    left = NULL;
    right = NULL;
    parent = NULL;
}


BTreeNode::BTreeNode(const int &data, BTreeNode *pleft, BTreeNode *pright)
{
    value = data;
    left = pleft;
    right = pright;
    parent = NULL;
}

BTreeNode::~BTreeNode()
{

}


BTreeNode &BTreeNode::operator =(const BTreeNode &copy)
{
    value = copy.value;
    left = copy.left;
    right = copy.right;
    parent = copy.parent;
}



BTreeNode *BTreeNode::root() const
{
    if (this->parent == NULL){
        return this->parent;
    }else{
        this->root();
    }
}

BTreeNode *BTreeNode::find(BTreeNode *node, const int &value) const
{
    BTreeNode* ret = NULL;
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

BTreeNode *BTreeNode::find(const int &value) const
{
    return find(root(), value);
}

BTreeNode *BTreeNode::find(BTreeNode *node, BTreeNode *obj) const
{
    BTreeNode* ret = NULL;
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

BTreeNode *BTreeNode::find(BTreeNode *node) const
{
    return find(this->root(), node);
}
/******************************************************************************
* @brief: 在node节点的指定位置插入新节点
* @author:leek
* @date 2018/10/10
*******************************************************************************/

bool BTreeNode::insert(BTreeNode *newnode, BTreeNode *node, BTNodePos pos)
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

bool BTreeNode::insert(BTreeNode *node, BTNodePos pos)
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
           BTreeNode* np = find(node->parent);
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

bool BTreeNode::insert(const int &value, BTreeNode *parent, BTNodePos pos)
{
    bool ret = true;
    BTreeNode* node = new BTreeNode();
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


void BTreeNode::free(BTreeNode *node)
{
    if(node != NULL)
    {
        free(node->left);
        free(node->right);
    }
    delete node;
}

void BTreeNode::clear()
{
    free(root());
    this->parent = NULL;
}
