/*
	Name - Vedant Puranik
	Class - TE-09
	Batch - M9
	Roll No - 33151
	Title - N-Queens
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int *x,count_ans=0;

//k represents row or queen no.
//n represents column no.
//i specifies the column in which the subsequent queen is placed

void display_result(int *x,int n)
{
	int iRow,iCol;
	count_ans++;
	printf("\nSolution : %d\n",count_ans);
	printf("\n");
	for(iRow=1;iRow<=n;iRow++) //Traverses rows
	{
		for(iCol=1;iCol<=n;iCol++) //Traverse Columns
		{
			if(x[iRow]==iCol) 
				printf(" Q "); //Queen exists
			else
				printf(" _ "); //Queen doesn't exist
		}
		printf("\n");
	}
	printf("\n");
}

int place(int k,int i)
{
	int j;
	for(j=1;j<=k-1;j++)
		if((x[j]==i) || (abs(x[j]-i)==abs(j-k))) //Check if no attack in the neighbouring positions and diagonals
			return 0;
	return 1;		
}

void N_Queens(int k,int n)
{
	int i,iter;
	for(i=1;i<=n;i++)
	{
		if(place(k,i)) //If new place found
		{
			x[k]=i; //Store queen position
			if(n==4) //Print output 4x4 chessboard
			{
				printf("\n::Positions of Queens:: Q1  Q2  Q3  Q4\n");
				printf("\t\t\t");
				for(iter=1;iter<=n;iter++)
					printf("%d   ",x[iter]);
				printf("\n");
			}					
			if(k==n)
				display_result(x,n); //display solution
			else
				N_Queens(k+1,n); //Solution not possible, look for other
		}
	}
}

int validateInput()
{
	int n;
	do
	{
		printf("\nEnter dimension of chessboard => ");
		scanf("%d",&n);
		if(n<=3)
			printf("\nInvalid dimsensions! Dimensions must have a value higher than 3!\n");
	}while(n<=3);
	return n;
}

int main()
{ 
	int n=0;
	n=validateInput();
	printf("\n---------------------------------------------------\n");
	printf("Implementing N-Queens' Algorithm for a %d x %d board",n,n);
	printf("\n---------------------------------------------------\n");
	x=(int*)malloc(n*sizeof(int));
	N_Queens(1,n);
	return 0;	
}



/*

OUTPUT

//INPUT
Enter dimension of chessboard => 4


//OUTPUT
---------------------------------------------------
Implementing N-Queens' Algorithm for a 4 x 4 board
---------------------------------------------------

::Positions of Queens:: Q1  Q2  Q3  Q4
			1   0   0   0   

::Positions of Queens:: Q1  Q2  Q3  Q4
			1   3   0   0   

::Positions of Queens:: Q1  Q2  Q3  Q4
			1   4   0   0   

::Positions of Queens:: Q1  Q2  Q3  Q4
			1   4   2   0   

::Positions of Queens:: Q1  Q2  Q3  Q4
			2   4   2   0   

::Positions of Queens:: Q1  Q2  Q3  Q4
			2   4   2   0   

::Positions of Queens:: Q1  Q2  Q3  Q4
			2   4   1   0   

::Positions of Queens:: Q1  Q2  Q3  Q4
			2   4   1   3   

Solution : 1

 _  Q  _  _ 
 _  _  _  Q 
 Q  _  _  _ 
 _  _  Q  _ 


::Positions of Queens:: Q1  Q2  Q3  Q4
			3   4   1   3   

::Positions of Queens:: Q1  Q2  Q3  Q4
			3   1   1   3   

::Positions of Queens:: Q1  Q2  Q3  Q4
			3   1   4   3   

::Positions of Queens:: Q1  Q2  Q3  Q4
			3   1   4   2   

Solution : 2

 _  _  Q  _ 
 Q  _  _  _ 
 _  _  _  Q 
 _  Q  _  _ 


::Positions of Queens:: Q1  Q2  Q3  Q4
			4   1   4   2   

::Positions of Queens:: Q1  Q2  Q3  Q4
			4   1   4   2   

::Positions of Queens:: Q1  Q2  Q3  Q4
			4   1   3   2   

::Positions of Queens:: Q1  Q2  Q3  Q4
			4   2   3   2 
---------------------------------------------------

//INPUT
Enter dimension of chessboard => 5


//OUTPUT
---------------------------------------------------
Implementing N-Queens' Algorithm for a 5 x 5 board
---------------------------------------------------

Solution : 1

 Q  _  _  _  _ 
 _  _  Q  _  _ 
 _  _  _  _  Q 
 _  Q  _  _  _ 
 _  _  _  Q  _ 


Solution : 2

 Q  _  _  _  _ 
 _  _  _  Q  _ 
 _  Q  _  _  _ 
 _  _  _  _  Q 
 _  _  Q  _  _ 


Solution : 3

 _  Q  _  _  _ 
 _  _  _  Q  _ 
 Q  _  _  _  _ 
 _  _  Q  _  _ 
 _  _  _  _  Q 


Solution : 4

 _  Q  _  _  _ 
 _  _  _  _  Q 
 _  _  Q  _  _ 
 Q  _  _  _  _ 
 _  _  _  Q  _ 


Solution : 5

 _  _  Q  _  _ 
 Q  _  _  _  _ 
 _  _  _  Q  _ 
 _  Q  _  _  _ 
 _  _  _  _  Q 


Solution : 6

 _  _  Q  _  _ 
 _  _  _  _  Q 
 _  Q  _  _  _ 
 _  _  _  Q  _ 
 Q  _  _  _  _ 


Solution : 7

 _  _  _  Q  _ 
 Q  _  _  _  _ 
 _  _  Q  _  _ 
 _  _  _  _  Q 
 _  Q  _  _  _ 


Solution : 8

 _  _  _  Q  _ 
 _  Q  _  _  _ 
 _  _  _  _  Q 
 _  _  Q  _  _ 
 Q  _  _  _  _ 


Solution : 9

 _  _  _  _  Q 
 _  Q  _  _  _ 
 _  _  _  Q  _ 
 Q  _  _  _  _ 
 _  _  Q  _  _ 


Solution : 10

 _  _  _  _  Q 
 _  _  Q  _  _ 
 Q  _  _  _  _ 
 _  _  _  Q  _ 
 _  Q  _  _  _ 

*/
