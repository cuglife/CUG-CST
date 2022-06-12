#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <QDebug>
#include <QString>
#include <QStack>
#include <QQueue>
#include <cmath>
#include <vector>

template <typename T>
struct BinTreeNode{
    T data;
    BinTreeNode<T> *leftChild;
    BinTreeNode<T> *rightChild;
    int num;
    BinTreeNode():leftChild(NULL), rightChild(NULL){}
    BinTreeNode(T x, BinTreeNode<T> *l = NULL, BinTreeNode<T> *r = NULL)
        :leftChild(l), rightChild(r){
        data = x;
    }
};

template <typename T>
class BinaryTree{

public:
    BinaryTree():root(nullptr){}
    BinaryTree(T value):root(NULL){RefValue = value;}
    ~BinaryTree(){destroy(root);}


    BinTreeNode<T> *LeftChild(BinTreeNode<T> *t){return (t != NULL)?t->leftChild:NULL;}
    BinTreeNode<T> *RightChild(BinTreeNode<T> *t){return (t != NULL)?t->rightChild:NULL;}
    BinTreeNode<T> *getRoot()const{return root;}

    bool IsEmpty(){return root == NULL;}

    void setNum();

    void destroy(BinTreeNode<T> *& subTree) ;


    BinTreeNode<T> *root;		//二叉树的根指针
    T RefValue;					//数据输入停止标志
};

//清除子二叉树
template <typename T>
void BinaryTree<T>::destroy(BinTreeNode<T> *& subTree)
{
    if (subTree != NULL)	{
        destroy (subTree->leftChild);
        destroy (subTree->rightChild);
        delete subTree;
    }
}

//确定每个节点的位置
template <typename T>
void BinaryTree<T>::setNum()
{
    BinTreeNode<QString> *p = root;
     if (p == nullptr)
         return;
     QQueue<BinTreeNode<QString> * > Q;
     p->num=1;
     Q.enqueue(p);
     while (!Q.isEmpty()) {
         p=Q.dequeue();
         if (p->leftChild != nullptr) {
             p->leftChild->num=2*p->num;
             Q.enqueue(p->leftChild);
         }
         if (p->rightChild != nullptr) {
             p->rightChild->num=2*p->num+1;
             Q.enqueue(p->rightChild);
         }
     }
}

#endif // BINARYTREE_H
