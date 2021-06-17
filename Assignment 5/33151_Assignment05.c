/*
	Name - Vedant Puranik
	Class - TE-09
	Batch - M9
	Roll No - 33151
	Title - Optimal Storage
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void Merge(int L[],int lb,int mid,int ub) //Function for merging two sorted sublists
{
	int n1=mid-lb+1,n2=ub-mid,i=0,j=0,k;
	int arr1[n1],arr2[n2];
	
	while(i<n1)
	{
		arr1[i]=L[lb + i];
		i++;
	}
	while(j<n2)
	{
		arr2[j]=L[mid+1+j];
		j++;
	}
	
	i=0,j=0,k=lb;
	while(i<n1 && j<n2) //Run loop till one of the array is depleted
	{
		if(arr1[i] <= arr2[j])	//Check which number is greater
			L[k++]=arr1[i++];
		else
			L[k++]=arr2[j++];
	}
	
	//Run loops till respective arrays are depleted
	while(i<n1)
		L[k++]=arr1[i++];
	while(j<n2)
		L[k++]=arr2[j++];	
}

void MergeSort(int L[],int lb,int ub)
{
	int mid;
	if(lb<ub)
	{
		mid=lb+(ub-lb)/2;
		MergeSort(L,lb,mid);
		MergeSort(L,mid+1,ub);
		Merge(L,lb,mid,ub);
	}
}

void PrintArray(int L[],int numberOfProg)
{
	for(int i=0;i<numberOfProg;i++)
	{
		printf("%d ",L[i]);
	}
}

double FindOptimalStorage(int L[],int numberOfProg,int numberOfTapes)
{
	
	int i,j,k=0;
	//int count[numberOfTapes]={0};
	int count[numberOfTapes];
	int progStore[numberOfTapes][4];
	//double MRT[numberOfTapes]={0};
	double MRT[numberOfTapes];
	double finalMRT = 0;
	MergeSort(L,0,numberOfProg);
	
	printf("\nPrograms in sorted order => ");
	PrintArray(L,numberOfProg);
	
	//Set initial count and MRT to zero
	for(i=0;i<numberOfTapes;i++)
	{
		count[i]=0;
		MRT[i]=0;
	}		
	
	j=0;
	i=0;
	k=0;
	//Store programs of sorted order into tapes represented by matrix
	while(k<numberOfProg)
	{
		progStore[i][j]=L[k];
		count[i]++;
		i=(i+1)%numberOfTapes;
		if(i==0)
			j=j+1;
		k++;
	}
	
	printf("\n");
	int sum=0;
	k=0;
	//Find MRT for each tape
	while(k<numberOfTapes)
	{
		sum=0;
		for(i=0;i<count[k];i++)
		{
			MRT[k] += progStore[k][i] + sum;
			sum=MRT[k];			
		}
		MRT[k] = MRT[k]/count[k];
		printf("\nMRT for tape %d is => %lf\n",k+1,MRT[k]);
		k++;
	}
	
	//Find overall MRT
	for(k=0;k<numberOfTapes;k++)
		finalMRT += MRT[k];
	
	return finalMRT/numberOfTapes;			
}

void Input(int L[],int numberOfProg)
{
	int i;
	for(i=0;i<numberOfProg;i++)
	{
		printf("\nProgram %d => ",i+1);
		scanf("%d",&L[i]);
	}
}

int main()
{
	//int *L;
	int numberOfProg,numberOfTapes;
	double optimalTime;
	printf("\nEnter number of programs => ");
	scanf("%d",&numberOfProg);
	int L[numberOfProg];
	printf("\nEnter number of tapes => ");
	scanf("%d",&numberOfTapes);
	printf("\nEnter lengths of programs (%d) => ",numberOfProg);
	//L = (int*)malloc(numberOfProg*sizeof(int));

	Input(L,numberOfProg);
	optimalTime=FindOptimalStorage(L,numberOfProg,numberOfTapes);	
	printf("\nOptimal Time for the given configuration is => %lf\n\n",optimalTime);
	
	return 0;
}


/*

//INPUT

Enter number of programs => 5

Enter number of tapes => 2

Enter lengths of programs (5) => 
Program 1 => 3

Program 2 => 6

Program 3 => 5

Program 4 => 9

Program 5 => 1


//OUTPUT

Programs in sorted order => 1 3 5 6 9 

MRT for tape 1 is => 7.666667

MRT for tape 2 is => 6.000000

Optimal Time for the given configuration is => 6.833333

*/
