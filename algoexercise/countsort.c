#include <stdio.h>
#include <stdlib.h>

void countsort(int *Array,int *Out,int size,int max) //element in Array all between max and 0
{
	int *tmp = (int *)malloc(sizeof(int) * (max + 1));
	
	int i;
	for(i = 0; i < max + 1; i++)    //k
	  *(tmp+i) = 0;

	for(i = 0; i < size; i++)       //n
	  tmp[Array[i]]++;
	
	for(i = 1; i < max + 1; i++)    //k
	  tmp[i] = tmp[i] + tmp[i-1];

	for(i = size-1; i >= 0; i--)    //n
	{
		Out[tmp[Array[i]] - 1] = Array[i];
		tmp[Array[i]]-- ;
	}
}//2(n+k)    k<<n O(n)

int main()
{
	int Array[] = {3,5,7,8,9,1,2,4,3};
	int Out[9];
	int size = sizeof(Out)/sizeof(int);

	countsort(Array,Out,size,9);

	int i;
	for(i = 0; i < size; i++)
	  printf("%d\t",*(Out+i));
	printf("\n");
}
