#ifndef _AVLSEARCHTREE_H
#define _AVLSEARCHTREE_H

template <class Key, class Value>
class AVLSearchTree;

template <class Key, class Value>
class AVLNode
{
	public:
		AVLNode(Key key, Value item, AVLNode* lt, AVLNode* rt, int h = 0):
		element(key), data(item), left(lt), right(rt), height(h){}
				
		Key		element;
		Value	data;
		
		AVLNode*	left;
		AVLNode*	right;

		int height;

		friend class AVLSearchTree<Key, Value>;
};

template <class Key, class Value>
class AVLSearchTree
{
	public:
		explicit AVLSearchTree(const Key & notFound);
		~AVLSearchTree();

		void AVLSearchTree<Key, Value>::insert(Key key, Value data);
		AVLNode<Key, Value>* AVLSearchTree<Key, Value>::find(Key key) const;
		AVLNode<Key, Value>* AVLSearchTree<Key, Value>::getRoot() const {return root;}

	private:
		AVLNode <Key, Value>* root;
		const Key ITEM_NOT_FOUND;

		void AVLSearchTree<Key, Value>::emptyTree(AVLNode<Key, Value>* & t) const;
		void AVLSearchTree<Key, Value>::p_insert(const Key & x, Value data, AVLNode<Key, Value>* & t) const;
		AVLNode<Key, Value>* AVLSearchTree<Key, Value>::p_find(const Key & x, AVLNode<Key, Value>* t) const;

		int AVLSearchTree<Key, Value>::height(AVLNode<Key, Value>* t) const;
		int AVLSearchTree<Key, Value>::max(int lhs, int rhs) const;
		void AVLSearchTree<Key, Value>::rotateWithLeftChild(AVLNode<Key, Value>* & k2) const;
		void AVLSearchTree<Key, Value>::rotateWithRightChild(AVLNode<Key, Value>* & k1) const;
		void AVLSearchTree<Key, Value>::doubleWithLeftChild(AVLNode<Key, Value>* & k3) const;
		void AVLSearchTree<Key, Value>::doubleWithRightChild(AVLNode<Key, Value>* & k1) const;

};

#endif