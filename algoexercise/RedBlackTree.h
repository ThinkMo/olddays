#ifndef _REDBLACKTREE_H
#define _REDBLACKTREE_H

#include <iostream>

enum RBTColor{RED, BLACK};

template <class T>
class RBTNode{
	public:
		RBTColor color;
		T key;
		RBTNode *left;
		RBTNode *right;
		RBTNode *parent;

		RBTNode(T value, RBTColor c, RBTNode *p, RBTNode *l,RBTNode *r):key(value),color(c),left(l),right(r),parent(p) {}
};


template <class T>
class RBTree{
	private:
		RBTNode<T> *mRoot;

	public:
		RBTree():mRoot(NULL){ }
		~RBTree(){
			if(mRoot)
			{
				destroy(mRoot);
			}
		}

		void preOrder() { preOrder(mRoot); }
		void inOrder() { inOrder(mRoot); }
		void postOrder() { postOrder(mRoot); }

		RBTNode<T>* search(T key) { return search(mRoot,key); }
		RBTNode<T>* iterativeSearch(T key){ return iterativeSearch(mRoot,key); }

		T minimum() { return minimum(mRoot)->key; }
		T maximum() { return maximum(mRoot)->key; }

		//houji
		RBTNode<T>* successor(RBTNode<T> *x) { 
			if(x->right != NULL)
				return minimum(x->right); 
			
			RBTNode<T> *y = x->parent;
			while((y != NULL) && (x == y->right))
			{
				x = y;
				y = x->parent;
			}
			return y;
		}
		//qianqu
		RBTNode<T>* predecessor(RBTNode<T> *x) { 
			if(x->left != NULL)
				return maximum(x->left); 
			RBTNode<T> *y = x->parent;
			while(y != NULL && x == y->left)
			{
				x = y;
			    y = x->parent;	
			}
			return y;
		}

		void insert(T key)
		{
			RBTNode<T> *node = new RBTNode<T>(key,RED,NULL,NULL,NULL);
			insert(node);
		}

		void remove(T key)
		{
			RBTNode<T> *rm = search(key);
			if(rm)
				remove(rm);
			else
			  std::cout << key << "not exist";
		}

	private:
		void preOrder(RBTNode<T>* tree) const;
		void inOrder(RBTNode<T>* tree) const;
		void postOrder(RBTNode<T>* tree) const;

		RBTNode<T>* search(RBTNode<T>* x,T key) const;
		RBTNode<T>* iterativeSearch(RBTNode<T>* x,T key) const;

		RBTNode<T>* minimum(RBTNode<T> *tree);
		RBTNode<T>* maximum(RBTNode<T> *tree);

		void leftRotate(RBTNode<T>* x);
		void rightRotate(RBTNode<T>* y);

		void insert(RBTNode<T> *node);
		void insertFixup(RBTNode<T>* node);

		void transParent(RBTNode<T> *u,RBTNode<T> *v);
		void remove(RBTNode<T>* node);
		void removeFixup(RBTNode<T> *p,RBTNode<T>* node);

		void destroy(RBTNode<T>* tree);

		void rb_set_parent(RBTNode<T> *s,RBTNode<T> *p)
		{
			s->parent = p;
		}

		bool rb_is_red(RBTNode<T> *r)
		{
			return r->color == RED;
		}

};

template <class T>
void RBTree<T>::preOrder(RBTNode<T> *tree) const
{
	if(tree != NULL)
	{
		std::cout << tree->key << " ";
		preOrder(tree->left);
		preOrder(tree->right);
	}
}

template <class T>
void RBTree<T>::inOrder(RBTNode<T> *tree) const
{
	if(tree != NULL)
	{
		inOrder(tree->left);
		std::cout << tree->key << " " << tree->color << std::endl;
		inOrder(tree->right);
	}
}

template <class T>
void RBTree<T>::postOrder(RBTNode<T> *tree) const
{
	if(tree != NULL)
	{
		postOrder(tree->left);
		postOrder(tree->right);
		std::cout << tree->key << " ";
	}
}

template <class T>
RBTNode<T>* RBTree<T>::search(RBTNode<T>* x,T key) const
{
	if(x == NULL || x->key == key)
	  return x;
	else if(x->key > key)
	  return search(x->left,key);
	else
	  return search(x->right,key);

}

template <typename T>
RBTNode<T>* RBTree<T>::iterativeSearch(RBTNode<T>* x,T key) const
{
	while(x != NULL)
	{
		if(x->key == key)
		  break;
		else if(x->key > key)
		  x = x->left;
		else
		  x = x->right; 
	}
	return x;
}

template <typename T>
RBTNode<T>* RBTree<T>::minimum(RBTNode<T> *tree)
{
	if(tree == NULL)
	  return NULL;
	while(tree->left != NULL)
	  tree = tree->left;
	return tree;
}

template <typename T>
RBTNode<T>* RBTree<T>::maximum(RBTNode<T> *tree)
{
	if(tree == NULL)
	  return NULL;
	while(tree->right != NULL)
	  tree = tree->right;
	return tree;
}

template <class T>
void RBTree<T>::leftRotate(RBTNode<T>* x)
{
	if(x != NULL && x->right != NULL)
	{
		RBTNode<T>* y = x->right;
		x->right = y->left;
		if(y->left != NULL)
			rb_set_parent(y->left,x);

		if(x->parent == NULL)
			mRoot = y;
		else if(x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
	
		rb_set_parent(y,x->parent);
		y->left = x;
		rb_set_parent(x,y);
	}
}

template <class T>
void RBTree<T>::rightRotate(RBTNode<T> *x)
{
	if(x != NULL && x->left != NULL)
	{
		RBTNode<T> *y = x->left;
		x->left = y->right;
		if(y->right != NULL)
			rb_set_parent(y->right,x);

		if(x->parent == NULL)
			mRoot = y;
		else if(x->parent->left == x)
			x->parent->left = y;
		else
			x->parent->right = y;

		rb_set_parent(y,x->parent);
		y->right = x;
		rb_set_parent(x,y);
	}

}

template <typename T>
void RBTree<T>::insert(RBTNode<T> *node)
{
	RBTNode<T> *t = mRoot,*p = NULL;
	while(t != NULL)
	{
		p = t;
		if(t->key > node->key)
			t = t->left;
		else
			t = t->right;
	}

	node->parent = p;
	if(p != NULL)
	{
		if(p->key > node->key)
		  p->left = node;
		else
		  p->right = node;
	}else
		mRoot = node;

	insertFixup(node);

}

template <typename T>
void RBTree<T>::insertFixup(RBTNode<T>* node)
{
	while(node->parent != NULL && rb_is_red(node->parent))
	{
		RBTNode<T> *grand = node->parent->parent;
		if(grand && node->parent == grand->left)
		{
			RBTNode<T> *uncle = grand->right;
			if(uncle && uncle->color == RED)   //case 1
			{
				node->parent->color = BLACK;
				uncle->color = BLACK;
				grand->color = RED;
				node = grand;
			}else {
				if(node == node->parent->right)             //case 2
				{
					node = node->parent;
					leftRotate(node);
				}
				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				rightRotate(grand);
			}
		}else{
			RBTNode<T> *uncle = grand->left;
			if(uncle && uncle->color == RED)   //case 1
			{
				node->parent->color = BLACK;
				uncle->color = BLACK;
				grand->color = RED;
				node = grand;
			}else {
				if(node == node->parent->left)             //case 2
				{
					node = node->parent;
					rightRotate(node);
				}
				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				leftRotate(grand);
			}
		}
	}

	mRoot->color = BLACK;
}


template <typename T>
void RBTree<T>::transParent(RBTNode<T> *u,RBTNode<T> *v) //v substitude u
{
	if(u->parent == NULL)
	  mRoot = v;
	else 
	{
	  if(u == u->parent->left)
		u->parent->left = v;
	  else 
		u->parent->right = v;
	}
	if(v != NULL)
		v->parent = u->parent;
}


template <typename T>
void RBTree<T>::remove(RBTNode<T>* node)
{
	RBTColor color;
	RBTNode<T> *y = node,*x,*p = NULL;
	color = node->color;
	if(node->left == NULL)
	{
		x = node->right;
		transParent(node,node->right);
	}else if(node->right == NULL){
		x = node->left;
		transParent(node,node->left);
	}else{

		y = minimum(node->right);
		color = y->color;
		x = y->right;
		if(y->parent == node)
		{
			if(x != NULL)
			{
				//std::cout << "hello" << std::endl;
				x->parent = node;
			}
		}else
		{
			transParent(y,y->right);
			p = y->parent;
			y->right = node->right;
			y->right->parent = y;
			
		}

		transParent(node,y);
		y->left = node->left;
		y->left->parent = y;
		y->color = node->color;
	}

	if(color == BLACK)
	{
		if(p)
			removeFixup(p,x);
		else
			removeFixup(y,x);
	}
	delete node;
}

template <typename T>
void RBTree<T>::removeFixup(RBTNode<T> *p,RBTNode<T>* node)
{
	while(node != mRoot && (node == NULL || node->color == BLACK))
	{
		if(node == p->left)
		{
			RBTNode<T> *bro = p->right;
			//bro is null color must be BLACK
			if(bro && bro->color == RED)   //case 1
			{
				bro->color = BLACK;
				p->color = RED;
				leftRotate(p);
				bro = p->right;
			}
			//bro is null,bro left and right should be root
			if((!bro) || ((bro->left == NULL || bro->left->color == BLACK) && (bro->right == NULL || bro->right->color == BLACK)))                // case 2
			{
				if(bro)
					bro->color = RED;
				node = p;
				p = node->parent;
			}
			else
			{ 
				if(!bro->right || bro->right->color == BLACK)  //case 3
				{
					bro->left->color = BLACK;
					bro->color = RED;
					rightRotate(bro);
					bro = p->right;
				}
				bro->color = p->color;         //case 4
				p->color = BLACK;
				if(bro->right)
					bro->right->color = BLACK;
				leftRotate(p);
				node = mRoot;
			}
		}else{

			RBTNode<T> *bro = p->left;
			//bro exsit because node is black
			if(bro && bro->color == RED)   //case 1
			{
				bro->color = BLACK;
				p->color = RED;
				leftRotate(p);
				bro = p->left;
			}
			if(!bro || ((bro->right == NULL || bro->right->color == BLACK) && (bro->left == NULL || bro->left->color == BLACK)))                 // case 2
			{
				if(bro)
					bro->color == RED;
				node = p;
				p = node->parent;
			}
			else
			{ 
				if(!bro->left || bro->left->color == BLACK)  //case 3
				{
					bro->right->color = BLACK;
					bro->color = RED;
					leftRotate(bro);
					bro = p->left;
				}

				bro->color = p->color;         //case 4
				p->color = BLACK;
				if(bro->left)
					bro->left->color = BLACK;
				rightRotate(p);
				node = mRoot;

			}
		}
	}
	if(node != NULL)
		node->color = BLACK;
}

template <typename T>
void RBTree<T>::destroy(RBTNode<T>* tree)
{
	if(tree == NULL)
	  return ;

	if(tree->left != NULL)
	  return destroy(tree->left);
	if(tree->right != NULL)
	  return destroy(tree->right);

	delete tree;
	tree = NULL;
}

#endif
