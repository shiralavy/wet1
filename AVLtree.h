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
    int m_key1;
    int m_key2;
    int m_key3;
    int m_height;
    T m_element;
    Node<T> *m_parent;
    Node<T> *m_left_son;
    Node<T> *m_right_son;

    bool operator==(const Node &other) const;
    Node(const T &data_element, int key1 = -1, int key2 = -1, int key3 = -1, int height = 0, Node<T> *parent = nullptr, Node<T> *left_son = nullptr,
         Node<T> *right_son = nullptr);
    ~Node();
    bool Node<T>::operator==(const Node &other) const;
};

/**************Node functions*****************/
template <class T>
bool Node<T>::operator==(const Node &other) const
{
    return m_key1 == other.m_key1;
}

template <class T>
Node<T>::Node(const T &data_element, int key1, int key2, int key3, int height, Node<T> *parent, Node<T> *left_son,
              Node<T> *right_son) : m_element(data_element), m_key1(key1), m_key2(key2), m_key3(key3), m_height(height), m_parent(parent), m_left_son(left_son), m_right_son(right_son) {}

template <class T>
Node<T>::~Node()
{
    delete m_left_son;
    delete m_right_son;
    m_parent = nullptr;
    delete m_parent;
}

/**************end of Node functions*******/

template <class T>
class AVLtree
{
public:
    Node<T> *m_root;
    int m_heighest_key1;
    int m_heighest_key2;
    int m_heighest_key3;
    // int m_lowest_key;

    AVLtree() : m_root(nullptr), m_heighest_key1(-1), m_heighest_key2(-1), m_heighest_key3(-1){};
    ~AVLtree();
    Node<T> *clearTree(Node<T> *current_root);
    int calcHeight(Node<T> *m_node);
    int getBalance(Node<T> *m_node);
    Node<T> *findNode(Node<T> *root, int key1, int key2, int key3);
    Node<T> *insertNode(Node<T> *root, int key1, int key2, int key3, const T &data_element);
    Node<T> *RotateLL(Node<T> *node);
    Node<T> *RotateRR(Node<T> *node);
    Node<T> *RotateRL(Node<T> *node);
    Node<T> *RotateLR(Node<T> *node);
    Node<T> *deleteNode(Node<T> *node, int key1, int key2, int key3);
    Node<T> *AVLtree<T>::minValueNode(Node<T> *node);
    int inOrderVisit(Node<T> *node, int *array, int start_index);
    int closestNode(Node<T> *node);

    int max(int a, int b)
    {
        return (a > b) ? a : b;
    }

    int abs(int a, int b)
    {
        if ((a - b) < 0)
        {
            return b - a;
        }
        return a - b;
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
Node<T> *AVLtree<T>::findNode(Node<T> *root, int key1, int key2, int key3)
{
    if (key1 <= 0)
    {
        return nullptr;
    }
    if (!root)
    {
        return nullptr;
    }
    if (root->m_key1 == key1)
    {
        if (root->m_key2 == key2)
        {
            if (root->m_key3 == key3)
            {
                return root;
            }
            else if (root->m_key3 < key3)
            {
                findNode(root->m_right_son, key1, key2, key3);
            }
            else
            {
                findNode(root->m_left_son, key1, key2, key3);
            }
        }
        else if (root->m_key2 < key2)
        {
            findNode(root->m_left_son, key1, key2, key3);
        }
        else
        {
            findNode(root->m_right_son, key1, key2, key3);
        }
    }
    else if (root->m_key1 < key1)
    {
        return findNode(root->m_left_son, key1, key2, key3);
    }
    else
        (root->m_key1 > key1)
        {
            return findNode(root->m_right_son, key1, key2, key3);
        }
}

template <class T>
Node<T> *AVLtree<T>::insertNode(Node<T> *root, int key1, int key2, int key3, const T &data_element)
{
    if (root == nullptr)
    {
        root = new Node<T>(T, key1, key2, key3);
        if (this->m_heighest_key1 < key1 || (this->m_heighest_key1 == key1 && this->m_heighest_key2 > key2) ||
            (this->m_heighest_key1 == key1 && this->m_heighest_key2 == key2 && this->m_heighest_key3 < key3))
        {
            this->m_heighest_key1 = key1;
            this->m_heighest_key2 = key2;
            this->m_heighest_key3 = key3;
        }
        return root;
    }
    if (key1 < root->m_key1)
    {
        Node<T> *new_left = insertNode(root->m_left_son, key1, key2, key3, data_element);
        if (!new_left)
        {
            new_left->m_parent = root;
        }
        root->m_left_son = new_left;
    }
    else if (key1 > root->m_key1)
    {
        Node<T> *new_right = insertNode(root->m_right_son, key1, key2, key3, data_element);
        if (!new_right)
        {
            new_right->m_parent = root;
        }
        root->m_right_son = new_right;
    }
    else
    {
        if (key2 < root->m_key2)
        {
            Node<T> *new_right = insertNode(root->m_right_son, key1, key2, key3, data_element);
            if (!new_right)
            {
                new_right->m_parent = root;
            }
            root->m_right_son = new_right;
        }
        else if (key2 > root->m_key2)
        {
            Node<T> *new_left = insertNode(root->m_left_son, key1, key2, key3, data_element);
            if (!new_left)
            {
                new_left->m_parent = root;
            }
            root->m_left_son = new_left;
        }
        else
        {
            if (key3 < root->m_key3)
            {
                Node<T> *new_left = insertNode(root->m_left_son, key1, key2, key3, data_element);
                if (!new_left)
                {
                    new_left->m_parent = root;
                }
                root->m_left_son = new_left;
            }
            else if (key3 > root->m_key3)
            {
                Node<T> *new_right = insertNode(root->m_left_son, key1, key2, key3, data_element);
                if (!new_right)
                {
                    new_right->m_parent = root;
                }
                root->m_right_son = new_right;
            }
        }
    }
    if (this->m_heighest_key1 < key1 || (this->m_heighest_key1 == key1 && this->m_heighest_key2 > key2) ||
        (this->m_heighest_key1 == key1 && this->m_heighest_key2 == key2 && this->m_heighest_key3 < key3))
    {
        this->m_heighest_key1 = key1;
        this->m_heighest_key2 = key2;
        this->m_heighest_key3 = key3;
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

template <class T>
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
    new_root->m_right_son->m_parent = temp;
    new_root->m_parent = temp->m_parent;
    temp->m_parent = new_root;
    new_root->m_right_son = temp;
    if ((new_root->m_parent->m_right_son->m_key1 == temp->m_key1) && (new_root->m_parent->m_right_son->m_key2 == temp->m_key2) && (new_root->m_parent->m_right_son->m_key3 == temp->m_key3))
    {
        new_root->m_parent->m_right_son = new_root;
    }
    else
    {
        new_root->m_parent->m_left_son = new_root;
    }
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
    new_root->m_left_son->m_parent = temp;
    new_root->m_parent = temp->m_parent;
    temp->m_parent = new_root;
    new_root->m_left_son = temp;

    if ((new_root->m_parent->m_right_son->m_key1 == temp->m_key1) && (new_root->m_parent->m_right_son->m_key2 == temp->m_key2) && (new_root->m_parent->m_right_son->m_key3 == temp->m_key3))
    {
        new_root->m_parent->m_right_son = new_root;
    }
    else
    {
        new_root->m_parent->m_left_son = new_root;
    }

    new_root->m_left_son->m_height = 1 + max(calcHeight(new_root->m_left_son->m_left_son),
                                             calcHeight(new_root->m_left_son->m_right_son));
    new_root->m_height = 1 + max(calcHeight(new_root->m_left_son),
                                 calcHeight(new_root->m_right_son));
    return new_root;
}

template <class T>
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

template <class T>
Node<T> *AVLtree<T>::minValueNode(Node<T> *node)
{
    Node<T> *curr = node;

    while (curr->m_left_son != nullptr)
        curr = curr->m_left_son;

    return curr;
}

template <class T>
Node<T> *AVLtree<T>::deleteNode(Node<T> *root, int key1, int key2, int key3) // add a check for the best player
{
    if (root == nullptr)
    {
        return nullptr;
    }
    if (key1 < root->m_key1)
    {
        root->m_left_son = deleteNode(root->m_left_son, key1, key2, key3);
        if (!root->m_left_son)
        {
            root->m_left_son->m_parent = root;
        }
    }
    else if (key1 > root->m_key1)
    {
        root->m_right_son = deleteNode(root->m_right_son, key1, key2, key3);
        if (!root->m_right_son)
        {
            root->m_right_son->m_parent = root;
        }
    }
    else
    {
        if (key2 > root->m_key2)
        {
            root->m_left_son = deleteNode(root->m_left_son, key1, key2, key3);
            if (!root->m_left_son)
            {
                root->m_left_son->m_parent = root;
            }
        }
        else if (key2 < root->m_key2)
        {
            root->m_right_son = deleteNode(root->m_right_son, key1, key2, key3);
            if (!root->m_right_son)
            {
                root->m_right_son->m_parent = root;
            }
        }
        else
        {
            if (key3 < root->m_key3)
            {
                root->m_left_son = deleteNode(root->m_left_son, key1, key2, key3);
                if (!root->m_left_son)
                {
                    root->m_left_son->m_parent = root;
                }
            }
            else if (key3 > root->m_key3)
            {
                root->m_right_son = deleteNode(root->m_right_son, key1, key2, key3);
                if (!root->m_right_son)
                {
                    root->m_right_son->m_parent = root;
                }
            }
            else
            { // this means the key is equal to the current node's key
                if (root->m_left_son && root->m_right_son)
                { // two children
                    // we find the next node that should be the new root - this is the node with the samllest value in the right son tree
                    Node<T> *new_root = minValueNode(root->m_right_son);
                    // we copy the data element and the key to the root, but keep the height and pointers to the next sons.
                    // this means we changed the value of the root node without actually deleting the root
                    root->m_key1 = new_root->m_key1;
                    root->m_key2 = new_root->m_key2;
                    root->m_key3 = new_root->m_key3;
                    root->m_element = new_root->m_element;
                    // now we delete the original node that was copied to become the root, we dont need it anymore because we copied it's values
                    root->m_right_node = deleteNode(root->m_right_son, root->m_key1, root->m_key2, root->m_key3);
                    if (!root->m_right_son)
                    {
                        root->m_right_son->m_parent = root;
                    }
                }
                else
                { // one or zero children
                    Node<T> *temp = root;
                    if (temp->m_left_son == nullptr)
                    {
                        root = root->m_right_son;
                    }
                    else if (temp->m_right_son == nullptr)
                    {
                        root = root->m_left_son
                    }
                    if (!root)
                    {
                        root->m_parent = temp->m_parent;
                    }
                    delete temp;
                }
                root->m_height = 1 + max(calcHeight(root->m_left_son),
                                         calcHeight(root->m_right_son));
            }
        }
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
        if (node->m_key3 != 0)
        {
            [index_after_left_visit] = node->m_key3;
        }
        else
        {
            [index_after_left_visit] = node->m_key1;
        }
        int index_after_right_visit = inOrderVisit(node->m_right_son, array, index_after_left_visit + 1);
        return index_after_right_visit;
    }
}

template <class T>
int closestNode(Node<T> *node)  //NEED TO UPDATE THIS FUNCTION SHIRA - closest could be minvalue if i have a right child, or maxvalue if i have a left child,
{
    if (!node)
    {
        return -1;
    }
    if (!node->m_left_son && !node->m_right_son){
        
        if (!node->m_parent)
        {
            // this is the only node in the tree
            return -1;
        }
        // there is only a parent
        return node->m_parent->m_key3;
    }
    if (node->m_left_son && node->m_right_son)
    {
        //there are two children - one of them is the closest
        int key1_option1 = node->m_left_son->m_key1;
        int key2_option1 = node->m_left_son->m_key2;
        int key3_option1 = node->m_left_son->m_key3;

        int key1_option2 = node->m_right_son->m_key1;
        int key2_option2 = node->m_right_son->m_key2;
        int key3_option2 = node->m_right_son->m_key3;
    }
    else if (node->m_left_son && node->m_right_son == nullptr)
    {
        if (!node->m_parent){
            //there is only a left son
            return node->m_left_son->m_key3;
        }
        //there is only a parent and left son
        int key1_option1 = node->m_left_son->m_key1;
        int key2_option1 = node->m_left_son->m_key2;
        int key3_option1 = node->m_left_son->m_key3;

        int key1_option2 = node->m_parent->m_key1;
        int key2_option2 = node->m_parent->m_key2;
        int key3_option2 = node->m_parent->m_key3;
    }
    else if (node->m_left_son == nullptr && node->m_right_son)
    {
        if (!node->m_parent){
            //there is only a right son
            return node->m_right_son->m_key3;
        }
        //there is only a parent and right son
        int key1_option1 = node->m_right_son->m_key1;
        int key2_option1 = node->m_right_son->m_key2;
        int key3_option1 = node->m_right_son->m_key3;

        int key1_option2 = node->m_parent->m_key1;
        int key2_option2 = node->m_parent->m_key2;
        int key3_option2 = node->m_parent->m_key3;
    }

    if (abs(key1_option1, node->m_key1) < abs(key1_option2, node->m_key1))
    {
        return key3_option1;
    }
    else if (abs(key1_option1, node->m_key1) > abs(key1_option2, node->m_key1))
    {
        return key3_option2;
    }
    else
    {
        if (abs(key2_option1, node->m_key2) < abs(key2_option2, node->m_key2))
        {
            return key3_option1;
        }
        else if (abs(key2_option1, node->m_key2) > abs(key2_option2, node->m_key2))
        {
            return key3_option2;
        }
        else
        {
            if (abs(key3_option1, node->m_key3) < abs(key3_option2, node->m_key3))
            {
                return key3_option1;
            }
            else if (abs(key3_option1, node->m_key3) > abs(key3_option2, node->m_key3))
            {
                return key3_option2;
            }
            else
            {
                return max(key3_option1, key3_option2);
            }
        }
    }
}

/**************end of AVLtree functions***************/

#endif
