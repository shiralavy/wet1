#ifndef AVLTREE_H
#define AVLTREE_H

// #include <iostream>
#include <stdexcept>
// #include <memory>
using namespace std;

typedef enum
{
    AVL_SUCCESS,
    AVL_NODE_ALREADY_EXISTS,
    AVL_NODE_DOES_NOT_EXIST,
    AVL_NULL_ARGUMENT,
    AVL_BAD_ALLOCATION,
} AVLResult;

template <class T>
class Node
{
public:
    int m_key;
    int m_height;
    T m_data_element;
    Node<T> *m_left_son;
    Node<T> *m_right_son;

    bool operator==(const Node &other) const;
    Node(const T &data_element, int key = -1, int height = 0, Node<T> *left_son = nullptr,
         Node<T> *right_son = nullptr);
    ~Node();
    bool Node<T>::operator==(const Node &other) const;
};

/**************Node functions*****************/
template <class T>
bool Node<T>::operator==(const Node &other) const
{
    return m_key == other.m_key
}

template <class T>
Node<T>::Node(const T &data_element, int key, int height, Node<T> *left_son,
              Node<T> *right_son) : m_data_element(data_element), m_key(key), m_height(height), m_left_son(left_son), m_right_son(right_son) {}

template <class T>
Node<T>::~Node()
{
    delete m_data_element;
    delete m_left_son;
    delete m_right_son;
}

/**************end of Node functions*******/

template <class T>
class AVLtree
{
public:
    Node<T> *m_root;
    int m_highest_key;
    int m_lowest_key;

    AVLtree() : m_root(nullptr), m_highest_key(0), m_lowest_key(0){};
    ~AVLtree();
    Node<T> *clearTree(Node<T> *current_root);
    int calcHeight(Node<T> *m_node);
    int getBalance(Node<T> *m_node);
    Node<T> *findNode(Node<T> *root, int key);
    Node<T> *insertNode(Node<T> *root, int key, const T &data_element);
    Node<T> *RotateLL(Node<T> *node);
    Node<T> *RotateRR(Node<T> *node);
    Node<T> *RotateRL(Node<T> *node);
    Node<T> *RotateLR(Node<T> *node);
    Node<T> *deleteNode(Node<T> *node, int key);
    Node<T> *AVLtree<T>::minValueNode(Node<T> *node);
    int inOrderVisit(Node<T> *node, int *array, int array_size);

    int max(int a, int b)
    {
        return (a > b) ? a : b;
    }
};

/**************AVLtree functions***************/
template <class T>
AVLtree<T>::~AVLtree()
{
    m_root = clearTree(m_root);
    delete m_root;
}

template <class T>
Node<T> *AVLtree<T>::clearTree(Node<T> *current_root)
{
    if (current_root)
    {
        current_root->m_left_son = clearTree(current_root->m_left_son);
        current_root->m_right_son = clearTree(current_root->m_right_son);
        delete current_root;
    }
    return nullptr;
}

template <class T>
int AVLtree<T>::calcHeight(Node<T> *node)
{
    if (!node)
    {
        return -1;
    }
    int h = 0;
    else
    {
        if (node->m_left_son && node->m_right_son)
        {
            h = 1 + max(node->m_left_son->m_height, node->m_right_son->m_height);
        }
        else if (!node->m_left_son && node->m_right_son)
        {
            h = 1 + node->m_right_son->m_height;
        }
        else if (node->m_left_son && !node->m_right_son)
        {
            h = 1 + node->m_left_son->m_height;
        }
    }
    return h;
}

template <class T>
int AVLtree<T>::getBalance(Node<T> *node)
{
    if (!node)
    {
        return 0;
    }
    return calcHeight(node->m_left_son) - calcHeight(node->m_right_son);
}

template <class T>
Node<T> *AVLtree<T>::findNode(Node<T> *root, int key)
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
Node<T> *AVLtree<T>::insertNode(Node<T> *root, int key, const T &data_element)
{
    if (root == nullptr)
    {
        root = new Node<T>(T, key);
        if (m_highest_key < key)
        {
            m_highest_key = key;
        }
        if (m_lowest_key < key)
        {
            m_lowest_key = key;
        }
        return root;
    }
    if (key < root->m_key)
        root->m_left_son = insertNode(root->m_left_son, key, data_element);
    else if (key > root->m_key)
        root->m_right_son = insertNode(root->m_right_son, key, data_element);

    if (key < this->m_lowest_key)
    {
        this->m_lowest_key = key;
    }
    else if (key > this->m_highest_key)
    {
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

template <class T> // change names var
Node<T> *AVLtree<T>::RotateLL(Node<T> *node)
{
    if (node == nullptr)
    {
        return nullptr;
    }
    Node<T> *temp;
    Node<T> *new_root;
    temp = node;
    new_root = temp->m_left_son;
    temp->m_left_son = new_root->m_right_son;
    new_root->m_right_son = temp;
    new_root->m_right_son->m_height = 1 + max(calcHeight(new_root->m_right_son->m_left_son),
                                              calcHeight(new_root->m_right_son->m_right_son));
    new_root->m_height = 1 + max(calcHeight(new_root->m_left_son),
                                 calcHeight(new_root->m_right_son));
    return new_root;
}

template <class T>
Node<T> *AVLtree<T>::RotateRR(Node<T> *node)
{

    if (node == nullptr)
    {
        return nullptr;
    }
    Node<T> *temp;
    Node<T> *new_root;
    temp = node;
    new_root = temp->m_right_son;

    temp->m_right_son = new_root->m_left_son;
    new_root->m_left_son = temp;

    new_root->m_left_son->m_height = 1 + max(calcHeight(new_root->m_left_son->m_left_son),
                                             calcHeight(new_root->m_left_son->m_right_son));
    new_root->m_height = 1 + max(calcHeight(new_root->m_left_son),
                                 calcHeight(new_root->m_right_son));
    return new_root;
}

template <class T> // need to finish rotate
Node<T> *AVLtree<T>::RotateLR(Node<T> *node)
{
    node->m_left_son = RotateRR(node->m_left_son);
    node->m_left_son->m_height = 1 + max(calcHeight(node->m_left_son->m_left_son),
                                         calcHeight(node->m_left_son->m_right_son));
    Node<T> *new_root = RotateLL(node);
    new_root->m_height = 1 + max(calcHeight(new_root->m_left_son),
                                 calcHeight(new_root->m_right_son));
    return new_root;
}

template <class T>
Node<T> *AVLtree<T>::RotateRL(Node<T> *node)
{
    node->m_right_son = RotateLL(node->m_right_son); // fix this functio jonah
    node->m_right_son->m_height = 1 + max(calcHeight(node->m_right_son->m_left_son),
                                          calcHeight(node->m_right_son->m_right_son));
    Node<T> *new_root = RotateRR(node);
    new_root->m_height = 1 + max(calcHeight(new_root->m_left_son),
                                 calcHeight(new_root->m_right_son));
    return new_root;
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

// infuse geekforgeeks with jonah

template <class T>
Node<T> *AVLtree<T>::minValueNode(Node<T> *node)
{
    shared_ptr<Node<T>> curr = node;

    while (curr->m_left_son != nullptr)
        curr = curr->m_left_son;

    return curr;
}

template <class T>
Node<T> *AVLtree<T>::deleteNode(Node<T> *root, int key) // add a check for the best player
{
    if (root == nullptr)
    {
        return nullptr;
    }
    if (key < root->m_key)
    {
        root->m_left_son = deleteNode(root->m_left_son, key);
    }
    else if (key > root->m_key)
    {
        root->m_right_son = deleteNode(root->m_right_son, key);
    }
    else
    { // this means the key is equal to the current node's key
        if (root->m_left_son && root->m_right_son)
        { // two children
            // we find the next node that should be the new root - this is the node with the samllest value in the right son tree
            Node<T> *new_root = minValueNode(root->m_right_son);
            // we copy the data element and the key to the root, but keep the height and pointers to the next sons.
            // this means we changed the value of the root node without actually deleting the root
            root->m_key = new_root->m_key;
            root->m_data_element = new_root->m_data_element;
            // now we delete the original node that was copied to become the root, we dont need it anymore because we copied it's values
            root->m_right_node = deleteNode(root->m_right_son, root->m_key);
        }
        else
        { // one or zero children
            Node<T> *temp = root;
            if (temp->m_left_son == nullptr)
            {
                root = root->m_right_son;
            }
            else if (temp->m_right_son == null)
            {
                root = root->m_left_son
            }
            delete temp;
        }
        root->m_height = 1 + max(calcHeight(root->m_left_son),
                                 calcHeight(root->m_right_son));
    }
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

    /*
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
    */
}

template <class T>
int AVLtree<T>::inOrderVisit(Node<T> *node, int *array, int start_index)
{
    if (!node)
    {
        return start_index;
    }
    else
    {
        int index_after_left_visit = inOrderVisit(node->m_left_son, array, start_index);
        array[index_after_left_visit] = node->m_key;
        int index_after_right_visit = inOrderVisit(node->m_right_son, array, index_after_left_visit + 1);
        return index_after_right_visit;
    }
}

/**************end of AVLtree functions***************/

#endif
