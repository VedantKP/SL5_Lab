/*
	Name - Vedant Puranik
	Class - TE-09
	Batch - M9
	Roll No - 33151
	Title - Recursive Descent Parser
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

/*
Grammar:

S -> TL
L -> +S | null
T -> UM
M -> *T | null
U -> (S) | V
V -> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
*/

int S(char str[30],int i);
int L(char str[30],int i);
int T(char str[30],int i);
int M(char str[30],int i);
int U(char str[30],int i);
int V(char str[30],int i);
void Parse(char str[30],int i);

int main()
{
	char str[30];
	printf("Enter string => ");
	scanf("%s",str);
	Parse(str,0);
	printf("\n");
	return 0;
}

int L(char str[30],int i)
{
	if(str[i]=='+')
	{
		i=S(str,i+1);
		return i;
	}
	else if(str[i]=='\0' || str[i]=='+' || str[i]=='*' || str[i]==')')
	{
		return i;
	}
	else
	{
		printf("\nRejected!");
		exit(0);
	}
}

int M(char str[30],int i)
{
	if(str[i]=='*')
	{
		i=T(str,i+1);
		return i;
	}
	else if(str[i]=='\0' || str[i]=='+' || str[i]=='*' || str[i]==')')
	{
		return i;
	}
	else
	{
		printf("\nRejected!");
		exit(0);
	}
}

int V(char str[30],int i)
{
	if(!isdigit(str[i]))
	{
		printf("\nRejected!");
		exit(0);
	}
	return i+1;
}

int U(char str[30],int i)
{
	if(str[i]=='(')
	{
		i=S(str,i+1);
		if(str[i]==')')
		{
			return i+1;
		}
		printf("\nRejected!");
			exit(0);
	}
	else
		i=V(str,i);
	return i;
}

int T(char str[30],int i)
{
	i=U(str,i);
	i=M(str,i);
	return i;
}

int S(char str[30],int i)
{
	i=T(str,i);
	i=L(str,i);
	return i;
}

void Parse(char str[30],int i)
{
	int ans = S(str,i);
	printf("\nAccepted!");
}



/*

//INPUT
Enter string => ((((4+(4*4)))))+(5*9)  

//OUTPUT
Accepted!
---------------------------------------------

//INPUT
Enter string => ((5+5)+3*2

//OUTPUT
Rejected!

*/
