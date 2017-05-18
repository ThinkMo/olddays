#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <iostream>
#include <random>  //c++11
#include <cstdlib>

template <typename Key,typename Type>
struct SkipListNode{
	Key key;
	Type value;
	SkipListNode<Key,Type>* forward[1];
};

// Key and Type comparable 
template<typename Key,typename Type,int MaxLevel = 15>
class SkipList{
	typedef SkipListNode<Key,Type>* pSkipListNode;


	void newNodeOfLevel(pSkipListNode& p,int level = 0)
	{
		int size = sizeof(SkipListNode<Key,Type>) + level*sizeof(pSkipListNode);
		p = (pSkipListNode)malloc(size);
	}

public:
	SkipList();
	~SkipList();

	bool insert(const Key& k,const Type& t);
	bool remove(const Key& k,Type& tout);
	bool search(const Key& k,Type& tout);
	void print();
	int getSize() { return size; }

private:
	int randomLevel();
	int level;
	int size;
	pSkipListNode header;
	pSkipListNode NIL;
};

template<typename Key,typename Type,int MaxLevel>
SkipList<Key,Type,MaxLevel>::SkipList():size(0),level(0)
{
	newNodeOfLevel(NIL);
	//set max key in NIL
	NIL->key = 0x7fffffff;

	newNodeOfLevel(header,MaxLevel);
	for(int i = 0; i < MaxLevel; i++)
	{
		header->forward[i] = NIL;
	}
}

template<typename Key,typename Type,int MaxLevel>
SkipList<Key,Type,MaxLevel>::~SkipList()
{
	pSkipListNode p = header;
	pSkipListNode q;
	while(p != NIL)
	{
		q = p->forward[0];
		free(p);
		p = q;
	}
	free(p);  //NIL
}

template<typename Key,typename Type,int MaxLevel>
int SkipList<Key,Type,MaxLevel>::randomLevel()
{
	int lev = 0;
	std::random_device rd;
	while(1)
	{
		if(rd()%2 == 0)
		  lev++;
		else
		  break;
	}

	return lev < MaxLevel ? lev : MaxLevel-1;
}

template<typename Key,typename Type,int MaxLevel>
bool SkipList<Key,Type,MaxLevel>::insert(const Key& k,const Type& t)
{
	pSkipListNode update[MaxLevel], p = header;
	for(int i = level; i >= 0; i--)
	{
		while(p->forward[i] != NIL && p->forward[i]->key < k)
		  p = p->forward[i];
		update[i] = p;
	}

	p = p->forward[0];

	if(p != NIL && p->key == k)
	{
		p->value = t;
		return false;
	}

	int lev = randomLevel();

	if(lev > level)
	{
		lev = ++level;
		update[lev] = header;
	}

	pSkipListNode newNode;
	newNodeOfLevel(newNode,lev);
	newNode->key = k;
	newNode->value = t;

	for(int i = lev; i >= 0; i--)
	{
		p = update[i];
		newNode->forward[i] = p->forward[i];
		p->forward[i] = newNode;
	}

	++size;
	return true;
}


template<typename Key,typename Type,int MaxLevel>
bool SkipList<Key,Type,MaxLevel>::remove(const Key& k,Type& tout)
{
	pSkipListNode update[MaxLevel], p = header;
	for(int i = level; i >= 0; i--)
	{
		while(p->forward[i] != NIL && p->forward[i]->key < k)
		  p = p->forward[i];

		update[i] = p;
	}

	p = p->forward[0];

	if(p != NIL && p->key == k)
	{
		tout = p->value;

		for(int i = 0; i <= level; i++)
		{
			if(update[i]->forward[i] != p)
				break;
			update[i]->forward[i] = p->forward[i];
		}
	
		free(p);
		while(level > 0 && header->forward[level] == NIL)
			level--;

		size--;

		return true;
	}else
		return false;
}

/*
template<typename Key,typename Type,int MaxLevel = 15>
bool SkipList::removeKey(Key& k)
{

}
*/
template<typename Key,typename Type,int MaxLevel>
bool SkipList<Key,Type,MaxLevel>::search(const Key& k,Type& tout)
{
	pSkipListNode p = header;
	for(int i = level; i >= 0; i--)
	{
		while(p->forward[i] != NIL && p->forward[i]->key < k)
		  p = p->forward[i];
	}
	p = p->forward[0];
	if(p != NIL && k == p->key)
	{
		tout = p->value;
		return true;
	}
	return false;
}

template<typename Key,typename Type,int MaxLevel>
void SkipList<Key,Type,MaxLevel>::print()
{
	pSkipListNode p = header;
	while(p->forward[0] != NIL)
	{
		std::cout << "Key:" << p->forward[0]->key << "Value:" << p->forward[0]->value << std::endl;
		p = p->forward[0];
	}
}

#endif
