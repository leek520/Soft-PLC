#include "binarytree.h"

template <typename T>
BTreeNode<T>::BTreeNode()
{
    left = NULL;
    right = NULL;
    parent = NULL;
}

template <typename T>
BTreeNode<T>::BTreeNode(const T &value, BTreeNode<T> *pleft, BTreeNode<T> *pright)
{
    data = value;
    left = pleft;
    right = pright;
    parent = NULL;
}

template <typename T>
BTreeNode<T> &BTreeNode<T>::operator =(const BTreeNode<T> &copy)
{
    data = copy.data;
    left = copy.left;
    right = copy.right;
    parent = copy.parent;
}


template <typename T>
BTree<T>::BTree(const T &value)
{
    root = new BTreeNode<T>(value);
}
template <typename T>
BTree<T>::BTree(BTreeNode<T> *r)
{
    root = r;
}

template <typename T>
BTree<T>::~BTree()
{

}
