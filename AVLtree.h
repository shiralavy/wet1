#ifndef AVLTREE_H
#define AVLTREE_H
//
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
    T m_element;
    int m_key1;
    int m_key2;
    int m_key3;
    int m_height;
    Node<T> *m_parent;
    Node<T> *m_left_son;
    Node<T> *m_right_son;

    // bool operator==(const Node &other) const;
    Node(const T &data_element, int key1 = -1, int key2 = -1, int key3 = -1, int height = 0, Node<T> *parent = nullptr, Node<T> *left_son = nullptr,
         Node<T> *right_son = nullptr);
    ~Node();
    // bool Node<T>::operator==(const Node &other) const;
};

/**************Node functions*****************/
/*
template <class T>
bool Node<T>::operator==(const Node &other) const
{
    return m_key1 == other.m_key1;
}
*/
template <class T>
Node<T>::Node(const T &data_element, int key1, int key2, int key3, int height, Node<T> *parent, Node<T> *left_son,
              Node<T> *right_son) : m_element(data_element), m_key1(key1), m_key2(key2), m_key3(key3), m_height(height), m_parent(parent), m_left_son(left_son), m_right_son(right_son) {}

template <class T>
Node<T>::~Node()
{
    delete m_left_son;
    delete m_right_son;
    m_parent = nullptr;
    //delete m_parent;
}

/**************end of Node functions*******/

template <class T>
class AVLtree
{
public:
    Node<T> *m_root;
    int m_highest_key1;
    int m_highest_key2;
    int m_highest_key3;
    // int m_lowest_key;

    AVLtree() : m_root(nullptr),m_highest_key1(-1), m_highest_key2(-1), m_highest_key3(-1){};
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
    Node<T> *minValueNode(Node<T> *node);
    Node<T> *maxValueNode(Node<T> *node);
    int inOrderVisit(Node<T> *node, int *const array, int start_index);
    int inOrderVisitBetweenRange(Node<T> *node, Node<T> **const array, int start_index, int minRange, int maxRange);
    int inOrderVisitUnite(Node<T> *node, Node<T> **array, int start_index);
    Node<T> *closestHigherNode(Node<T> *node);
    Node<T> *closestLowerNode(Node<T> *node);
    Node<T> *closerBetweenTwoOptions(Node<T> *node, Node<T> *option1, Node<T> *option2);
    void merge(Node<T> **array1, Node<T> **array2, Node<T> **mergedArray, int size1, int size2);
    Node<T> *arrayToTree(Node<T> **array, int start, int end);

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
    int h = 0;
    if (!node)
    {
        return -1;
    }
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
    if (key1 < 0)
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
                return findNode(root->m_right_son, key1, key2, key3);
            }
            else
            {
                return findNode(root->m_left_son, key1, key2, key3);
            }
        }
        else if (root->m_key2 < key2)
        {
            return findNode(root->m_left_son, key1, key2, key3);
        }
        else
        {
            return findNode(root->m_right_son, key1, key2, key3);
        }
    }
    else if (root->m_key1 < key1)
    {
        return findNode(root->m_right_son, key1, key2, key3);
    }
    else if (root->m_key1 > key1)
    {
        return findNode(root->m_left_son, key1, key2, key3);
    }
    return nullptr;
}

template <class T>
Node<T> *AVLtree<T>::insertNode(Node<T> *root, int key1, int key2, int key3, const T &data_element)
{
    if (root == nullptr)
    {
        root = new Node<T>(data_element, key1, key2, key3);
        if (this->m_highest_key1 < key1 || (this->m_highest_key1 == key1 && this->m_highest_key2 > key2) ||
            (this->m_highest_key1 == key1 && this->m_highest_key2 == key2 && this->m_highest_key3 < key3))
        {
            this->m_highest_key1 = key1;
            this->m_highest_key2 = key2;
            this->m_highest_key3 = key3;
        }
        //return root;
    }
    if (key1 < root->m_key1)
    {
        Node<T> *new_left = insertNode(root->m_left_son, key1, key2, key3, data_element);
        if (new_left)
        {
            new_left->m_parent = root;
        }
        root->m_left_son = new_left;
    }
    else if (key1 > root->m_key1)
    {
        Node<T> *new_right = insertNode(root->m_right_son, key1, key2, key3, data_element);
        if (new_right)
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
            if (new_right)
            {
                new_right->m_parent = root;
            }
            root->m_right_son = new_right;
        }
        else if (key2 > root->m_key2)
        {
            Node<T> *new_left = insertNode(root->m_left_son, key1, key2, key3, data_element);
            if (new_left)
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
                if (new_left)
                {
                    new_left->m_parent = root;
                }
                root->m_left_son = new_left;
            }
            else if (key3 > root->m_key3)
            {
                Node<T> *new_right = insertNode(root->m_right_son, key1, key2, key3, data_element);
                if (new_right)
                {
                    new_right->m_parent = root;
                }
                root->m_right_son = new_right;
            }
        }
    }
    if (this->m_highest_key1 < key1 || (this->m_highest_key1 == key1 && this->m_highest_key2 > key2) ||
        (this->m_highest_key1 == key1 && this->m_highest_key2 == key2 && this->m_highest_key3 < key3))
    {
        this->m_highest_key1 = key1;
        this->m_highest_key2 = key2;
        this->m_highest_key3 = key3;
    }
    root->m_height = 1 + max(calcHeight(root->m_left_son),
                             calcHeight(root->m_right_son));

    int balance_factor = getBalance(root);
    if (balance_factor == 2 && getBalance(root->m_left_son) == 1)
    {
        // LL ROTATION
        return RotateLL(root);
    }
    else if (balance_factor == -2 && getBalance(root->m_right_son) == -1)
    {
        // RR ROTATION
        return RotateRR(root);
    }
    else if (balance_factor == -2 && getBalance(root->m_right_son) == 1)
    {
        // RL ROTATION
        return RotateRL(root);
    }
    else if (balance_factor == 2 && getBalance(root->m_left_son) == -1)
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
    new_root->m_right_son = temp;
    new_root->m_parent = temp->m_parent;
    temp->m_parent = new_root;
    if (new_root->m_left_son) {
        new_root->m_left_son->m_parent = new_root;
    }
    if (new_root->m_parent) {
        if ((new_root->m_key1 < new_root->m_parent->m_key1) ||
            (new_root->m_key1 == new_root->m_parent->m_key1 && new_root->m_key2 > new_root->m_parent->m_key2) ||
            (new_root->m_key1 == new_root->m_parent->m_key1 && new_root->m_key2 == new_root->m_parent->m_key2 && new_root->m_key3 < new_root->m_parent->m_key3)) {
            new_root->m_parent->m_left_son = new_root;
        } else {
            new_root->m_parent->m_right_son = new_root;
        }
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
    new_root->m_left_son = temp;
    new_root->m_parent = temp->m_parent;
    temp->m_parent = new_root;
    if (new_root->m_right_son) {
        new_root->m_right_son->m_parent = new_root;
    }
    //new_root->m_left_son = temp;

    if (new_root->m_parent) {
        if ((new_root->m_key1 < new_root->m_parent->m_key1) ||
            (new_root->m_key1 == new_root->m_parent->m_key1 && new_root->m_key2 > new_root->m_parent->m_key2) ||
            (new_root->m_key1 == new_root->m_parent->m_key1 && new_root->m_key2 == new_root->m_parent->m_key2 && new_root->m_key3 < new_root->m_parent->m_key3)) {
            new_root->m_parent->m_left_son = new_root;
        } else {
            new_root->m_parent->m_right_son = new_root;
        }
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
    node->m_right_son = RotateLL(node->m_right_son);
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
Node<T> *AVLtree<T>::maxValueNode(Node<T> *node)
{
    Node<T> *curr = node;

    while (curr->m_right_son != nullptr)
        curr = curr->m_right_son;

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
                    // we find the next node that should be the new root - this is the node with the smallest value in the right son tree
                    Node<T> *new_root = minValueNode(root->m_right_son);
                    // we copy the data element and the key to the root, but keep the height and pointers to the next sons.
                    // this means we changed the value of the root node without actually deleting the root
                    root->m_key1 = new_root->m_key1;
                    root->m_key2 = new_root->m_key2;
                    root->m_key3 = new_root->m_key3;
                    root->m_element = new_root->m_element;
                    // now we delete the original node that was copied to become the root, we don't need it anymore because we copied its values
                    root->m_right_son = deleteNode(root->m_right_son, root->m_key1, root->m_key2, root->m_key3);
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
                        root = root->m_left_son;
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
    if (balance_factor == 2 && getBalance(root->m_left_son) == 1)
    {
        // LL ROTATION
        return RotateLL(root);
    }
    else if (balance_factor == -2 && getBalance(root->m_right_son) == -1)
    {
        // RR ROTATION
        return RotateRR(root);
    }
    else if (balance_factor == -2 && getBalance(root->m_right_son) == 1)
    {
        // RL ROTATION
        return RotateRL(root);
    }
    else if (balance_factor == 2 && getBalance(root->m_left_son) == -1)
    {
        // LR ROTATION
        return RotateLR(root);
    }
    return root;
}

template <class T>
int AVLtree<T>::inOrderVisit(Node<T> *node, int *const array, int start_index)
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
            array[index_after_left_visit] = node->m_key3;
        }
        else
        {
            array[index_after_left_visit] = node->m_key1;
        }
        int index_after_right_visit = inOrderVisit(node->m_right_son, array, index_after_left_visit + 1);
        return index_after_right_visit;
    }
}

template <class T>
int AVLtree<T>::inOrderVisitBetweenRange(Node<T> *node, Node<T> **const array, int start_index, int minRange, int maxRange)
{
    if (!node)
    {
        return start_index;
    }
    else
    {
        int index = start_index;
        if (node->m_key1 > minRange)
        {
            index = inOrderVisitBetweenRange(node->m_left_son, array, start_index, minRange, maxRange);
        }
        if (node->m_key1 >= minRange && node->m_key1 <= maxRange)
        {
            array[index] = node;
            index++;
        }
        if (node->m_key1 < maxRange)
        {
            index = inOrderVisitBetweenRange(node->m_right_son, array, index,minRange, maxRange);
        }
        return index;
    }
}

template <class T>
Node<T> *AVLtree<T>::closestHigherNode(Node<T> *node)
{
    if (!node)
    {
        return nullptr;
    }
    if (!node->m_parent && !node->m_right_son)
    {
        // this node doesn't have a parent or a right son - there is no higher value node in the tree
        return nullptr;
    }
    if (node->m_parent && node->m_right_son == nullptr)
    {
        // there is a parent and no right son
        if ((node->m_key1 < node->m_parent->m_key1) ||
        (node->m_key1 == node->m_parent->m_key1 && node->m_key2 > node->m_parent->m_key2) ||
        (node->m_key1 == node->m_parent->m_key1 && node->m_key2 == node->m_parent->m_key2 && node->m_key3 < node->m_parent->m_key3))
        {
            // this node is the left son of the parent, so the parent is bigger
            return node->m_parent;
        }
    }
    else
    {
        // there is a right son and a parent or only a right son and no parent, either way the parent is not the closest higher
        return minValueNode(node->m_right_son);
    }
    return nullptr;
}

template <class T>
Node<T> *AVLtree<T>::closestLowerNode(Node<T> *node)
{
    if (!node)
    {
        return nullptr;
    }
    if (!node->m_parent && !node->m_left_son)
    {
        // this node doesn't have a parent or a left son - there is no lower value node in the tree
        return nullptr;
    }
    if (node->m_parent && node->m_left_son == nullptr)
    {
        // there is a parent and no left son
        if (node->m_key1 > node->m_parent->m_key1 ||
           (node->m_key1 == node->m_parent->m_key1 && node->m_key2 < node->m_parent->m_key2) ||
           (node->m_key1 == node->m_parent->m_key1 && node->m_key2 == node->m_parent->m_key2 && node->m_key3 > node->m_parent->m_key3))
        {
            // this node is the right son of the parent, so the parent is smaller
            return node->m_parent;
        }
    }
    else
    {
        // there is a left son and a parent or only a left son and no parent, either way the parent is not the closest lower
        return maxValueNode(node->m_left_son);
    }
    return nullptr;
}

template <class T>
Node<T> *AVLtree<T>::closerBetweenTwoOptions(Node<T> *node, Node<T> *option1, Node<T> *option2)
{

    int key1_option1 = option1->m_key1;
    int key2_option1 = option1->m_key2;
    int key3_option1 = option1->m_key3;
    //
    int key1_option2 = option2->m_key1;
    int key2_option2 = option2->m_key2;
    int key3_option2 = option2->m_key3;

    if (abs(key1_option1, node->m_key1) < abs(key1_option2, node->m_key1))
    {
        return option1;
    }
    else if (abs(key1_option1, node->m_key1) > abs(key1_option2, node->m_key1))
    {
        return option2;
    }
    else
    {
        if (abs(key2_option1, node->m_key2) < abs(key2_option2, node->m_key2))
        {
            return option1;
        }
        else if (abs(key2_option1, node->m_key2) > abs(key2_option2, node->m_key2))
        {
            return option2;
        }
        else
        {
            if (abs(key3_option1, node->m_key3) < abs(key3_option2, node->m_key3))
            {
                return option1;
            }
            else if (abs(key3_option1, node->m_key3) > abs(key3_option2, node->m_key3))
            {
                return option2;
            }
            else
            {
                if (key3_option1 < key3_option2)
                {
                    return option2;
                }
                return option1;
            }
        }
    }
}

template <class T>
int AVLtree<T>::inOrderVisitUnite(Node<T> *node, Node<T> **array, int start_index)
{
    if (!node)
    {
        return start_index;
    }
    int index_after_left_visit = inOrderVisitUnite(node->m_left_son, array, start_index);
    array[index_after_left_visit] = new Node<T>(node->m_element, node->m_key1, node->m_key2, node->m_key3);
    int index_after_right_visit = inOrderVisitUnite(node->m_right_son, array, index_after_left_visit + 1);
    delete node;
    return index_after_right_visit;
}

template <class T>
void AVLtree<T>::merge(Node<T> **array1, Node<T> **array2, Node<T> **mergedArray, int size1, int size2)
{

    int i = 0, j = 0, k = 0;
    while (i < size1 && j < size2)
    {
        // by goals
        if (array1[i]->m_key1 < array2[j]->m_key1)
        {
            mergedArray[k] = array1[i];
            i++;
            k++;
            continue;
        }
        else if (array1[i]->m_key1 > array2[j]->m_key1)
        {
            mergedArray[k] = array2[j];
            j++;
            k++;
            continue;
        }
        else
        {
            // by cards
            if (array1[i]->m_key2 < array2[j]->m_key2)
            {
                mergedArray[k] = array2[j];
                j++;
                k++;
                continue;
            }

            else if (array1[i]->m_key2 > array2[j]->m_key2)
            {
                mergedArray[k] = array1[i];
                i++;
                k++;
                continue;
            }
                // by id
            else
            {
                if (array1[i]->m_key3 < array2[j]->m_key3)
                {
                    mergedArray[k] = array1[i];
                    i++;
                    k++;
                    continue;
                }
                else if (array1[i]->m_key2 > array2[j]->m_key2)
                {
                    mergedArray[k] = array2[j];
                    j++;
                    k++;
                    continue;
                }
            }
        }
    }
    while (i < size1)
    {
        mergedArray[k] = array1[i];
        i++;
        k++;
    }
    while (j < size2)
    {
        mergedArray[k] = array2[j];
        j++;
        k++;
    }
    return;
    //return mergedArray;
}

/* check with shira */
template <class T>
Node<T> *AVLtree<T>::arrayToTree(Node<T> **array, int start, int end)
{
    if (start > end)
    {
        return nullptr;
    }
    int middle = (start + end) / 2;

    Node<T> *curr = new Node<T>(array[middle]->m_element, array[middle]->m_key1, array[middle]->m_key2, array[middle]->m_key3);
    curr->m_left_son = arrayToTree(array, start, middle - 1);
    curr->m_right_son = arrayToTree(array, middle + 1, end);
    if (!curr->m_left_son)
    {
        curr->m_left_son->m_parent = curr;
    }
    if (!curr->m_right_son)
    {
        curr->m_right_son->m_parent = curr;
    }
    return curr;
}

/**************end of AVLtree functions***************/

#endif