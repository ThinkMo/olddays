/*
filename:inserter.h
author:ThinkMo
function:a insertion sort,exercise for template class
*/
#ifndef _INSERTER_H_
#define _INSERTER_H_

#include <iterator>

template<typename RandomAccessIterator> 
class Inserter{	
	typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
	typedef int (*COMPARE)(const RandomAccessIterator key1,const RandomAccessIterator key2);//for operator <
	COMPARE m_compare;
public:
	Inserter():m_compare(0) {};
	Inserter(COMPARE cmp)
		:m_compare(cmp) {};
	int operator()(RandomAccessIterator key1,RandomAccessIterator key2);
};

template<typename RandomAccessIterator> 
int Inserter<RandomAccessIterator>::operator()(RandomAccessIterator key1,RandomAccessIterator key2){
	RandomAccessIterator i,j;
	value_type tmp;
	if(m_compare)
	{
		for(i = key1+1;i != key2;i++)
		{
			tmp = *i;
			for(j = i-1; j >= key1 && m_compare(i,j) > 0;j--)
				*(j+1) = *j;
			*(j+1) = tmp;
		}
	}else{
		for(i = key1+1;i != key2;i++)
		{
			tmp = *i;
			for(j = i-1; j >= key1 && *j > tmp;j--)
				*(j+1) = *j;
			*(j+1) = tmp;
		}
	}
	return 0;
}

#endif
