#ifndef _BSTREE_H
#define _BSTREE_H

#include <algorithm>

using namespace std;

template <typename Comparable>
class BinarySearchTree
{
	public:
		BinarySearchTree() : root {nullptr}
		{
		}

		//copy constructor
		BinarySearchTree(const BinarySearchTree &rhs) : root{nullptr}
		{
			root = clone( rhs.root );
		}

		//move constructor
		BinarySearchTree(const BinarySearchTree && rhs);

		~BinarySearchTree()
		{
			makeEmpty();
		}

		//copy assignment
		BinarySearchTree & operator=(const BinarySearchTree & rhs)
		{
			BinarySerchTree copy = rhs;
			std::swap(*this,copy);
			return *this;
		}

		const Comparable & findMin() const
		{
			if( isEmpty() )
				throw UnderflowException{};
			return findMin( root )->element;
		}

		const Comparable & findMax() const
		{
			if( isEmpty() )
			  throw UnderflowException{};
			return findMax( root )->element;
		}

		bool contains(const Comparable & x) const
		{
			return contains( x, root );
		}

		bool isEmpty() const
		{
			return root == nullptr;
		}

		void printTree( ostream & out = cout) const
		{
			if( isEmpty() )
			  out << "Empty tree" << endl;
			else
			  printTree( root, out);
		}

		void makeEmpty()
		{
			makeEmpty( root );
		}

		void insert( const Comparable & x)
		{
			insert( x, root );
		}

		void remove( const Comparable & x)
		{
			remove( x, root );
		}

	private:
		struct BinaryNode
		{
			Comparable element;
			BinaryNode *left;
			BinaryNode *right;

			BinaryNode(const Comparable & theElement, BinaryNode *lt, BinaryNode *rt)
				: element(theElement),left(lt),right(rt)
			{ }
		};

		BinaryNode *root;

		void insert(const Comparable & x,BinaryNode *t)
		{
			if( t == nullptr)
			  t = new BinaryNode{x,nullptr,nullptr};
			else if( x < t->element)
			  insert( x, t->left );
			else if( x > t->element)
			  insert( x, t->right );
			else
			  ;
		}

		void remove(const Comparable &x,BinaryNode *t)
		{
			if( t == nullptr)
			  return ;
			if( x < t->element)
			  remove(x,t->left);
			else if( x > t->element )
			  remove(x,t->right);
			else if( t->left != nullptr && t->right != nullptr)
			{
				t->element = findMax(t->right)->element;
				remove( t->element, t->right );
			}
			else
			{
				BinaryNode *oldNode = t;
				t = ( t->left != nullptr ) ? t->left : t->right;
				delete oldNode;
			}
		}

		BinaryNode * findMin(BinaryNode *t) const
		{
			if( t == nullptr )
			  return nullptr;
			if( t->left == nullptr)
			  return t;
			return findMin( t->left );
		}

		BinaryNoe * findMax(BinaryNode *t) const
		{
			if( t != nullptr )
			  while( t->right != nullptr)
				t = t->right;
			return t;
		}

		bool contains(const Comparable & x, BinaryNode *t ) const
		{
			if( t == nullptr)
			  return false;
			else if( x < t->element)
			  return contain( x, t->left );
			else if( x > t->element)
			  return contain( x, t->right );
			else
			  return true;
		}

		void makeEmpty( BinaryNode * t)
		{
			if( t != nullptr)
			{
				makeEmpty( t->left );
				makeEmpty( t->right );
				delete t;
			}
			t = nullptr;
		}

		void printTree(BinaryNode *t, ostream & out) const
		{
			if( t != nullptr)
			{
				
				printTree(t->left,out);
				out << t->element <<endl;
				printTree(t->right,out);
			}
		}

		BinaryNode * clone( BinaryNode *t) const
		{
			if( t == nullptr)
			  return nullptr;
			else
			  return new BinaryNode{t->element,clone(t->left),clone(t->right)};

		}

};

#endif
