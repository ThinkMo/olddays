//qsort
#ifndef _QSORT_H_
#define _QSORT_H_


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

void qsort(int *Array,int begin,int end)
{
	if(begin < end)
	{
		int pos = partition(Array,begin,end);

		qsort(Array,begin,pos-1);
		qsort(Array,pos+1,end);
	}
}

#endif
