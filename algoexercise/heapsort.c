/* heap sort(big) */
#include <stdio.h>

void swap(int *one,int *another)
{
	int tmp = *one;
	*one = *another;
	*another = tmp;
}

void heapify(int *Array,int pos,int size)
{
	int left = pos*2 + 1;
	int right = left + 1;
	int big = pos;

	if((left < size) && (Array[pos] < Array[left]))
		big = left;

	if((right < size) && (Array[big] < Array[right]))
	    big = right;

	if(big != pos)
	{
		swap(&Array[pos],&Array[big]);
		heapify(Array,big,size);
	}

}

void buildheap(int *Array,int size)
{
	int half = (size-1) / 2;

	for( ; half >= 0; half--)
	    heapify(Array,half,size);
}

void heapsort(int *Array,int size)
{
	buildheap(Array,size);
	
	int heap_size = size;

	while(heap_size > 0)
	{
		swap(&Array[0],&Array[heap_size-1]);
		heap_size--;
		heapify(Array,0,heap_size);
	}
}

/*========================================*/
//Priority Queue
void insert(int *Array,int size,int value)
{
	int son = size;
	int parent = (son - 1) / 2;
	Array[size++] = value;

	while((parent >= 0) && (Array[parent] < Array[son]))
	{
		swap(Array+parent,Array+son);
		son = parent;
		paretn = (parent-1) / 2;
	}
}

int maximum(int *Array)
{
	return Array[0];
}

void increase_key(int *Array,int size,int x,int key);

int getmax(int *Array,int size)
{
	int max = Array[0];
	Array[0] = Array[size-1];
	size--;

	heapify(Array,1,size);
	return max;
}
/*========================================*/


int main()
{
	int Array[] = {6,2,5,7,10,1,23,9,4};
	heapsort(Array,9);

	int i;
	for(i = 0; i < 9; i++)
	  printf("%d\t",*(Array+i));
}
