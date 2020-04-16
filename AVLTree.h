/* Created By: Jack Easton
 * Date: 4/7/2020 */

#ifndef AVLTREE_AVLTREE_H
#define AVLTREE_AVLTREE_H

#include <cmath>
#include <fstream>

#include "AVLNode.h"

template <typename T>
class AVLTree
{
private:
    AVLNode<T> *root;
    int numNodes;
public:
    /* constructors/destructor */
    AVLTree();
    AVLTree(const AVLTree &);
    ~AVLTree();

    /* overloaded operators */
    AVLTree & operator=(const AVLTree &);

    /* AVL insertion public interface */
    void insert (const T &val) { insert(val, root); numNodes++;}
    /**/
    void clear();
    /**/
    bool contains(const T &);
    /**/
    AVLNode<T> * find(const T &);

    //getters
    AVLNode<T> * getRoot() { return root; }
    int getNumNodes() { return numNodes; }

    //print public interface
    void print();
    /* write contents of tree to file (public interface)*/
    void writeToFile(const char *);

private:
    /* returns the height of the sub-tree
     * takes the root of that sub-tree as argument */
    int height(AVLNode<T> *ptr) { return ptr == nullptr ? -1 : ptr->height; }

    /* recursive in-order traversal of a separate AVLTree and inserts its values to this AVLTree */
    void inOrderCopy(AVLNode<T> *);

    /* AVL insertion
     * case 1, case 2, case 3, case 4 (in order below) */
    void insert (const T &, AVLNode<T> *&);
    void rotateLeft(AVLNode<T> *&);
    void doubleLeft(AVLNode<T> *&);
    void doubleRight(AVLNode<T> *&);
    void rotateRight(AVLNode<T> *&);

    /**/
    AVLNode<T> * find(const T &, AVLNode<T> *&);

    //recursive print
    void print(AVLNode<T> *, int);

    //recursive write to file
    void writeToFile(AVLNode<T> *, ofstream &);
};

/* ------------------------------------------------- Implementation ------------------------------------------------- */

template<typename T>
AVLTree<T>::AVLTree()
{
    root = nullptr;
    numNodes = 0;
}

template<typename T>
AVLTree<T>::AVLTree(const AVLTree &other)
{
    if (other.root == nullptr)
        root = nullptr;
    else
        inOrderCopy(other.root);

    numNodes = other.numNodes;
}

template<typename T>
AVLTree<T>::~AVLTree()
{
    delete root;
}

template<typename T>
AVLTree<T> &AVLTree<T>::operator=(const AVLTree &rhs) {
    if (rhs.root == nullptr)
        root = nullptr;
    else
        inOrderCopy(rhs.root);

    numNodes = rhs.numNodes;

    return *this;
}

template<typename T>
void AVLTree<T>::clear() {
    delete root;
    root = nullptr;

    numNodes = 0;
}

template<typename T>
AVLNode<T> *AVLTree<T>::find(const T &element) {
    return find(element, root);
}

template<typename T>
bool AVLTree<T>::contains(const T &element) {
    return find(element) != nullptr;
}

template<typename T>
void AVLTree<T>::insert(const T &val, AVLNode<T> *&curr)
{
    if (curr == nullptr)
        curr = new AVLNode<T>(val);
    else if (val < curr->data) {
        insert(val, curr->left);

        //check if tree is unbalanced
        if (height(curr->left) - height(curr->right) == 2) {
            //find which case to use
            if (val < curr->left->data) //LL
                rotateLeft(curr);
            else if (val > curr->left->data) //RL
                doubleLeft(curr);
        }
    }
    else { //val inserted to right if greater then or equal to (duplicate) current val
        insert(val, curr->right);

        //check if tree is unbalanced
        if (height(curr->right) - height(curr->left) == 2) {
            if (val > curr->right->data) //RR
                rotateRight(curr);
            else //LR
                doubleRight(curr);
        }
    }
    curr->height = fmax( height(curr->left), height(curr->right) ) + 1;
}

template<typename T>
void AVLTree<T>::rotateLeft(AVLNode<T> *&alpha)
{
    //rotate
    AVLNode<T> *leftChild = alpha->left;
    alpha->left = leftChild->right;
    leftChild->right = alpha;

    //adjust heights
    alpha->height = fmax( height(alpha->left), height(alpha->right) ) + 1;
    leftChild->height = fmax( height(leftChild->left), height(leftChild->right) ) + 1;

    //put leftChild in alpha's old position
    alpha = leftChild;
}

template<typename T>
void AVLTree<T>::doubleLeft(AVLNode<T> *&alpha)
{
    rotateRight(alpha->left);
    rotateLeft(alpha);
}

template<typename T>
void AVLTree<T>::doubleRight(AVLNode<T> *&alpha)
{
    rotateLeft(alpha->right);
    rotateRight(alpha);
}

template<typename T>
void AVLTree<T>::rotateRight(AVLNode<T> *&alpha)
{
    //rotate
    AVLNode<T> *rightChild = alpha->right;
    alpha->right = rightChild->left;
    rightChild->left = alpha;

    //adjust heights
    alpha->height = fmax( height(alpha->left), height(alpha->right) ) + 1;
    rightChild->height = fmax( height(rightChild->left), height(rightChild->right) ) + 1;

    //put leftChild in alpha's old position
    alpha = rightChild;
}

template<typename T>
void AVLTree<T>::inOrderCopy(AVLNode<T> *otherCurr)
{
    if (otherCurr != nullptr) {
        //left sub-tree
        inOrderCopy(otherCurr->left);

        //deep copy
        insert(otherCurr->data, root);

        //right sub-tree
        inOrderCopy(otherCurr->right);
    }
}

template<typename T>
AVLNode<T> *AVLTree<T>::find(const T &element, AVLNode<T> *&curr) {
    if (curr == nullptr)
        return nullptr;
    else if (element < curr->data)
        return find(element, curr->left);
    else if (element > curr->data)
        return find(element, curr->right);
    else // element == curr->data
        return curr;
}

template<typename T>
void AVLTree<T>::print() {
    print(root, 0);
}

template<typename T>
void AVLTree<T>::writeToFile(const char *fileName) {
    ofstream file(fileName);
    if (!file) {
        cout << "could not open AVLTree output file" << endl;
        exit(1);
    }

    writeToFile(root, file);

    file.close();
}

template<typename T>
void AVLTree<T>::print(AVLNode<T> *curr, int numTabs) {
    if (curr == nullptr)
        return;

    print(curr->right, numTabs + 1);

    cout << endl;
    for (int i = 0; i < numTabs; i++)
        cout << "\t";
    cout << curr->data << endl;

    print(curr->left, numTabs + 1);
}

template<typename T>
void AVLTree<T>::writeToFile(AVLNode<T> *curr, ofstream &file)
{
    //PRE-ORDER traversal
    if (curr != nullptr) {
        //visit current node
        file << curr->data << endl;

        //recurse to other nodes
        writeToFile(curr->left, file);
        writeToFile(curr->right, file);
    }
}

#endif //AVLTREE_AVLTREE_H
