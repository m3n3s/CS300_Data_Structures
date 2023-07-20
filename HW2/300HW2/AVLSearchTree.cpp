#include "AVLSearchTree.h"
#include <iostream>

using namespace std;

template <class Key, class Value>
AVLSearchTree<Key, Value>::
		AVLSearchTree(const Key & notFound ):
          			ITEM_NOT_FOUND( notFound ), root( NULL ){}

template <class Key, class Value>
AVLSearchTree<Key, Value>::~AVLSearchTree()
{
	// delete all nodes
	emptyTree(root);
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::emptyTree(AVLNode<Key, Value>* & t) const
{
	if(t != NULL)
	{
		emptyTree(t->left);
		emptyTree(t->right);
		delete t;
	}
	t = NULL;
}

template <class Key, class Value>
int AVLSearchTree<Key, Value>::max(int lhs, int rhs) const
{
	if (lhs > rhs)
		return lhs;
	return rhs;            
}


template <class Key, class Value>
int AVLSearchTree<Key, Value>::height(AVLNode<Key, Value>* t) const
{
	if (t == NULL)
		return -1;

	return t->height;
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::insert(Key key, Value data)
{
	p_insert(key, data, root);
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::p_insert(const Key & x, Value data, AVLNode<Key, Value>* & t ) const
{

    if (t == NULL)
		t = new AVLNode<Key, Value>(x, data, NULL, NULL);

    else if ( x < t->element )
	{
		// X should be inserted to the left tree!
        p_insert(x, data, t->left);
        
		// Check if the left tree is out of balance (left subtree grew in height!)
		if(height( t->left) - height(t->right) == 2)
		{
			if (x < t->left->element)  // X was inserted to the left-left subtree!
				rotateWithLeftChild(t);
            
			else // X was inserted to the left-right subtree!
				doubleWithLeftChild(t);
		}
	}

	else if(t->element < x)
	{   
		// Otherwise X is inserted to the right subtree
		p_insert(x, data, t->right);

        if(height( t->right ) - height( t->left ) == 2 )
		{
		// height of the right subtree increased
			if(t->right->element < x) // X was inserted to right-right subtree
                rotateWithRightChild(t);
            
			else // X was inserted to right-left subtree
				doubleWithRightChild(t);
		}
	}
    
	else
		; // Duplicate, do nothing
          
	// update the height of the node	
	t->height = max(height(t->left), height(t->right)) + 1;
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::rotateWithLeftChild(AVLNode<Key, Value>* & k2 ) const
{
	AVLNode<Key, Value>* k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), k2->height) + 1;
    k2 = k1;
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::rotateWithRightChild(AVLNode<Key, Value>* & k1 ) const
{
	AVLNode<Key, Value> *k2 = k1->right;
	k1->right = k2->left;
	k2->left = k1;
	k1->height = max(height(k1->left), height(k1->right)) + 1;
	k2->height = max(height(k2->right), k1->height) + 1;
	k1 = k2;
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::doubleWithLeftChild(AVLNode<Key, Value>* & k3 ) const
{
	rotateWithRightChild(k3->left);
	rotateWithLeftChild(k3);
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::doubleWithRightChild(AVLNode<Key, Value>* & k1) const
{
	rotateWithLeftChild(k1->right);
	rotateWithRightChild(k1);
}

template <class Key, class Value>
AVLNode<Key, Value>* AVLSearchTree<Key, Value>::find(Key key) const
{
	return p_find(key, root);
}

template <class Key, class Value>
AVLNode<Key, Value>* AVLSearchTree<Key, Value>::p_find(const Key & x, AVLNode<Key, Value>* t) const
{
	while(t != NULL)
	{
		if(x < t->element)
			t = t->left;

        else if(t->element < x)
			t = t->right;

        else
			return t;    // Match
	}

	return NULL;   // No match
}