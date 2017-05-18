/*
filename:maximum_subarray.h
author:
funtion:get a maximum subarray,nonlinear
another method:linear__find max 0....i;then find max j...i (j < i)
 */
#ifndef _MAXIMUM_SUBARRAY_H
#define _MAXIMUM_SUBARRAY_H

template<typename RandomAccessIterator>
int maximum_subarray_cross(RandomAccessIterator mid)
{
	//find max i...mid
	//find max mid...j
	//sum i...mid...j
}

template<typename RandomAccessIterator>
int maximum_subarray(RandomAccessIterator first,RandomAccessIterator last)
{
	if(last - first <= 1)
	  return 0;
	RandomAccessIterator mid = first + (last-first)/2;
	int sum1 = maximum_subarray(first,mid);
	int sum2 = maximum_subarray(mid,last);
	int sum3 = maximum_subarray_cross(mid);
	return sum3 > (sum1 > sum2 ? sum1 : sum2) ? sum3 : (sum1 > sum2 ? sum1 : sum2);
}

#endif
