#ifndef AVLTREE_H
#define AVLTREE_H


//#include <iostream>
#include <stdexcept>
#include <memory>
using namespace std;

typedef enum {
    AVL_SUCCESS,
    AVL_NODE_ALREADY_EXISTS,
    AVL_NODE_DOES_NOT_EXIST,
    AVL_NULL_ARGUMENT,
    AVL_BAD_ALLOCATION,
} AVLResult;


template <class T>
class Node {
    int m_key;
    int m_height;
    shared_ptr<T> m_data_element;
    shared_ptr<Node<T>> m_left_son;
    shared_ptr<Node<T>> m_right_son;

    bool operator==(const Node& other) const;
 Node(int key = -1, shared_ptr<T> data_element = nullptr, int height = 0, shared_ptr<Node<T>> left_son = nullptr,
     shared_ptr<Node<T>> right_son = nullptr); 
 ~Node();
  bool Node<T>::operator==(const Node& other) const;  
};

/**************Node functions*****************/
template<class T>
bool Node<T>::operator==(const Node& other)const {
    return m_key == other.m_key
}

template <class T>
Node<T>::Node(int key, shared_ptr<T> data_element, int height, shared_ptr<Node<T>> left_son,
         shared_ptr<Node<T>> right_son) :
        m_key(key), m_data_element(element), m_height(height), m_left_son(left_son), m_right_son(right_son) {}

template <class T>
Node<T>::~Node()
{
    delete m_data_element;
}

/**************end of Node functions*******/

template <class T>
class AVLtree {
public:
    shared_ptr<Node<T>> m_root;
    int m_highest_key;
    int m_lowest_key;
    
    AVLtree() : m_root(nullptr), m_highest_key(0), m_lowest_key(0){};

    AVLtree() : m_root(nullptr), m_highest_key(0), m_lowest_key(0) {};
    //AVLtree(shared_ptr<Node<T>> root) : m_root(root) {};
    ~AVLtree() = default; //is this default or do we need to implement?
    int calcHeight(shared_ptr<Node<T>> m_node);
    int getBalance(shared_ptr<Node<T>> m_node);
    shared_ptr<Node<T>> findNode(shared_ptr<Node<T>> root, int key);
    shared_ptr<Node<T>> insertNode(shared_ptr<Node<T>> root, int key, shared_ptr<T> data_element);
    shared_ptr<Node<T>> RotateLL(shared_ptr<Node<T>> node);
    shared_ptr<Node<T>> RotateRR(shared_ptr<Node<T>> node);
    shared_ptr<Node<T>> RotateRL(shared_ptr<Node<T>> node);
    shared_ptr<Node<T>> RotateLR(shared_ptr<Node<T>> node);
    shared_ptr<Node<T>> deleteNode(shared_ptr<Node<T>> node, int key);
    shared_ptr<Node<T>> AVLtree<T>::minValueNode(shared_ptr<Node<T>> node);
    int inOrderVisit(shared_ptr<Node<T>> node, int* array, int array_size);


    int max(int a, int b)
    {
        return (a > b) ? a : b;
    }
};


/**************AVLtree functions***************/
template <class T>
int AVLtree<T>::calcHeight(shared_ptr<Node<T>> node) {
    if (!node) {
        return -1;
    }
    int h = 0;
    else {
        if (node->m_left_son && node->m_right_son) {
            h = 1 + max(node->m_left_son->m_height, node->m_right_son->m_height);
        }
        else if (!node->m_left_son && node->m_right_son) {
            h = 1 + node->m_right_son->m_height;
        }
        else if (node->m_left_son && !node->m_right_son) {
            h = 1 + node->m_left_son->m_height;
        }
    }
    return h;
}

template <class T>
int AVLtree<T>::getBalance(shared_ptr<Node<T>> node) {
    if (!node) {
        return 0;
    }
    return calcHeight(node->m_left_son) - calcHeight(node->m_right_son);

}

template <class T>
shared_ptr<Node<T>> AVLtree<T>::findNode(shared_ptr<Node<T>> root, int key)
{
    if (key <= 0)
    {
        return nullptr;
    }
    if (!root)
    {
        return nullptr;
    }
    if (root->m_key == key)
    {
        return root;
    }
    if (key < root->m_key)
    {
        return findNode(root->m_left_son, key);
    }
    if (key > root->m_key)
    {
        return findNode(root->m_right_son, key);
    }
}

template <class T>
shared_ptr<Node<T>> AVLtree<T>::insertNode(shared_ptr<Node<T>> root, int key, shared_ptr<T> data_element)
{
    if (root == nullptr)
    {
        root = make_shared<Node<T>>(key, data_element); //this means we need a constructor that recives variables for each data type
        return root;
    }
    else {
        if (findNode(this->m_root, key) != nullptr) {
            return root;
        }
    }
    if (key < root->m_key)
        root->m_left_son = insertNode(root->m_left_son, key, data_element);
    else if (key > root->m_key)
        root->m_right_son = insertNode(root->m_right_son, key, data_element);

    if (key < this->m_lowest_key) {
        this->m_lowest_key = key;
    }
    else if (key > this->m_highest_key) {
        this->m_highest_key = key;
    }
    root->m_height = 1 + max(calcHeight(root->m_left_son),
        calcHeight(root->m_right_son));


    int balance_factor = getBalance(root);
    if (balance_factor == 2 && getBalance(node->m_left_son) == 1)
    {
        // LL ROTATION
        return RotateLL(root)
    }
    else if (balance_factor == -2 && getBalance(node->m_right_son) == -1)
    {
        // RR ROTATION
        return RotateRR(root);
    }
    else if (balance_factor == -2 && getBalance(node->m_right_son) == 1)
    {
        // RL ROTATION
        return RotateRL(root);
    }
    else if (balance_factor == 2 && getBalance(node->m_left_son) == -1)
    {
        // LR ROTATION
        return RotateLR(root);
    }
    return root;
}



template <class T>//change names var
shared_ptr<Node<T>> AVLtree<T>::RotateLL(shared_ptr<Node<T>> node)
{
    if (node == nullptr)
    {
        return  nullptr;
    }
    shared_ptr<Node<T>> temp;
    shared_ptr<Node<T>> tp;
    temp = node;
    tp = temp->m_left_son;
    temp->m_left_son = tp->m_right_son;
    tp->m_right_son = temp;
    return tp;
}


template <class T>
shared_ptr<Node<T>> AVLtree<T>::RotateRR(shared_ptr<Node<T>> node)
{

    if (node == nullptr)
    {
        return nullptr;
    }
    shared_ptr<Team> temp;
    shared_ptr<Team> tp;
    temp = node;
    tp = temp->m_right_team;

    temp->m_right_team = tp->m_left_team;
    tp->m_left_team = temp;

    return tp;
}


template <class T>// need to finish rotate
shared_ptr<Node<T>> AVLtree<T>::RotateLR(shared_ptr<Node<T>> node)
{
    shared_ptr<Node<T>> res1 = RotateRR(node->m_right_son)
        shared_ptr<Node<T>> res2 = RotateLL(node);
    if (res1 != nullptr && res2 != nullptr)
    {
        return res2;
    }
    else
    {
        return nullptr;
    }
}


template <class T>
shared_ptr<Node<T>> AVLtree<T>::RotateRL(shared_ptr<Node<T>> node)
{
    AVLResults res1 = RotateLL(node->m_left_son)//fix this functio jonah
        AVLResults res2 = RotateRR(node);
    if (res1 != nullptr && res2 != nullptr)
    {
        return res2;
    }
    else
    {
        return nullptr;
    }
}


/*
template<class T>
shared_ptr<Node<T>> AVLtree<T>::minValueNode()
{
    if (m_left_son == nullptr)
    {
        return this;
    }
    return m_left_son->getMin();
}


template<class T>
shared_ptr<Node<T>> AVLtree<T>::deleteNode(shared_ptr<Node<T>> root, int key)//add a check for the best player 
{
    if (root == nullptr)
    {
        return nullptr;
    }
    if (findNode(root, key) == nullptr)//this is if we dont find the node with the key 
    {
        return nullptr;
    }
    if (key == root->key)
    {
        if (root->m_right_sun != nullptr && root->m_left_son != nullptr)
        {
            shared_ptr<Node<T>> next_node = node->m_right_son->minValueNode();
            root->m_key = next_node->m_key;
            root->m_right_son = deleteNode(node->m_right_son, next_node->m_key);
        }
    }
    else
    {
        shared_ptr<Node<T>>tmpNode = root;
        if (root->m_right_son == nullptr)
        {
            root = root->m_right_son;
        }
        if (node!- nullptr)
        {
            root->
        }

    }

}
*/

//infuse geekforgeeks with jonah

template<class T>
shared_ptr<Node<T>> AVLtree<T>::minValueNode(shared_ptr<Node<T>> node)
{
    shared_ptr<Node<T>> curr = node;

    while (curr->m_left_son != nullptr)
        curr = curr->m_left_son;

    return curr;

}

template<class T>
shared_ptr<Node<T>> AVLtree<T>::deleteNode(shared_ptr<Node<T>> root, int key)//add a check for the best player 
{
    if (root == nullptr)
    {
        return nullptr;
    }

    if (findNode(root, key) == nullptr)//this is if we dont find the node with the key 
    {
        return nullptr;
    }

    if (key < root->m_key)
    {
        root->m_left_son = deleteNode(root->m_left_son, key);
        if (getBalance(root) == -2)
        {
            if (getBalance(root->m_right_son) <= 0)//check 
            {
                root = RotateRR(root);
            }
            else
                root = RotateRL(root);
        }
    }
    else if (key > root->m_key)
    {
        root->m_right_son = deleteNode(root->m_right_son, key);
        if (getBalance(node) == 2)
        {
            if (getBalance(root->left) >= 0)
            {
                root = RotateLL(root);
            }
            else
                root = RotateLR(root);
        }
    }
    //this is qustinbole
    else
    {
        if (root->m_left_son == nullptr || root->m_right_son == nullptr)
        {
            shared_ptr<Node<T>> temp;
            if (root->m_left_son)
            {
                temp = root->m_left_son;
            }
            else
            {
                temp = root->m_right_son;
            }

            if (temp == nullptr)
            {
                temp = root;
                root = nullptr;
            }
            else
            {
                root = temp;
            }
        }
        else
        {
            shared_ptr<Node<T>> temp1 = minValueNode(root->m_right_son);
            root->m_key = temp->m_key;
            root->m_right=deleteNode(root->m_right,temp->m_key)


        }

    }

    if (root == nullptr)
    {
        return root;
    }

    root->m_hight = 1 + max(calcHeight(root->m_son_left), calcHeight(root->m_son_right));
    // might be problem ask on the rotate
    int balance = getBalance(root);
    if (balance > 1 && getBalance(root->m_left) >= 0)
    {
        return RotateLL(root);
    }
    if (balance > 1 && getBalance(root->m_left)< 0)
    {
        return RotateLR(root);
    }

    if (balance < -1 && getBalance(root->m_left) <= 0)
    {
        return RotateRR(root);
    }

    if (balance < -1 && getBalance(root->m_left) > 0)
    {
        return RotateRL(root);
    }

    return root;
}




template <class T>
int AVLtree<T>::inOrderVisit(shared_ptr<Node<T>> node, int* array, int start_index) {
    if (!node) {
        return start_index;
    }
    else {
        int index_after_left_visit = inOrderVisit(node->m_left_son, array, start_index);
        array[index_after_left_visit] = node->m_key;
        int index_after_right_visit = inOrderVisit(node->m_right_son, array, index_after_left_visit + 1);
        return index_after_right_visit;
    }
}




/**************end of AVLtree functions***************/

#endif
