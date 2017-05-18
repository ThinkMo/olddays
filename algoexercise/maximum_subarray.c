//find maximum son sum
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct{
	int begin;
	int end;
	int sum;
}Info;

Info *maxiarray(int *array,int length)
{
	int i,tmpsum=0;

	Info *pinfo = (Info *)malloc(sizeof(Info));
	bzero(pinfo,sizeof(Info));

	for(i = 0; i < length; i++)
	{
		tmpsum += array[i];
		if(tmpsum > pinfo->sum)
		{
			pinfo->sum = tmpsum;
			pinfo->end = i; 
		}
	}

	tmpsum = pinfo->sum;

	for(i = 0; i <= pinfo->end; i++)
	{
		tmpsum -= array[i];
		if(tmpsum > pinfo->sum)
		{
			pinfo->sum = tmpsum;
			pinfo->begin = i+1; 
		}
	}

	return pinfo;
}

int main()
{
	int A[] = {1,-2,3,10,-4,7,2,-5};
	Info *info = maxiarray(A,8);
	printf("%d\n",info->sum);
	free(info);
}
