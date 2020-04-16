/* Created By: Jack Easton
 * Date: 4/7/2020 */

#ifndef AVLTREE_AVLNODE_H
#define AVLTREE_AVLNODE_H

#include <iostream>
using namespace std;

template <typename T>
struct AVLNode
{
    template <typename U> friend class AVLTree;

    //data members
    T data;
    int height;
    AVLNode *left, *right;

    //constructors/destructor
    AVLNode(const T &);
    AVLNode(const AVLNode &);
    ~AVLNode();

    //overloaded operators
    AVLNode & operator=(const AVLNode &);
};

/* ------------------------------------------------- Implementation ------------------------------------------------- */

template <typename T>
AVLNode<T>::AVLNode(const T &val)
        : data(val), left(nullptr), right(nullptr), height(0) {}

template <typename T>
AVLNode<T>::AVLNode(const AVLNode &other) : data(other.data), height(other.height), left(nullptr), right(nullptr)
{
    if (other.left != nullptr)
        left = new AVLNode (*other.left);
    if (other.right != nullptr)
        right = new AVLNode (*other.right);
}

template <typename T>
AVLNode<T>::~AVLNode()
{
    delete left;
    delete right;
}

template<typename T>
AVLNode<T> & AVLNode<T>::operator=(const AVLNode &rhs)
{
    data = rhs.data;

    if (rhs.left == nullptr)
        left = nullptr;
    else {
        AVLNode *origLeft = left;
        left = new AVLNode(*rhs.left);
        delete origLeft;
    }

    if (rhs.right == nullptr)
        right = nullptr;
    else {
        AVLNode *origRight = right;
        right = new AVLNode(*rhs.right);
        delete origRight;
    }

    return *this;
}

#endif //AVLTREE_AVLNODE_H
