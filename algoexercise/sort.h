/*
filename:sort.h
author:ThinkMo
function:insertion sort|selection sort
like std::sort
*/
#ifndef _SORT_H_
#define _SORT_H_

#include <iterator>

/*=============insertion sort================*/
template<typename RandomAccessIterator> 
int insert_sort(RandomAccessIterator first,RandomAccessIterator last){
	typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
	RandomAccessIterator i,j;
	value_type tmp;
	for(i = first+1;i != last;i++)
	{
		tmp = *i;
		for(j = i-1; j >= first && *j > tmp;j--)
			*(j+1) = *j;
		*(j+1) = tmp;
	}
	return 0;
}
template<typename RandomAccessIterator,typename Compare> 
int insert_sort(RandomAccessIterator first,RandomAccessIterator last,Compare comp){
	typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
	RandomAccessIterator i,j;
	value_type tmp;
	for(i = first+1;i != last;i++)
	{
		tmp = *i;
		for(j = i-1; j >= first && comp(tmp,*j) > 0;j--) //odd
			*(j+1) = *j;
		*(j+1) = tmp;
	}
	return 0;
}
/*=============selection sort================*/
template<typename RandomAccessIterator> 
void swap(RandomAccessIterator key1,RandomAccessIterator key2){
	typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
	value_type tmp;
	tmp = *key1;
	*key1 = *key2;
	*key2 = tmp;
}

template<typename RandomAccessIterator> 
int select_sort(RandomAccessIterator first,RandomAccessIterator last){
	RandomAccessIterator i,j;
	for(i = first;i != last;i++)
		for(j = i+1; j != last ;j++)
			if(*j < *i)
				swap(j,i);
	return 0;
}

template<typename RandomAccessIterator,typename Compare> 
int select_sort(RandomAccessIterator first,RandomAccessIterator last,Compare comp){
	RandomAccessIterator i,j;
	for(i = first;i != last;i++)
		for(j = i+1; j != last ;j++)
			if(comp(j,i) > 0)
				swap(j,i);
	return 0;
}

/*=============merge sort================*/

template <typename RandomAccessIterator>
int merge(RandomAccessIterator first,RandomAccessIterator mid,RandomAccessIterator last)
{
	typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
	value_type *m = new value_type[last-first];
	int mpos = 0;
	RandomAccessIterator fpos = first,lpos = mid;
	while(fpos != mid || lpos != last)
	{
		if(fpos == mid)
		{
			while(lpos != last)
			{
				m[mpos] = *lpos;
				lpos++;
				mpos++;
			}
			continue;
		}
		else if(lpos == last)
		{
			while(fpos != mid)
			{
				m[mpos] = *fpos;
				fpos++;
				mpos++;
			}
			continue;
		}
		if(*fpos < *lpos)
		{
			m[mpos] = *fpos;
			mpos++;
			fpos++;
		}else
		{
			m[mpos] = *lpos;
			mpos++;
			lpos++;
		}
	}
	for(fpos = first,mpos=0;fpos != last;fpos++,mpos++)
		*fpos = m[mpos];
	delete [] m;
}

template <typename RandomAccessIterator>
int mgsort(RandomAccessIterator first,RandomAccessIterator last)
{
	if(last - first <= 1)
		return 0;
	RandomAccessIterator mid = first + (last-first)/2;
	mgsort(first,mid);
	mgsort(mid,last);
	merge(first,mid,last);
	return 0;
}

#endif
