/*
	Name - Vedant Puranik
	Class - TE-09
	Batch - M9
	Roll No - 33151
	Title - Max and Min
*/

#include<stdio.h>
int * arr;
int maxmin0[2];

void Maxmin(int i,int j,int maxmin[],int flag)
{
	int maxmin1[2];
	maxmin1[0]=maxmin1[1]=0; //Store temporary max and min
	int mid,k;
	if(i==j) //If only one element in subarray, set it to max and min
	{
		maxmin[0]=arr[i];
		maxmin[1]=arr[i];
		if(flag==1)
		{
			printf("\t\t\tmaxl::%d minl::%d",maxmin[0],maxmin[1]);
		}
		else if(flag==2)
		{
			printf("\t\tmaxr::%d minr::%d",maxmin[0],maxmin[1]);
		}
		else
		{
			printf("\t\tmax::%d min::%d",maxmin[0],maxmin[1]);
		}
	}
	else if(i==j-1) //If two elements in subarray, compare them and set to max and min
	{
		if(arr[i]<arr[j])
		{
			maxmin[0]=arr[j];
			maxmin[1]=arr[i];
		}
		else
		{
			maxmin[0]=arr[i];
			maxmin[1]=arr[j];
		}
		if(flag==1)
		{
			printf("\t\t\tmaxl::%d minl::%d",maxmin[0],maxmin[1]);
		}
		else if(flag==2)
		{
			printf("\t\tmaxr::%d minr::%d",maxmin[0],maxmin[1]);
		}
		else
		{
			printf("\t\tmax::%d min::%d",maxmin[0],maxmin[1]);
		}
	}
	else //More than 2 elements
	{
		mid=(i+j)/2; //Find middle of subarray
		printf("\n\n**********************************************LEFT PART**************************************************************\n");
		for(k=i;k<=mid;k++)
		{
			printf("%d\t",arr[k]);
		}
		Maxmin(i,mid,maxmin,1); //Find max and min for left subarray
		
		printf("\n\n**********************************************RIGHTT PART************************************************************\n\t\t");
		for(k=mid+1;k<=j;k++)
		{
			printf("%d\t",arr[k]);
		}
		Maxmin(mid+1,j,maxmin1,2); //Find max and min for right subarray
		
		//Compare max and min of two subarrays to set overall max and min
		if(maxmin[0]<maxmin1[0]) 
		{
			maxmin[0]=maxmin1[0];
		}
		if(maxmin[1]>maxmin1[1])
		{
			maxmin[1]=maxmin1[1];
		}
		printf("\n\n***********************************************MERGE STEP************************************************************\n");
		for(k=i;k<=j;k++)
		{
			printf("%d\t",arr[k]);
		}
		if(flag==1) //Print max and min for left subarray
		{
			printf("\t\t\tmaxl::%d minl::%d",maxmin[0],maxmin[1]);
		}
		else if(flag==2) //Print max and min for right subarray
		{
			printf("\t\tmaxr::%d minr::%d",maxmin[0],maxmin[1]);
		}
		else
		{
			printf("\t\tmax::%d min::%d",maxmin[0],maxmin[1]);
		}		
	}	
}
int main()
{
	int n,i;
	printf("\nEnter no of elements::");
	scanf("%d",&n);
	arr=(int *)malloc(n*sizeof(int));
	for(i=0;i<n;i++) //Input array
	{
		printf("\narr[%d]::",i);
		scanf("%d",&arr[i]);
	}	
	Maxmin(0,n-1,maxmin0,0); //Find maximum and minimum
	printf("\n\nmaximum::%d minimum::%d\n\n",maxmin0[0],maxmin0[1]);
}

/*

//OUTPUT

Enter no of elements::7

arr[0]::4

arr[1]::6

arr[2]::3

arr[3]::5

arr[4]::2

arr[5]::9

arr[6]::-2


**********************************************LEFT PART**************************************************************
4	6	3	5	

**********************************************LEFT PART**************************************************************
4	6				maxl::6 minl::4

**********************************************RIGHTT PART************************************************************
		3	5			maxr::5 minr::3

***********************************************MERGE STEP************************************************************
4	6	3	5				maxl::6 minl::3

**********************************************RIGHTT PART************************************************************
		2	9	-2	

**********************************************LEFT PART**************************************************************
2	9				maxl::9 minl::2

**********************************************RIGHTT PART************************************************************
		-2			maxr::-2 minr::-2

***********************************************MERGE STEP************************************************************
2	9	-2			maxr::9 minr::-2

***********************************************MERGE STEP************************************************************
4	6	3	5	2	9	-2			max::9 min::-2

maximum::9 minimum::-2

*/

