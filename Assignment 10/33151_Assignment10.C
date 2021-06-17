/*
	Name - Vedant Puranik
	Class - TE-09
	Batch - M9
	Roll No - 33151
	Title - Travelling Salesman Problem
*/

#include<stdio.h>
int a[10][10],visited[10],n,cost=0;
int least(int c);


void mincost(int city)
{
	int i,ncity;
	visited[city]=1;	
	printf("%d -->",city+1);
	ncity=least(city);
	if(ncity==999)
	{
		ncity=0;
		printf("%d",ncity+1);
		cost+=a[city][ncity];
		return;
	}
	mincost(ncity);
}

int least(int c)
{
	int i,nc=999;
	int min=999,kmin;
	for(i=0;i < n;i++)
	{
		if((a[c][i]!=0)&&(visited[i]==0))
			if(a[c][i] < min)
			{
				min=a[i][0]+a[c][i];
				kmin=a[c][i];
				nc=i;
			}
	}
	if(min!=999)
		cost+=kmin;
	return nc;
}

int main()
{

	int i,j;
	printf("Enter the number of cities:: ");
	scanf("%d",&n);
	printf("\nEnter the cost matrix:: ");
		for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			printf("\nEnter the distance between city %d and city %d :: ", i+1, j+1);
		scanf("%d",&a[i][j]);
		visited[i]=0;
		}
	}

	printf("\n\nThe cost matrix is:\n\n");
	for( i=0;i < n;i++)
	{
	
		for(j=0;j < n;j++)
			printf("\t%d",a[i][j]);
		printf("\n");
	}

	printf("\n\nThe Path is:\t\t");
	mincost(0);
		printf("\n\nMinimum cost ::");
	printf("%d",cost);
	

}

/*

//INPUT

Enter the number of cities:: 4

Enter the cost matrix:: 
Enter the distance between city 1 and city 1 :: 0

Enter the distance between city 1 and city 2 :: 3

Enter the distance between city 1 and city 3 :: 5

Enter the distance between city 1 and city 4 :: 2

Enter the distance between city 2 and city 1 :: 3

Enter the distance between city 2 and city 2 :: 0

Enter the distance between city 2 and city 3 :: 8

Enter the distance between city 2 and city 4 :: 4

Enter the distance between city 3 and city 1 :: 5

Enter the distance between city 3 and city 2 :: 8

Enter the distance between city 3 and city 3 :: 0

Enter the distance between city 3 and city 4 :: 7

Enter the distance between city 4 and city 1 :: 2

Enter the distance between city 4 and city 2 :: 4

Enter the distance between city 4 and city 3 :: 7

Enter the distance between city 4 and city 4 :: 0



//OUTPUT

The cost matrix is:

	0	3	5	2
	3	0	8	4
	5	8	0	7
	2	4	7	0


The Path is:		1 -->4 -->2 -->3 -->1

Minimum cost ::19

*/
