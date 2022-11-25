#ifndef AVLTREE_H
#define AVLTREE_H


#include <iostream>

typedef enum {
    AVL_SUCCESS,
    AVL_NODE_ALREADY_EXISTS,
    AVL_NODE_DOES_NOT_EXIST,
    AVL_NULL_ARGUMENT,
    AVL_BAD_ALLOCATION,
} AVLResult;


template <class T>
class Node{
    int m_key;
    int m_height;
    T m_data_element;
    shared_ptr<T> m_left_son;
    shared_ptr<T> m_right_son;

 bool operator==(const Node& other) const;
    
};

/**************Node functions*****************/
template<class T>
bool Node<T>::operator==(const Node& other)const{
    return m_key==other.m_key
}

template <class T>
class AVLtree{
    public:
    shared_ptr<Node<T>> m_root;
    int m_highest_key;
    int m_second_highest_key;
    int m_lowest_key;
    int m_second_lowest_key;
    
    AVLtree() : root(nullptr), m_highest_key(0), m_lowest_key(0){};
    //AVLtree(shared_ptr<Node<T>> root) : m_root(root) {};
    ~AVLtree() = default; //is this default or do we need to implement?
    int calcHeight(shared_ptr<Node<T>> m_node);
    int getBalance(shared_ptr<Node<T>> m_node);
    shared_ptr<Node<T>> findNode(shared_ptr<Node<T>> root, int key);
    shared_ptr<Node<T>> insertNode(shared_ptr<Node<T>> node)
    AVLResult RotateLL(shared_ptr<Node<T>> node);
    AVLResult RotateRR(shared_ptr<Node<T>> node);
    AVLResult RotateRL(shared_ptr<Node<T>> node);
    AVLResult RotateLR(shared_ptr<Node<T>> node);
    AVLResult deleteNode(shared_ptr<Node<T>> node, int key);
    

    int max(int a, int b)
    {
	return (a > b) ? a : b;
    }
};


/**************AVLtree functions***************/
template <class T>
int AVLtree<T>::calcHeight(shared_ptr<Node> node){
    if (!node){
        return -1;
    }
    int h = 0;
    else{
        if (node->m_left_son && node->m_right_son){
            h = 1 + max(node->m_left_son->m_height, node->m_right_son->m_height);    
        }
        else if(!node->m_left_son && node->m_right_son){
            h = 1 + node->m_right_son->m_height;
        }
        else if(node->m_left_son && !node->m_right_son){
            h = 1 + node->m_left_son->m_height;
        }
    }
    return h;
}

template <class T>
int AVLtree<T>::getBalance(shared_ptr<Node<T>> node){
    if (!node){
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
	if (!node)
	{
		return nullptr;
	}
	if (node->m_key == key)
	{
		return node;
	}
	if (key < node->m_key)
	{
		findNode(node->m_left_son, key);
	}
	if (key > node->m_key)
	{
		findNode(node->m_right_son, key);
	}
}

template <class T>
shared_ptr<Node<T>> AVLtree<T>::insertNode(shared_ptr<Node<T>> root, int key, T& data_element)
{
	if (root == nullptr)
	{
        root = make_shared<Node<T>>(key, data_element);
        return root;
	}
    else{
        if (findNode(this->m_root, key) != nullptr){
            return root;
        }
    }
    if (key < root->m_key)
        root->m_left_son = insertNode(root->m_left_son, key, data_element);
    else if (key > root->m_key)
        root->m_right_son = insertNode(root->m_right_son, key, data_element);
 
    root->m_height = 1 + max(height(root->m_left_son),
                        height(root->m_right_son));
 

	int balance_factor = getBalance(node);
	if (balance_factor == 2 && getBalance(node->m_left_son) == 1)
	{
        AVLResult res = RotateLL(root);
        if (res == AVL_SUCCESS){
            return root;
        }
		// LL ROTATION
	}
	else if (balance_factor == -2 && getBalance(node->m_right_son) == -1)
	{
		// RR ROTATION
	}
	else if (balance_factor == -2 && getBalance(node->m_right_son) == 1)
	{
		// RL ROTATION
	}
	else if (balance_factor == 2 && getBalance(node->m_left_son) == -1)
	{
		// LR ROTATION
	}
    return root;
}



template <class T>
 AVLResult AVLtree<T>::RotateLL(shared_ptr<Node<T>> node)
 {
    if(node==nullptr)
    {
        return  AVL_NULL_ARGUMENT;
    }
    shared_ptr<Node<T>> temp;
	shared_ptr<Node<T>> tp;
	temp = node;
	tp = temp->m_left_son;
	temp->m_left_son= tp->m_right_son;
	tp->m_right_son = temp;
	return AVL_SUCCESS;
 }


template <class T>
 AVLResult AVLtree<T>::RotateRR(shared_ptr<Node<T>> node)
 {
    
    if (node== nullptr)
    {
        return AVL_NULL_ARGUMENT;
    }
    shared_ptr<Team> temp;
	shared_ptr<Team> tp;
	temp = node;
	tp = temp->m_right_team;

	temp->m_right_team = tp->m_left_team;
	tp->m_left_team = temp;
	return AVL_SUCCESS;
 }
 

 template <class T>
 AVLResult AVLtree<T>::RotateLR(shared_ptr<Node<T>> node)
 {
    AVLResults res1=RotateLL(node->m_left_son)
    AVLResults res2=RotateLL(node);
    if(res1 == AVL_SUCCESS && res2 == AVL_SUCCESS)
    {
        return AVL_SUCCESS;
    }
    else
    {
        return AVL_NULL_ARGUMENT;
    }
 }

 
 template <class T>
 AVLResult AVLtree<T>::RotateRL(shared_ptr<Node<T>> node)
 {
    AVLResults res1=RotateLL(node->m_left_right)
    AVLResults res2=RotateLL(node);
    if(res1 == AVL_SUCCESS && res2 == AVL_SUCCESS)
    {
        return AVL_SUCCESS;
    }
    else
    {
        return AVL_NULL_ARGUMENT;
    }
 }

   AVLResult AVLtree<T>:: deleteNode(shared_ptr<Node<T>> node, int key)
   {
    if(node==nullptr)
    {
        return AVL_NULL_ARGUMENT;
    }

    if(key<node->m_key)
    {
        node->m_left_son=deleteNode(node->m_left_son,key);
        if(getBalance(node)==-2)
        {
            if(getBalance(node->m_right_son)<=0)
            {
                node=RotateRR(node);
            }
            else
                node=RotateRL(node);
        }
    }
     else if(key>node->m_key)
     {
        node->m_right_son=deleteNode(node->m_right_son,key);
        if(getBalance(node)==2)
        {
            if(getBalance(node->left)>=0)
            {
                node=RotateLL(node);
            }
            else 
                node=RotateLR(node);
        }
     }
     //last condition on of delete 
     else
     {
        
     }
 }
   


// this is a qustioned implemt of deleteNode//
/*
  AVLResult AVLtree<T>:: deleteNode(shared_ptr<Node<T>> node, int key)
  {
    if (node == nullptr)
	{
		return AVL_NULL_ARGUMENT;
	}

	if (key< node->m_key)
	{
		node->m_left_son = deleteNode(node->m_left_team, key);
	}

	else if (key > node->m_key)
	{
		node->m_right_team = deleteNode(node->m_right_team, teamId);
	}
	else
	{
		if ((node->m_left_son == nullptr) || (node->m_right_son == nullptr))
		{
			shared_ptr<Node<T>> temp = //node->m_team ? node->m_left_team:node->m_right_team;
            if(temp == NULL)
        {
            temp=node;
            node=NULL;
        }
        else{
            *node=*temp;
        }
		  
	}
    else
    {
       shared_ptr<Team> temp=minValueNode(node->m_right_team);

       node->m_team_id=temp->m_team_id;

       node->m_right_team=deleteNode(node->m_right_team,temp->m_team_id);
    }

 } 

    if(node==NULL)
    {
        return node;
    }
	

    node->m_height_teams=1+ max(node->m_right_team->m_height_teams,node->m_left_team->m_height_teams);

    int balance=calc_BF_teams(node);


    if(balance>ONE &&calc_BF_teams(node->m_left_team)>=0 )
    {
        return llrotation_teams(node);
    }

    if(balance-1 &calc_BF_teams(node->m_right_team)<=0)
    {
        
    }
    
    if(balance> ONE && calc_BF_teams(node->m_left_team)<0)
    {
        node->m_left_team=lrrotation_teams(node->m_left_team);
        return node;
    }
  }

shared_ptr<Team> minValueNode(shared_ptr<Team> node) //helps the delete function 
{ 
   shared_ptr<Team> current = node; 
  
    while (current->m_left_team != NULL) 
        current = current->m_left_team; 
  
    return current; 
} 
*/


	


/**************end of AVLtree functions***************/

#endif