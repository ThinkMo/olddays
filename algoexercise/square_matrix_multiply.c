//nxn martix multiply  O(n3)
//int a[M][N] or int a[][N] or int (*a)[N]  through function
//int *a[N]  and int (*a)[N]  different
#include <string.h>
#include <stdio.h>

#define N 2

void square_martix_multiply(int A[][N],int B[][N],int out[][N],int n)
{
	memset(out,0,sizeof(int)*n*n);

	int i,j,k;
	
	for(i = 0; i < n; i++)
	  for(j = 0; j < n; j++)
	  {
		  out[i][j] = 0;
		  for(k = 0; k < n; k++)
			out[i][j] += A[i][k] * B[k][j];
	  }
}

int main() //test
{

	int A[2][2] ={{1,2},{3,4}};
	int B[2][2] ={1,2,3,4};
	int C[2][2];

	square_martix_multiply(A,B,C,N);

	int i,j;
	for(i = 0; i < N; i++)
	{
	  for(j = 0; j < N; j++)
		printf("%d\t",C[i][j]);
	  printf("\n");
	}
}
