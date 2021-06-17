/*
	Name - Vedant Puranik
	Class - TE-09
	Batch - M9
	Roll No - 33151
	Title - Lexical Analyzer
*/

#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

char keywords[40][10] ={"auto","break","case","char","const","continue","default",
			 "do","double","else","enum","extern","float","for","goto",
			 "if","int","long","register","return","short","signed",
			 "sizeof","static","struct","switch","typedef","union",
			 "unsigned","void","volatile","while"				
			};
char operators[50][10]={
			 "+","-","*","/","%",					//arithmetic
			 "=","+=","-=","*=","/=","%=","&=","^=",		//assignment
			 ">","<",">=","<=","==","!=",				//relational
			 "&&","||","!",						//logical
			 "&","|","~","^","<<",">>",				//bitwise
			 "++","--",
			 "_","(", ")",";","$",":", "[","]","#","?","'","{","}",","
		   	};

typedef struct ust
{
	char sym_name[10];
	char class[5];
	int index;
}UST;
UST ust[10000];
char LT[50][10];
char IDN[50][10];
int ltind,idind,ustind;
int search_in_keywords(char buffer[])
{
	int i=0;
	while(strcmp(keywords[i],"")!=0)
	{
		if(strcmp(keywords[i],buffer)==0)
		{
			return i;
		}
		i++;
	}
	return -1;
}
int search_in_operators(char buffer[])
{
	int i=0;
	while(strcmp(operators[i],"")!=0)
	{
		if(strcmp(operators[i],buffer)==0)
		{
			return i;
		}
		i++;
	}
	return -1;
}
int search_in_lt(char buffer[])
{
	int i=0;
	while(i<ltind)
	{
		if(strcmp(LT[i],buffer)==0)
		{
			return i;
		}
		i++;
	}
	return -1;
}
int search_in_idn(char buffer[])
{
	int i=0;
	while(i<idind)
	{
		if(strcmp(IDN[i],buffer)==0)
		{
			return i;
		}
		i++;
	}
	return -1;
}
int isOperator(char ch)
{
	if(ch=='=' || ch=='+' || ch=='-' || ch=='/' || ch=='*')
		return 1;
	return 0;
}
int main()
{
	FILE *fp;
	char *buffer,ch,nextchar;
	int i;
	ltind=0,idind=0,ustind=0;
	int ind;
	fp=fopen("input.c","r");
	ch=fgetc(fp);
	while(!feof(fp))
	{
		//ch=fgetc(fp);
		//printf("\nchar::%c",ch);
		if(ch!='\n' && ch!='\t' && ch!=' ')
		{
			buffer=(char *)malloc(10*sizeof(char));
			if(isalpha(ch)&&!feof(fp))
			{
				i=0;	
				do 
				{
					buffer[i++]=ch;
					ch=getc(fp);			
				}while(isalnum(ch)&&!feof(fp));
				buffer[i]='\0';
				if(search_in_keywords(buffer)==-1&&search_in_idn(buffer)==-1)
				{
					strcpy(IDN[idind++],buffer);
				}	
			}
			else if(ch=='"' && !feof(fp))
			{
				i=0;
				buffer[i++]=ch;
				while((ch=getc(fp))!='"' && !feof(fp))
				{
					buffer[i++]=ch;
				}
				buffer[i++]='"';
				buffer[i]='\0';
				ch=fgetc(fp);
				strcpy(LT[ltind++],buffer);	
			}
			else if(ch=='=' && !feof(fp))
			{
				i=0;
				buffer[i]='=';
				ind=search_in_operators("=");
				strcpy(ust[ustind].sym_name,"=");
				strcpy(ust[ustind].class,"TRM");
				ust[ustind++].index=ind;
				while(!feof(fp) && isdigit(ch=fgetc(fp)))
				{
					buffer[i++]=ch;
				}
				if(i!=0)
					buffer[i]='\0';
				else
					buffer[1]='\0';
				if(strcmp(buffer,"=")!=0)
					strcpy(LT[ltind++],buffer);
			}
			else if(!feof(fp))
			{
				i=0;
				//printf("\nextra::%c",ch);
				buffer[i++]=ch;
				nextchar=fgetc(fp);
				if(isOperator(ch)&&isOperator(nextchar))
				{
					buffer[i++]=nextchar;
				}
				buffer[i]='\0';
				ch=nextchar;
			}
			if(buffer[0]=='"'||isdigit(buffer[0]))
			{
				ind=search_in_lt(buffer);
				strcpy(ust[ustind].sym_name,buffer);
				strcpy(ust[ustind].class,"LIT");
				ust[ustind++].index=ind;
			}
			else if(isalpha(buffer[0]))
			{
				ind=search_in_keywords(buffer);
				if(ind==-1)
				{
					ind=search_in_idn(buffer);
					strcpy(ust[ustind].sym_name,buffer);
					strcpy(ust[ustind].class,"IDN");
					ust[ustind++].index=ind;
				}
				else
				{
					strcpy(ust[ustind].sym_name,buffer);
					strcpy(ust[ustind].class,"TRM");
					ust[ustind++].index=ind;
				}
			}
			else if(buffer[0]!='=')
			{
				ind=search_in_operators(buffer);
				strcpy(ust[ustind].sym_name,buffer);
				strcpy(ust[ustind].class,"TRM");
				ust[ustind++].index=ind;
			}
			free(buffer);
		}
		else
		{
			ch=fgetc(fp);
		}
	}
	printf("\n::::::::::::::::::::::::LIT TAB::::::::::::::::::::::::::");
	for(i=0;i<ltind;i++)
	{
		printf("\n%s",LT[i]);
	}
	printf("\n::::::::::::::::::::::::::IDN:::::::::::::::::::::::::::::");
	for(i=0;i<idind;i++)
	{
		printf("\n%s",IDN[i]);
	}
	printf("\n::::::::::::::::::::::::::::UST::::::::::::::::::::::::::::");
	for(i=0;i<ustind;i++)
	{
		printf("\n%s\t%s\t%d",ust[i].sym_name,ust[i].class,ust[i].index);
	}
}


/*

//INPUT

main()
{
	int a,b,c,d;
	a=10;
	scanf("%d",&b);
	c=a+b;
	d=c/a;
	printf("%d %d",c,d);
}

//OUTPUT

::::::::::::::::::::::::LIT TAB::::::::::::::::::::::::::
10
"%d"
"%d %d"
::::::::::::::::::::::::::IDN:::::::::::::::::::::::::::::
main
a
b
c
d
scanf
printf
::::::::::::::::::::::::::::UST::::::::::::::::::::::::::::
main	IDN	0
(	TRM	31
)	TRM	32
{	TRM	41
int	TRM	16
a	IDN	1
,	TRM	43
b	IDN	2
,	TRM	43
c	IDN	3
,	TRM	43
d	IDN	4
;	TRM	33
a	IDN	1
=	TRM	5
10	LIT	0
;	TRM	33
scanf	IDN	5
(	TRM	31
"%d"	LIT	1
,	TRM	43
&	TRM	22
b	IDN	2
)	TRM	32
;	TRM	33
c	IDN	3
=	TRM	5
a	IDN	1
+	TRM	0
b	IDN	2
;	TRM	33
d	IDN	4
=	TRM	5
c	IDN	3
/	TRM	3
a	IDN	1
;	TRM	33
printf	IDN	6
(	TRM	31
"%d %d"	LIT	2
,	TRM	43
c	IDN	3
,	TRM	43
d	IDN	4
)	TRM	32
;	TRM	33
}	TRM	42

*/
