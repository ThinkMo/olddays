/*
filename:select_sort.h
author:ThinkMo
function:a selection sort
*/
#ifndef _SELECT_SORT_H_
#define _SELECT_SORT_H_

#include <iterator>

template<typename RandomAccessIterator> 
class SelectionSort{	
	typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
	typedef int (*COMPARE)(const RandomAccessIterator key1,const RandomAccessIterator key2);//for <
	COMPARE m_compare;
public:
	SelectionSort():m_compare(0) {};
	SelectionSort(COMPARE cmp)
		:m_compare(cmp) {};
	void swap(RandomAccessIterator key1,RandomAccessIterator key2);
	int operator()(RandomAccessIterator key1,RandomAccessIterator key2);
};

template<typename RandomAccessIterator> 
void SelectionSort<RandomAccessIterator>::swap(RandomAccessIterator key1,RandomAccessIterator key2){
	value_type tmp;
	tmp = *key1;
	*key1 = *key2;
	*key2 = tmp;
}


template<typename RandomAccessIterator> 
int SelectionSort<RandomAccessIterator>::operator()(RandomAccessIterator key1,RandomAccessIterator key2){
	RandomAccessIterator i,j;
	if(m_compare)
	{
		for(i = key1;i != key2;i++)
		  for(j = i+1; j != key2 ;j++)
			  if(m_compare(j,i) > 0)
				swap(j,i);
	}else{
		for(i = key1;i != key2;i++)
		  for(j = i+1; j != key2 ;j++)
			  if(*j < *i)
				swap(j,i);
	}
	return 0;
}

#endif
