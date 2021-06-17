/*
	Name - Vedant Puranik
	Class - TE-09
	Batch - M9
	Roll No - 33151
	Title - Pass 2 of 2-pass Assembler
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

struct MOT
{
	char mneumonic[20],type[3];
	int opcode,no_operands,size;
};

typedef union SV{
	int size;
	char value[5];
}SV;

struct SYMTAB
{ 
	char symbol[20]; //stores symbol
	int address; //stores address
	int sr_no; //stores number of symbol
	SV len_value; //stores either value or size
	int flag; //used to indicate whether value or size - size: flag=0, value: flag=1 
};

struct LITTAB
{
	int address;
	char literal[5];
	int sr_no;
};

struct registers
{
	char name[5];
	int num;

};

struct MOT t[18];
struct SYMTAB sym_tab[18];
struct LITTAB lit_tab[18];
int POOLTAB[18];
struct registers reg[4];

//maintains count of symbols,literals and next pool location respectively
int sym_count=0,lit_count=0,pool_index=0;


void init_registers()
{
	strcpy(reg[0].name,"AREG");
	strcpy(reg[1].name,"BREG");
	strcpy(reg[2].name,"CREG");
	strcpy(reg[3].name,"DREG");
	
	reg[0].num=1;
	reg[1].num=2;
	reg[2].num=3;
	reg[3].num=4;
}

struct MOT findOp(char op[10])
{
	int i=0,mtp=0; //sets indexes to 1
	struct MOT mot;
	while(strcmp(t[mtp].mneumonic,op)!=0) //Searches through the MOT Table
	{
		mtp++;
		if(mtp==19)
			break;
	}
	if(mtp==19) //Entry not found (Error occured)
		mot.opcode=-1;
	else
	{
		mot=t[mtp]; //Entry stored in mot and returned
	}	
	return mot;
}

struct SYMTAB insert_into_symtab(char sym[10],int lc)
{
	//insert symbol structure in the last row of symtab (found from sym_count)
	//printf("Symbol is => %s\n",sym);
	//printf("sym_count is %d\n",sym_count);
	strcpy(sym_tab[sym_count].symbol,sym);
	//printf("Symbol entered in sym_tab is %s\n",sym_tab[sym_count].symbol);
	sym_tab[sym_count].address = lc;
	//printf("Location counter is %d\n",sym_tab[sym_count].address);
	sym_tab[sym_count].sr_no = sym_count+1;
	//printf("Serial number is %d\n",sym_tab[sym_count].sr_no);
	sym_count++;
	//printf("\nSym count is %d\n",sym_count);
	return sym_tab[sym_count-1]; //Increase symbol count by 1 after returning new symbol entry
}

struct SYMTAB findSymbol(char sym[10],int lc,int flag)
{
	int i=0,stp=0;
	struct SYMTAB sym_entry;
	int sym_index=0;
	if(sym_count==0) //SYMTAB is empty
	{
		sym_entry = insert_into_symtab(sym,lc);
	}
	else
	{	
		while(strcmp(sym_tab[stp].symbol,sym)!=0) //Search through SYMTAB for symbol
		{
			stp++;
			sym_index++;
			if(sym_index>sym_count)
			{
				break;	
			}	
		}
		if(sym_index>sym_count && flag==1) //Handles condition of label when occuring for the first time
		{
			//printf("Symbol is => %s\n",sym);
			sym_entry = insert_into_symtab(sym,lc);
			//printf("Sym_entry is: \n");
			//printf("%d\n%s\n%d\n",sym_entry.sr_no,sym_entry.symbol,sym_entry.address);
			
		}
		else if(sym_index<=sym_count && flag==1) //Handles condition of label already present in symbol table
		{
			//printf("\nAddress of symbol %s is => %d\n",sym_tab[stp].symbol,sym_tab[stp].address);
			if(sym_tab[stp].address==-1) //If location not assigned to label
				sym_tab[stp].address=lc;
			//printf("\nAddress of symbol %s is => %d\n",sym_tab[stp].symbol,sym_tab[stp].address);
			sym_entry=sym_tab[stp];
		}
		else if(sym_index<=sym_count && flag==0) //Handles condition for symbol as operand which already exists but not label
			sym_entry=sym_tab[stp];
		else if(sym_index>sym_count && flag==0) //Handles condition for symbol as operand not present in the symbol table
		{
			sym_entry = insert_into_symtab(sym,-1);	
		}
	}
	return sym_entry; //Return specific entry of the symbol
}

struct LITTAB findLiteral(char lit[10])
{
	//use lit_count for handling empty littab
	int i=0,ltp=0;
	struct LITTAB lit_entry;
	//while(strcmp(lit_tab[ltp++].literal,lit)!=0);
	
	if(lit_count==0) //LITTAB is empty
	{
		//Insert first entry into the LITTAB
		strcpy(lit_tab[0].literal,lit);
		lit_tab[0].address = -1; //Temporary address (wait for assignment by LTORG or END)
		lit_tab[0].sr_no = lit_count+1;
		lit_entry = lit_tab[0];

		lit_count++; 
		pool_index = 0; //Indicates current pool
		POOLTAB[0]=pool_index; //Set pool value

	}
	else
	{
		ltp=pool_index; //looks for literal in the current pool
		while(strcmp(lit_tab[ltp].literal,lit)!=0 && lit_tab[ltp].address==-1) //searches till matches
		{
			ltp++;
		}
		if(lit_tab[ltp].address==-1) //if match found
		{
			lit_entry = lit_tab[ltp];
		}
		else //if no match found
		{
			strcpy(lit_tab[ltp].literal,lit);
			lit_tab[ltp].address = -1;
			lit_tab[ltp].sr_no = lit_count+1;
			lit_entry = lit_tab[ltp];

			lit_count++;
		}
	}
	return lit_entry;
}

struct registers findRegister(char reg_input[10])
{
	int i=0,iter=0;
	struct registers reg_entry;
	while(strcmp(reg[iter].name,reg_input)!=0)
	{
		iter++;
		if(iter==4) break;
	}
	if(iter==4)
		reg_entry.num=-1;
	else
	{
		iter--;
		reg_entry=reg[iter];
	}
	return reg_entry;
}

void init_optab(char optab_list[1][18][7])
{
	int i;
	for(i=0;i<18;i++)
	{
		strcpy(t[i].mneumonic,optab_list[0][i]);
		if(strcmp(t[i].mneumonic,"STOP")==0 || strcmp(t[i].mneumonic,"ADD")==0 || strcmp(t[i].mneumonic,"SUB")==0 || strcmp(t[i].mneumonic,"MULT")==0 || strcmp(t[i].mneumonic,"MOVER")==0 || strcmp(t[i].mneumonic,"MOVEM")==0 || strcmp(t[i].mneumonic,"COMP")==0 || strcmp(t[i].mneumonic,"BC")==0 || strcmp(t[i].mneumonic,"DIV")==0 || strcmp(t[i].mneumonic,"READ")==0 || strcmp(t[i].mneumonic,"PRINT")==0) 
		{
			strcpy(t[i].type,"IS");
			if(strcmp(t[i].mneumonic,"STOP")==0)
			{
				t[i].opcode=0;
				t[i].no_operands=0;
			}
			if(strcmp(t[i].mneumonic,"ADD")==0)
			{
				t[i].opcode=1;
				t[i].no_operands=2;
			}
			if(strcmp(t[i].mneumonic,"SUB")==0)
			{
				t[i].opcode=2;
				t[i].no_operands=2;
			}
			if(strcmp(t[i].mneumonic,"MULT")==0)
			{
				t[i].opcode=3;
				t[i].no_operands=2;
			}
			if(strcmp(t[i].mneumonic,"MOVER")==0)
			{
				t[i].opcode=4;
				t[i].no_operands=2;
			}
			if(strcmp(t[i].mneumonic,"MOVEM")==0)
			{
				t[i].opcode=5;
				t[i].no_operands=2;
			}
			if(strcmp(t[i].mneumonic,"COMP")==0)
			{
				t[i].opcode=6;
				t[i].no_operands=2;
			}
			if(strcmp(t[i].mneumonic,"BC")==0)
			{
				t[i].opcode=7;
				t[i].no_operands=2;
			}
			if(strcmp(t[i].mneumonic,"DIV")==0)
			{
				t[i].opcode=8;
				t[i].no_operands=2;
			}
			if(strcmp(t[i].mneumonic,"READ")==0)
			{
				t[i].opcode=9;
				t[i].no_operands=1;
			}
			if(strcmp(t[i].mneumonic,"PRINT")==0)
			{
				t[i].opcode=10;
				t[i].no_operands=1;							
			}
			t[i].size=2;
		}		
		if(strcmp(t[i].mneumonic,"DS")==0 || strcmp(t[i].mneumonic,"DC")==0)
		{
			strcpy(t[i].type,"DL");
			if(strcmp(t[i].mneumonic,"DS")==0)
				t[i].opcode=1;
			if(strcmp(t[i].mneumonic,"DC")==0)
				t[i].opcode=2;	
			
			if(t[i].opcode == 01)
				t[i].size=0;
			else
				t[i].size=1;		
			t[i].no_operands=1;
		}
		if(strcmp(t[i].mneumonic,"START")==0 || strcmp(t[i].mneumonic,"END")==0 || strcmp(t[i].mneumonic,"LTORG")==0 || strcmp(t[i].mneumonic,"ORIGIN")==0 || strcmp(t[i].mneumonic,"EQU")==0)
		{
			strcpy(t[i].type,"AD");
			if(strcmp(t[i].mneumonic,"START")==0)
			{
				t[i].opcode=1;
				t[i].no_operands=1;
			}
			if(strcmp(t[i].mneumonic,"END")==0)
			{
				t[i].opcode=2;
				t[i].no_operands=0;
			}
			if(strcmp(t[i].mneumonic,"LTORG")==0)
			{
				t[i].opcode=3;
				t[i].no_operands=0;
			}
			if(strcmp(t[i].mneumonic,"ORIGIN")==0)
			{
				t[i].opcode=4;
				t[i].no_operands=1;
			}
			if(strcmp(t[i].mneumonic,"EQU")==0)
			{
				t[i].opcode=5;	
				t[i].no_operands=1;
			}
			t[i].size=0;
		}
	}
}

int assignAddrLiterals(int lc)
{
	int ltp = pool_index; //Look in the current pool
	//while(lit_tab[ltp].address!=-1) 
	//	ltp++;
	while(ltp<lit_count) //Assign addresses to all the literals in the current pool
	{
		lit_tab[ltp].address = lc++;
		ltp++;
	}
	return lc; //Return address for next instruction		
}

int extractAddress(char buff[10])
{
	int i=0,j=0,num=0,result=0;
	char symbol[5]; //Stores symbol before the operator 
	char operator = '+'; //Initial value of operator which may or may not occur
	char number[5]; //Stores number which may or may not occur
	
	while(isalnum(buff[i])>0) //extract symbol
	{
		symbol[i]=buff[i];
		i++;
	}
	if(buff[i]!='\0')
	{
		operator = buff[i++]; //extract operator
		while(isdigit(buff[i])>0) //extract number
		{
			number[j]=buff[i];
			i++;
			j++;
		}
		num=atoi(number);
	}	
	//printf("\nOperator for %s is => %c\n",buff,operator);
	struct SYMTAB sym_entry = findSymbol(symbol,0,0);
	switch(operator)
	{
		case '+':
				result = sym_entry.address + num;
				break;
		case '-':
				result = sym_entry.address - num;
				break;		
	}
	return result;
}

int pass_1()
{
	//Set all pointers, lc and counts to zero
	int lc=0,ptp=0,ltp=0,stp=0;
	int iter=0;
	int mtp=0;
	int size=0;
	
	int start_error=0,end_error=0,first_instruction=0,operand_error=0;
	
	FILE *f1,*f2; //file pointer to read from file
	char line[25],word[10];
	int count,i,j;
	char buff_1[10],buff_2[10],buff_3[10],buff_4[10]; //Buffers for 4 words (max)
	f2 = fopen("IC_Assembler_2_input.txt","w+");
	if(f1=fopen("assembler_input.txt","r")) //Only if file can be read
	{
		while(fgets(line,20,f1)) //Read one line from file
		{
			count=0;
			memset(word,0,sizeof(word)); //Buffer for storing each word
			memset(buff_1,0,sizeof(buff_1));
			memset(buff_2,0,sizeof(buff_2));
			memset(buff_3,0,sizeof(buff_3));
			memset(buff_4,0,sizeof(buff_4));
			i=0,j=0;
			while(line[i]!='\0')
			{	
				if(line[i]!=' ' && line[i]!='\n') //Till end of word or end of line
				{
					//printf("\nMoving %c from line to word!",line[i]);
					word[j++]=line[i];
				}
				//use strtok() for tokenizing
				else
				{
				//	printf("Word is => %s\n",word);
					//printf("Count is => %d\n",count);
					if(count==0)
						strcpy(buff_1,word); //Move data from source buffer to destination buffer
					else if(count==1)
						strcpy(buff_2,word);	
					else if(count==2)
						strcpy(buff_3,word);
					else if(count==3)
						strcpy(buff_4,word);
					count++; //Maintain word count		 
					j=0;
					//printf("\nWord %d over!",count);
					memset(word,0,sizeof(word)); //Clear temporary buffer for next line
				}
				i++;
			}
			//printf("Buff_1 is => %s\n",buff_1);
			//printf("Buff_2 is => %s\n",buff_2);
			//printf("Buff_3 is => %s\n",buff_3);
			//printf("Buff_4 is => %s\n",buff_4);
			if(buff_1[0]!='\0')
				printf("%-7s",buff_1);
			else
				printf("%-7s","");
			if(buff_2[0]!='\0')
				printf("%-7s",buff_2);
			else
				printf("%-7s","");	
			if(buff_3[0]!='\0')
				printf("%-7s",buff_3);
			else
				printf("%-7s","");	
			if(buff_4[0]!='\0')
				printf("%-7s",buff_4);
			else
				printf("%-7s","");
			//printf("\t");	
			if(strcmp(buff_1,"START")==0 || strcmp(buff_1,"ORIGIN")==0)
			{
				if(strcmp(buff_1,"START")==0)
				{
					if(start_error == 0)
					{
						start_error=2;
						//printf("%s",buff_2);
						lc=atoi(buff_2); //Convert operand to integer to set lc
						//printf("\nBuff_1 for START is => %s\n",buff_1);
						struct MOT optab_entry = findOp(buff_1); 
						//printf("Optab entry type for START => %s\n",optab_entry.type);
						printf("00 (%s %d) (C,%d)\n",optab_entry.type,optab_entry.opcode,lc);
						fprintf(f2,"(%s %d) (C,%d)\n",optab_entry.type,optab_entry.opcode,lc);
					}
					else
						printf("\nError in START! START Location invalid");	
				}
				if(strcmp(buff_1,"ORIGIN")==0)
				{
					if(start_error == 2)
					{
						int temp_lc = lc; //Stores address at which instruction occured as lc will be changed later
						int address_from_string;
						if(isalpha(buff_2[0])) //If not just a number
						{
							lc = extractAddress(buff_2); //Extract address from string
						}
						else
							lc=atoi(buff_2); //If just a number
						struct MOT optab_entry = findOp(buff_1);
						if(isalpha(buff_2[0]) && lc>0)
						{
							printf("%d (%s %d) (S,%d)\n",temp_lc,optab_entry.type,optab_entry.opcode,lc);
							fprintf(f2,"(%s %d) (S,%d)\n",optab_entry.type,optab_entry.opcode,lc);
						}
						else if(!isalpha(buff_2[0]))
						{
							printf("%d (%s %d) (C,%d)\n",temp_lc,optab_entry.type,optab_entry.opcode,lc);
							fprintf(f2,"(%s %d) (C,%d)\n",optab_entry.type,optab_entry.opcode,lc);	
						}
						else
						{
							printf("Error! Operand does not have address!\n");
							first_instruction=1;
						}
					}
					else
					{
						printf("Invalid location of ORIGIN\n");
						start_error = 1;
					}
				}
			}
			else if(count==3 && (strcmp(buff_2,"DS")==0 || strcmp(buff_2,"DC")==0))
			{
				if(start_error==2)
				{
					if((strcmp(buff_2,"DS")==0))
					{	
						struct SYMTAB sym_entry = findSymbol(buff_1,lc,1); //Find and insert
						sym_tab[sym_entry.sr_no-1].flag=0; //Specifies DS
						size = atoi(buff_3);
					
						sym_tab[sym_entry.sr_no-1].len_value.size=size; //Stores size of symbol
						
						//compute IC
						while(strcmp(t[mtp++].mneumonic,"DS")!=0);
						mtp--;
						
						//print IC
						printf("%d (%s %d) (C,%d)\n",lc,t[mtp].type,t[mtp].opcode,size);
						fprintf(f2,"(%s %d) (C,%d)\n",t[mtp].type,t[mtp].opcode,size);
						lc += size; //Increments location counter by size
						mtp=0;
					}
					if((strcmp(buff_2,"DC")==0))
					{
						//printf("buff_1 is => %s\n",buff_1);
						struct SYMTAB sym_entry = findSymbol(buff_1,lc,1); //Find and insert
						//printf("Sym_entry is: \n");
						//printf("%d\n%s\n%d\n",sym_entry.sr_no,sym_entry.symbol,sym_entry.address);
						
						sym_tab[sym_entry.sr_no-1].flag=1; //Specifies DC
						strcpy(sym_tab[sym_entry.sr_no-1].len_value.value,buff_3); //Stores constant for symbol 
						//lc++;
						
						//printf("Value of %s => %s\n",sym_entry.symbol,sym_tab[sym_entry.sr_no-1].len_value.value);
						while(strcmp(t[mtp++].mneumonic,"DC")!=0);
						mtp--;
						printf("%d (%s %d) (C,%s)\n",lc,t[mtp].type,t[mtp].opcode,buff_3);
						fprintf(f2,"(%s %d) (C,%s)\n",t[mtp].type,t[mtp].opcode,buff_3);
						lc++;
						mtp=0;
					}
				}
				else
				{
					printf("Error in processing DS/DC statement!");
					start_error = 1;
				}	
			}
			else if(strcmp(buff_1,"END")==0)
			{
				if(start_error==2)
				{
					while(strcmp(t[mtp++].mneumonic,"END")!=0);
					mtp--;
					printf("%d (%s %d)",lc,t[mtp].type,t[mtp].opcode);
					fprintf(f2,"(%s %d)",t[mtp].type,t[mtp].opcode);
					mtp=0;
					if(lit_count!=0) //If non-zero literals only then assign addresses
						lc = assignAddrLiterals(lc);
					//printf("\nEnd reached!");
					end_error=1;
					if(count>1)
					{
						printf("Wrong number of operands for END Statement");
						operand_error = 1;
					}
				}
				else
				{
					printf("\nInvalid location of END statement");
					start_error = 1;
				}
			}
			//Imperative Statement
			else if(count == 4 && (strcmp(buff_2,"MOVER")==0 || strcmp(buff_2,"MOVEM")==0 || strcmp(buff_2,"ADD")==0 || strcmp(buff_2,"SUB")==0 || strcmp(buff_2,"MULT")==0 || strcmp(buff_2,"DIV")==0 || strcmp(buff_2,"BC")==0 || strcmp(buff_2,"COMP")==0))
			{
				//printf("\nIn count 4");
				if(start_error==2)
				{
					char sym_or_lit = 'S';
					int pos=0;

					//buff_1
					struct SYMTAB sym_entry1 = findSymbol(buff_1,lc,1);

					//buff_2
					struct MOT optab_entry = findOp(buff_2);
					if(strcmp(optab_entry.type,"IS")==0)
					{
						//struct SYMTAB sym_entry = findSymbol(buff_4);
					
						//buff_3
						struct registers reg_entry = findRegister(buff_3);

						struct SYMTAB sym_entry2;
						struct LITTAB lit_entry;
						//printf("\nInside count = 4\n");
						//buff_4
						//printf("\nbuff_4 is => %s",buff_4);
						if(buff_4[0] == '=')
						{
							sym_or_lit = 'L';
							lit_entry = findLiteral(buff_4);
						}
						else
						{
							sym_or_lit = 'S';
							sym_entry2 = findSymbol(buff_4,lc,0);							
						}	


						if(sym_or_lit == 'S' && sym_entry2.address!=-1)
						{
							printf("%d (IS %d) (R %d) (%c,%d)\n",lc,optab_entry.opcode,reg_entry.num + 1,sym_or_lit,sym_entry2.sr_no);	
							fprintf(f2,"(IS %d) (R %d) (%c,%d)\n",optab_entry.opcode,reg_entry.num + 1,sym_or_lit,sym_entry2.sr_no);
						}
						else if(sym_or_lit == 'L')
						{
							printf("%d (IS %d) (R %d) (%c,%d)\n",lc,optab_entry.opcode,reg_entry.num + 1,sym_or_lit,lit_entry.sr_no);
							fprintf(f2,"(IS %d) (R %d) (%c,%d)\n",optab_entry.opcode,reg_entry.num + 1,sym_or_lit,lit_entry.sr_no);
						}
						else
						{
							printf("Address of symbol is not valid!");
							first_instruction = 1;
						}
						lc++;
					}
					else
					{
						printf("\nMnemonic not allowed!\n");
						exit(0);
					}
				}
				else
				{
					printf("\nInvalid location of IS statement");	
					start_error = 1;
				}
			}
			else if(count == 3 && (strcmp(buff_2,"DS")!=0 && strcmp(buff_2,"DC")!=0 && strcmp(buff_2,"EQU")!=0))
			{
				if(start_error == 2)
				{
					struct MOT mot = findOp(buff_1);
					if(mot.opcode==-1) //If starts with symbol and not mnemonic
					{
						struct SYMTAB sym_entry1 = findSymbol(buff_1,lc,1); //Find and insert
						struct MOT optab_entry = findOp(buff_2); //Find mnemonic of instruction
						if(optab_entry.opcode==-1) //If none found then error
						{
							printf("\nMnemonic not allowed!");
							exit(0);
						}
						struct SYMTAB sym_entry2 = findSymbol(buff_3,lc,0); //Find and insert
						//if(sym_entry2.address!=-1)
						printf("%d (%s %d) (S %d)\n",lc,optab_entry.type,optab_entry.opcode,sym_entry2.sr_no);
						fprintf(f2,"(%s %d) (S %d)\n",optab_entry.type,optab_entry.opcode,sym_entry2.sr_no);
						//else
						//{
						//	printf("Error in number of operands!\n");
						//	operand_error=1;
						//}
					}
					else
					{
						//If starts with mnemonic
						char sym_or_lit;
						struct registers reg_entry = findRegister(buff_2);
						struct SYMTAB sym_entry2;
						struct LITTAB lit_entry;
						//buff_4
						if(buff_3[0] == '=')
						{
							sym_or_lit = 'L';
							lit_entry = findLiteral(buff_3);
						}
						else
						{
							sym_or_lit = 'S';
							sym_entry2 = findSymbol(buff_3,lc,0);							
						}	
						
						if(sym_or_lit == 'S')
						{
							printf("%d (IS %d) (R %d) (%c,%d)\n",lc,mot.opcode,reg_entry.num + 1,sym_or_lit,sym_entry2.sr_no);	
							fprintf(f2,"(IS %d) (R %d) (%c,%d)\n",mot.opcode,reg_entry.num + 1,sym_or_lit,sym_entry2.sr_no);
						}
						else if(sym_or_lit == 'L')
						{
							printf("%d (IS %d) (R %d) (%c,%d)\n",lc,mot.opcode,reg_entry.num + 1,sym_or_lit,lit_entry.sr_no);
							fprintf(f2,"(IS %d) (R %d) (%c,%d)\n",mot.opcode,reg_entry.num + 1,sym_or_lit,lit_entry.sr_no);
						}
					}
					lc++;
				}
				else
				{
					printf("\nInvalid location of IS statement");	
					start_error = 1;
				}	
			}
			else if(strcmp(buff_2,"EQU")==0 && count==3)
			{
				//printf("\nInside EQU\n");
				if(start_error == 2)
				{
					int value_of_lc; //Value of address of operand is stored here
					struct MOT optab_entry = findOp(buff_2); //Find MOT entry
					if(isalpha(buff_3[0])) //If not just a number
					{
						value_of_lc = extractAddress(buff_3);
					}
					else
						value_of_lc=atoi(buff_3); 
					struct SYMTAB sym_entry = findSymbol(buff_1,value_of_lc,1);
					if(isalpha(buff_3[0]) && value_of_lc>0)
					{
						printf("%d (%s %d) (S,%d)\n",lc,optab_entry.type,optab_entry.opcode,value_of_lc);
						fprintf(f2,"(%s %d) (S,%d)\n",optab_entry.type,optab_entry.opcode,value_of_lc);
					}
					else if(!isalpha(buff_3[0]))
					{
						printf("%d (%s %d) (C,%d)\n",lc,optab_entry.type,optab_entry.opcode,value_of_lc);
						fprintf(f2,"(%s %d) (C,%d)\n",optab_entry.type,optab_entry.opcode,value_of_lc);
					}
					else
					{
						printf("Error! Operand does not have address!\n");
						first_instruction=1;
					}
					//lc++;
				}
				else
				{
					printf("\nInvalid location of IS statement");	
					start_error = 1;
				}	
			}
			else if(strcmp(buff_1,"LTORG")==0 || strcmp(buff_2,"LTORG")==0)
			{
				if(start_error == 2)
				{
					if(strcmp(buff_2,"LTORG")==0) //If label and LTORG both exist
					{
						struct MOT optab_entry = findOp(buff_2); //Find MOT Entry
						struct SYMTAB sym_entry = findSymbol(buff_1,lc,1); //Find and insert
						printf("%d (%s %d) (S %d)\n",lc,optab_entry.type,optab_entry.opcode,sym_entry.sr_no);
						fprintf(f2,"(%s %d) (S %d)\n",optab_entry.type,optab_entry.opcode,sym_entry.sr_no);
					}
					else
					{
						//Only LTORG without label
						struct MOT optab_entry = findOp(buff_1);					
						printf("%d (%s %d)\n",lc,optab_entry.type,optab_entry.opcode);
						fprintf(f2,"(%s %d)\n",optab_entry.type,optab_entry.opcode);
					}
					if(lit_count!=0)
					{
						//Assign addresses to label in the current pool only if literal count is non-zero
						lc = assignAddrLiterals(lc);
						pool_index = lit_count;
						int pool_entry=0;
						while(isdigit(POOLTAB[pool_entry])!=0)//while pooltab has valid entries for diff pools, iterate
							pool_entry++;
						POOLTAB[pool_entry]=pool_index;
					}
				}
				else
				{
					printf("\nInvalid location of IS statement");	
					start_error = 1;
				}	
			}
			else if(count == 1 && strcmp(buff_1,"STOP")==0)
			{
				if(start_error == 2)
				{
					struct MOT optab_entry = findOp(buff_1);
					printf("%d (%s %d)\n",lc,optab_entry.type,optab_entry.opcode);
					fprintf(f2,"(%s %d)\n",optab_entry.type,optab_entry.opcode);
					lc++;
				}
				else
				{
					printf("\nInvalid location of IS statement");	
					start_error = 1;
				}	
			}
			else
			{
				printf("Error in number of operands!\n");
				operand_error = 1;
			}	
		}
		fclose(f1);
		fclose(f2);
	}
	else
		printf("\nCannot read from file!\n");

	if(end_error!=1)
		printf("\nEND Statement not found!");

	if(start_error == 2 && end_error == 1 && operand_error == 0 && first_instruction == 0)
		return 1;
	return 0;	
}

void print_tables()
{
	int i;
	printf("\n\n****SYMTAB****\n");
	printf("|Sr_No\t   |Symbol    |Address   |Size/Value|\n");
	for(i=0;i<sym_count;i++)
	{
		printf("|%-10d|%-10s|%-10d|",sym_tab[i].sr_no,sym_tab[i].symbol,sym_tab[i].address);
		if(sym_tab[i].flag==1)
			printf("%-10s|",sym_tab[i].len_value.value);
		else if(sym_tab[i].flag==0)
			printf("%-10d|",sym_tab[i].len_value.size);
		else
			printf("-|");	
		printf("\n");				
	}
	printf("\n****LITTAB****\n");
	printf("|Sr_No\t   |Literal   |Address   |\n");
	for(i=0;i<lit_count;i++)
	{
		printf("|%-10d|%-10s|%-10d|\n",lit_tab[i].sr_no,lit_tab[i].literal,lit_tab[i].address);
	}
}

void pass_2()
{
	int i=0,j=0,k=0,iter=0,lc=0,pool_index=0,from=0,to=0,size=0;
	FILE* f2 = fopen("IC_Assembler_2_input.txt","r");
	char line[27],word[10];
	int number,reg_number,sym_addr,lit_addr,number1;
	char buff_1[3],buff_2,buff_3,buff_4[5]; 
	char buff_num[3];
	while(fgets(line,25,f2)) //Read one line at a time till EOF
	{
		//Clear all buffers
		memset(buff_1,0,sizeof(buff_1));
		memset(buff_4,0,sizeof(buff_4));
		memset(buff_num,0,sizeof(buff_num));

		//Read first two character
		i=0;
		buff_1[0] = line[++i];
		buff_1[1] = line[++i];
		
		while(!isdigit(line[i])) //Traverse line till digit
			i++;
		
		j=0;	
		//Fill digits in buffer (opcode)
		while(isdigit(line[i]))
		{
			buff_num[j]=line[i];
			i++;
			j++;
		}
		number = atoi(buff_num); //Find opcode in MOT	
		j=0;
		if(buff_1[0]=='A') //If AD instruction
		{
			for(j=0;j<18;j++)
			{
				if(strcmp(t[j].type,"AD")==0 && t[j].opcode==number) //Find entry from MOT
					break;	
			}
			
			while(!isdigit(line[i]))
				i++;
			
			k=0;
			while(isdigit(line[i])) //Store digits in buffer
			{
				buff_4[k]=line[i];
				i++;
				k++;
			}
			number1 = atoi(buff_4);
	
			//Check for instruction and print output
			if(strcmp(t[j].mneumonic,"START")==0 || strcmp(t[j].mneumonic,"ORIGIN")==0)
			{
				printf("%d\n",lc);
				lc=number1;
			}
			else if(strcmp(t[j].mneumonic,"EQU")==0)
			{
				printf("%d\n",lc);	
			}
			else if(strcmp(t[j].mneumonic,"LTORG")==0 || strcmp(t[j].mneumonic,"END")==0)
			{
				from = POOLTAB[pool_index];
				if(isdigit(POOLTAB[pool_index+1])) //Print addresses of all literals in the current pool 
				{
					to = POOLTAB[pool_index+1];
					for(iter = from;iter<to;iter++)
					{
						printf("%d  %s\n",lc,lit_tab[iter].literal);
						lc++;
					}
					pool_index = pool_index+1;
				}
				else
				{
					for(iter = from;iter<lit_count;iter++) //Print addresses of all literals in the last pool
					{
						printf("%d  %s\n",lc,lit_tab[iter].literal);
						lc++;
					}
				}
			}		
		}
		else if(buff_1[0]=='I') //If instruction of type IS
		{
			
			printf("%d + %d ",lc,number);
			
			while(line[i]!='(')
				i++;
			i++;
			
			buff_2 = line[i];
			if(buff_2 == 'R') //Find register number
			{
				while(!isdigit(line[i]))
					i++;
				number = line[i]-'0';
				printf("%d ",number);		
			}
			if(buff_2 == 'S') ///Find symbol address
			{
				while(!isdigit(line[i]))
					i++;
				number = line[i] - '0';
				sym_addr = sym_tab[number-1].address;
				printf("%d ",sym_addr);
			}
			if(buff_2 == 'L') //Find literal address
			{
				while(!isdigit(line[i]))
					i++;
				number = line[i] - '0';
				lit_addr = lit_tab[number-1].address;
				printf("%d ",lit_addr);
			}
			//If length is 4 repeat process for symbol and literal
			if(line[i+3]=='(')
			{
				while(line[i]!='(')
					i++;
				i++;
				buff_3 = line[i];
				if(buff_3 == 'S')
				{	
					while(!isdigit(line[i]))
						i++;
					number = line[i] - '0';
					sym_addr = sym_tab[number-1].address;
					printf("%d ",sym_addr);
				}
				if(buff_3 == 'L')
				{
					while(!isdigit(line[i]))
						i++;
					number = line[i] - '0';
					lit_addr = lit_tab[number-1].address;
					printf("%d ",lit_addr);
				}	
			}	
			printf("\n");
			lc++;				
		}
		else if(buff_1[0]=='D') //If instruction of type DL
		{
			for(j=0;j<18;j++)
			{
				if(strcmp(t[j].type,"DL")==0 && t[j].opcode==number)
					break;
			}
		 	
			if(strcmp(t[j].mneumonic,"DC")==0) //Instruction is DC
			{
				while(line[i]!='\'')
					i++;
				
				j=0;
				while(line[i]!=')')
				{
					buff_4[j]=line[i];
					i++;
					j++;
				}	
				
				j=0;
				while(strcmp(sym_tab[j].len_value.value,buff_4)!=0)
					j++;
					
				printf("%d + %s\n",lc,sym_tab[j].len_value.value);
				lc++;	
			}	
			else if(strcmp(t[j].mneumonic,"DS")==0) //Instruction is DS
			{
				while(!isdigit(line[i]))
					i++;
				
				j=0;
				while(isdigit(line[i]))
				{
					buff_4[j]=line[i];
					i++;
					j++;
				}
				
				size=atoi(buff_4);
				
				printf("%d + %d\n",lc,size);	
				lc+=size;
			}
		}		
	}
}

int main()
{
	int lc=0,ptp=1,rc=0,flag=0;
	char optab_list[1][18][7]={"STOP","ADD","SUB","MULT","MOVER","MOVEM","COMP","BC","DIV","READ","PRINT","DC","DS","START","END","LTORG","ORIGIN","EQU"}; 
	init_optab(optab_list);
	init_registers();
	int i;
	printf("\n****MOT TABLE****\n");
	printf("\nMnemonic\tOpcode\tClass\tSize\n");
	for(i=0;i<18;i++)
	{
		printf("%s\t\t%d\t%s\t%d\n",t[i].mneumonic,t[i].opcode,t[i].type,t[i].size);
	}	
	printf("\nContents of file are =>\n\n");
	i=0;
	flag=pass_1(); 
	if(flag==1)
		print_tables();
	printf("\n");
	printf("\nPass 2 output => \n");
	pass_2();
	return 0;	
}

/*

//INPUT

START 100
A DS 3
L1 MOVER AREG ='8'
ADD AREG C
MOVEM AREG D
D EQU A+1
L2 PRINT D
ORIGIN A-1
C DC '5'
ORIGIN L2+1
STOP
B DC '19'
END

//OUTPUT

****MOT TABLE****

Mnemonic	Opcode	Class	Size
STOP		0	IS	2
ADD		1	IS	2
SUB		2	IS	2
MULT		3	IS	2
MOVER		4	IS	2
MOVEM		5	IS	2
COMP		6	IS	2
BC		7	IS	2
DIV		8	IS	2
READ		9	IS	2
PRINT		10	IS	2
DC		2	DL	1
DS		1	DL	0
START		1	AD	0
END		2	AD	0
LTORG		3	AD	0
ORIGIN		4	AD	0
EQU		5	AD	0

Contents of file are =>

START  100                  00 (AD 1) (C,100)
A      DS     3             100 (DL 1) (C,3)
L1     MOVER  AREG   ='8'   103 (IS 4) (R 1) (L,1)
ADD    AREG   C             104 (IS 1) (R 1) (S,3)
MOVEM  AREG   D             105 (IS 5) (R 1) (S,4)
D      EQU    A+1           106 (AD 5) (S,101)
L2     PRINT  D             106 (IS 10) (S 4)
ORIGIN A-1                  107 (AD 4) (S,99)
C      DC     '5'           99 (DL 2) (C,'5')
ORIGIN L2+1                 100 (AD 4) (S,107)
STOP                        107 (IS 0)
B      DC     '19'          108 (DL 2) (C,'19')
END                         109 (AD 2)

****SYMTAB****
|Sr_No	   |Symbol    |Address   |Size/Value|
|1         |A         |100       |3         |
|2         |L1        |103       |0         |
|3         |C         |99        |'5'       |
|4         |D         |101       |0         |
|5         |L2        |106       |0         |
|6         |B         |108       |'19'      |

****LITTAB****
|Sr_No	   |Literal   |Address   |
|1         |='8'      |109       |


Pass 2 output => 
0
100 + 3
103 + 4 1 109 
104 + 1 1 99 
105 + 5 1 101 
106
106 + 10 101 
107
99 + '5'
100
107 + 0 
108 + '19'
109  ='8'

*/
