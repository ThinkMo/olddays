//qsort
#ifndef _RANDQSORT_H_
#define _RANDQSORT_H_

#include <stdlib.h>
#include <time.h>

void swap(int *one,int *another)
{
	int tmp;
    tmp	= *one;
	*one = *another;
	*another = tmp;
}

int partition(int *Array,int begin,int end)
{
	int i = begin,j;
	for(j = i+1; j < end+1; j++)
	{
		if(Array[j] > Array[begin])
		{
			continue;
		}else
		{
			swap(&Array[i+1],&Array[j]);
			i++;
		}
	}
	swap(&Array[begin],&Array[i]);
	return i;
}

int randpartition(int *Array,int begin,int end)
{
	srand((unsigned int)time(NULL));
	int pos = (rand()%(end - begin +1)) + begin;

	swap(Array+begin,Array+pos);

	return partition(Array,begin,end);
}

void randqsort(int *Array,int begin,int end)
{
	if(begin < end)
	{
		int pos = randpartition(Array,begin,end);

		randqsort(Array,begin,pos-1);
		randqsort(Array,pos+1,end);
	}
}

#endif
