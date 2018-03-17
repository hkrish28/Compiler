%{
# include <stdlib.h>
# include <ctype.h>
# include <string.h>
# include <stdio.h>
# include "y.tab.h"
# include "exptree.h"
int cnt=0,fileflag=0;
FILE *target_file;
char *temp;
int find(char *str)
{
	int i;
	char check[10];
	int addr;
	for(i=1;str[i]!='\0';i++);
	str[i]=':';
	str[i+1]='\0';
	FILE *rd=fopen("labeltr.txt","r");
	if(rd==NULL)
	{
		printf("error");
		exit(0);
	}
	fscanf(rd,"%s",check);
	fscanf(rd,"%d",&addr);
	while(strcmp(check,str)!=0)
	{
		fscanf(rd,"%s",check);
		fscanf(rd,"%d",&addr);
	}
	return addr;
}

%}

%%

L[0-9]+[:][\n]	{if(fileflag==0)
		 	{temp=strndup(yytext,yyleng-1);
			fprintf(target_file,"%s %d ",yytext,2048+2*cnt-8);}
		}
L[0-9]+		{if(fileflag==1)
		  {	
			temp=strndup(yytext,yyleng);
			int addr=find(temp);
			fprintf(target_file,"%d",addr);}
		}
[\n]		{
		if(fileflag==0)
			cnt=cnt+1;
		 else 
			{temp=strndup(yytext,yyleng);
			fprintf(target_file,"%s",temp);
			}		
		}	
.		{if(fileflag==1)
			{temp=strndup(yytext,yyleng);
			fprintf(target_file,"%s",temp);
			}
		}
%%

int yywrap(void)
{	
	if (fileflag==1)
		return 1;
	fileflag=1;
	fclose(target_file);
	fclose(yyin);
	yyin=fopen("exptree.xsm","r");
	target_file=fopen("final.xsm","w");
	return 0;	
}
int main()
{
	yyin=fopen("exptree.xsm","r");
	target_file=fopen("labeltr.txt","w");
	//printf("yylex");
	yylex();
}
