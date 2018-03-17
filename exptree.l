%{
# include <stdlib.h>
# include <stdio.h>
# include "y.tab.h"
# include "exptree.h"
int val,fileflag=0;
%}

%%
";"		{return *yytext;}
begin 		{return BEG;}
end 		{return END;}
decl		{return DECL;}
enddecl		{return ENDDECL;}
int		{return INT;}
str		{return STR;}

read		{return READ;}
write 		{return WRITE;}

if		{return IF;}
then		{return THEN;}
else		{return ELSE;}
endif		{return ENDIF;}

repeat		{return REPEAT;}
until		{return UNTIL;}
while		{return WHILE;}
do		{return DO;}
endwhile	{return ENDWHILE;}
break		{return BREAK;}
continue	{return CONTINUE;}

[a-zA-Z][a-zA-Z0-9]*	{
			char *temp=strndup(yytext,yyleng);
		
			yylval = createVarNode(temp);
		
			return ID;
		}

["][a-zA-Z0-9:,. \t\n]*["]	{
			char *temp=strndup(yytext,yyleng);
		
			yylval = createStrNode(temp);
		
			return STR;

			}

[0-9]+ 		{
			val = atoi(yytext);
			yylval = createNumNode(val);
			return NUM;
		}


"+" 		{return PLUS;}

"*"			{return MUL;}

"-"			{return MINUS;}

"/"			{return DIV;}

"="			{return EQ;}

[ ]		{}
[\t]		{}
[\n]		{}

[()\[\]]		{return *yytext;}
","		{return *yytext;}
"<="		{return LTE;}
"!="		{return NEQ;}
"=="		{return ISEQ;}
">="		{return GTE;}
"<"		{return LT;}
">"		{return GT;}
"&"		{return *yytext;}


.			{yyerror("unknown character\n"); exit(1);}

%%
int yywrap()
{	
	return 1;
}




