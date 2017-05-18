
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

int select(int *Array,int p,int q,int i) //return the i min element in Array
{
	if((p+1) == i)
	  return Array[p];
	int pos = hoare_partition(Array,p,q);
	if((pos+1) == i)
	  return Array[pos];
	else if((pos+1) < i)
	  return select(Array,pos+1,q,i-pos-1);
	else
	  return select(Array,p,pos-1,i);
}
