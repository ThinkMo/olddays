//qsort
#ifndef _QSORT_H_
#define _QSORT_H_


int hoare_partition(int *Array,int begin,int end)
{
	int low = begin,high = end;
	int pviot = Array[low];

	while(low < high)
	{
		while((low < high) && (Array[high] >= pviot))
		{
			high--;
		}
		Array[low] = Array[high];

		while((low < high) && (Array[low] <= pviot))
		{
			low++;
		}
		Array[high] = Array[low];
	}
	
	Array[high] = pviot;
	return high;
}

void qsort(int *Array,int begin,int end)
{
	if(begin < end)
	{
		int pos = hoare_partition(Array,begin,end);

		qsort(Array,begin,pos-1);
		qsort(Array,pos+1,end);
	}
}

#endif
