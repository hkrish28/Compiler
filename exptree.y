%{
	#include <stdio.h>
	#include <stdlib.h>
	#include<string.h>
	#define YYSTYPE tnode*
	#include "exptree.h"
	#include "exptree.c"
	

%}


%token END PLUS MINUS DIV MUL NUM EQ READ WRITE BEG IF THEN ENDIF ELSE WHILE ENDWHILE LT LTE GT GTE NEQ ISEQ DO BREAK CONTINUE REPEAT UNTIL DECL ENDDECL INT STR
%token ID
%nonassoc LT LTE GT GTE NEQ ISEQ EQ
%left PLUS MINUS
%left MUL DIV

%%

/*Declarations: DECL DeclList ENDDECL		{}
	| DECL ENDDECL				{};

DeclList: DeclList Decl				{} 
	| Decl					{};

Decl: Type Varlist';'	 			{ assignType();};

	

Type: INT					{stype=SINT;}
| STR						{stype=SSTR;};
*/
Program: GDeclBlock MainBlock 
	//| GDeclBlock FdefBlock MainBlock
	| MainBlock;	

GDeclBlock: DECL GdecList ENDDECL		{}
	| DECL ENDDECL	';'		{printf("H1");};		

GdecList: GdecList GDecl			{}
	| GDecl					{};


GDecl: Type GidList ';'				{ assignType();};



GidList: GidList ',' ID	'[' NUM ']' '[' NUM ']'	{	
							if(Lookup($3->varname)==NULL)
							{
							Ginstall($3->varname,NULL,$5->val,$8->val,0,NULL);
							}
						else
						{
							printf("Multiple declarations");
							exit(0);	
						}
						}

	| GidList ',' ID '[' NUM ']'		{	
							if(Lookup($3->varname)==NULL)
							{
							Ginstall($3->varname,NULL,$5->val,NULL,0,NULL);
							
							}
						else
						{
							printf("Multiple declarations");
							exit(0);	
						}
						}
	| ID	'[' NUM ']' '[' NUM ']'		{	
							if(Lookup($1->varname)==NULL)
							{
							Ginstall($1->varname,NULL,$3->val,$6->val,0,NULL);
							
							}
						else
						{
							printf("Multiple declarations");
							exit(0);	
						}
						}
	| GidList ',' ID				{	
							if(Lookup($3->varname)==NULL)
							{
							Ginstall($3->varname,NULL,1,NULL,0,NULL);
							
							}
						else
						{
							printf("Multiple declarations");
							exit(0);	
						}
						}
	| GidList ',' MUL ID			{	if(Lookup($4->varname)==NULL)
							{
							Ginstall($4->varname,NULL,1,NULL,1,NULL);
							
							}
						else
						{	
							printf("Multiple declarations");	
						}
						}
	| ID '[' NUM ']'			{	if(Lookup($1->varname)==NULL)
							{
							Ginstall($1->varname,NULL, $3->val,NULL,0,NULL);
							
							}
						else
						{	
							printf("Multiple declarations");	
						}
						}

	| MUL ID				{	if(Lookup($2->varname)==NULL)
							{
							Ginstall($2->varname,NULL,1,NULL,1,NULL);
							
							}
						else
						{	
							printf("Multiple declarations");	
						}
						}
	
	
	| ID					{	if(Lookup($1->varname)==NULL)
							{
							Ginstall($1->varname,NULL,1,NULL,0,NULL);
							
							}
						else
						{	
							printf("Multiple declarations");	
						}
						}

							
	| ID '(' ParamList ')'			{	if(Lookup($1->varname)==NULL)
							{
							Ginstall($1->varname,NULL,1,NULL,0,$3);
							
							}
						else
						{	
							printf("Multiple declarations");	
						}
						} 
	;

ParamList: ParamList ',' Param 			{$1->right=$3;
						$$=$1;}
	| Param					{$$=$1;}
	| 					{$$=NULL;};

Param: PType ID 				{$$=createTree(NULL,tPARAM,$2->varname,NULL,NULL);
						$$->type=ptype;};								

PType :  INT					{ptype=inttype;}
	| STR					{ptype=strtype};


/*FDefBlock: FDefBlock Fdef
	| Fdef;


Fdef:  Type ID '(' ParamList ')' '{' LdeclBlock Body '}' ;






LdeclBlock: DECL LDecList ENDDECL
	| DECL ENDDECL;


LDecList: LDecList LDecl
	| LDecl;

LDecl:	Type IdList ;

IdList: IdList ',' ID				
	| ID ;

*/




MainBlock: BEG Declarations Slist END ';'	{
						Gsymbol *temp=shead;
						if(shead==NULL)
							printf("NO variables");
						else
						{
							while(temp!=NULL)
							{
								printf("%s %d %d %d\n",temp->name,temp->size,temp->size2,temp->binding);
								temp=temp->next;
							}
						}	
						fprintf(target_file,"%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\nMOV SP, 4196\n",0,2056,0,0,0,0,0,0);
						// CODEGEN (TO BE MODIFIED)
						// Print the output into file.
						//printf("TO CODEGEN");
						codeGen($3);
						//printf("FROM CODEGEN");
						fprintf(target_file,"MOV R0,\"Exit\" \nPUSH R0\nPUSH R0\nPUSH R0\nPUSH R0\nPUSH R0\nCALL 0\n");
						fprintf(target_file,"POP R0\nPOP R1\nPOP R1\nPOP R1\nPOP R1\n");
						exit(1);

						printf("\nDone\n");
						exit(1);
						}
	 | BEG Declarations END ';'		{
					 	printf("Empty\n");
					 	exit(1);
						};
Declarations: DECL DeclList ENDDECL		{}
	| DECL ENDDECL				{};

DeclList: DeclList Decl				{} 
	| Decl					{};

Decl: Type Varlist';'	 			{ assignType();};

	

Type: INT					{stype=SINT;}
| STR						{stype=SSTR;};

Varlist: Varlist ',' ID	'[' NUM ']' '[' NUM ']'	{	
							if(Lookup($3->varname)==NULL)
							{
							Install($3->varname,NULL,$5->val,$8->val,0);
							}
						else
						{
							printf("Multiple declarations");
							exit(0);	
						}
						}

	| Varlist ',' ID '[' NUM ']'		{	
							if(Lookup($3->varname)==NULL)
							{
							Install($3->varname,NULL,$5->val,NULL,0);
							
							}
						else
						{
							printf("Multiple declarations");
							exit(0);	
						}
						}
	| ID	'[' NUM ']' '[' NUM ']'		{	
							if(Lookup($1->varname)==NULL)
							{
							Install($1->varname,NULL,$3->val,$6->val,0);
							
							}
						else
						{
							printf("Multiple declarations");
							exit(0);	
						}
						}
	| Varlist ',' ID				{	
							if(Lookup($3->varname)==NULL)
							{
							Install($3->varname,NULL,1,NULL,0);
							
							}
						else
						{
							printf("Multiple declarations");
							exit(0);	
						}
						}
	| Varlist ',' MUL ID			{	if(Lookup($4->varname)==NULL)
							{
							Install($4->varname,NULL,1,NULL,1);
							
							}
						else
						{	
							printf("Multiple declarations");	
						}
						}
	| ID '[' NUM ']'			{	if(Lookup($1->varname)==NULL)
							{
							Install($1->varname,NULL, $3->val,NULL,0);
							
							}
						else
						{	
							printf("Multiple declarations");	
						}
						}

	| MUL ID				{	if(Lookup($2->varname)==NULL)
							{
							Install($2->varname,NULL,1,NULL,1);
							
							}
						else
						{	
							printf("Multiple declarations");	
						}
						}
	
	
	| ID					{	if(Lookup($1->varname)==NULL)
							{
							Install($1->varname,NULL,1,NULL,0);
							
							}
						else
						{	
							printf("Multiple declarations");	
						}
						};

								
							
Slist: Slist Stmt 				{
						$$ = createTree(NULL, tCONNECT, NULL, $1, $2);
						}
	| Stmt 					{$$=$1;};
Stmt: 	InputStmt 				{$$=$1;}
	| OutputStmt				{$$=$1;}
	| AsgStmt 				{$$=$1;}
	| Ifstmt				{$$=$1;}
	| Whilestmt				{$$=$1;}
	| Brkstmt				{$$=$1;}
	| Contstmt				{$$=$1;}
	| Repeatstmt				{$$=$1;}
	| Dostmt				{$$=$1;};

InputStmt: READ '(' Expr ')' ';' 		{if($3->nodetype!=tVAR && $3->nodetype!=tARR && $3->nodetype!=t2D && $3->nodetype!=tPTR)
						{
							printf("syntax error");
							exit(0);
						}
						$$= createReadNode($3);};
OutputStmt: WRITE '(' Expr ')' ';' 		{$$= createWriteNode($3);};
AsgStmt: Expr EQ Expr ';' 			{
						if($3->type!=$1->type || ($1->nodetype!=tARR && $1->nodetype!=t2D && $1->nodetype!=tPTR))
							{
								printf("Invalid assign operation");
								exit(0);
							}
						$$ = createAsgNode($1, $3);}
	| ID EQ Expr ';' 			{
						if($3->type!=$1->type)
							{
								printf("Invalid assign operation");
								exit(0);
							}
						$$ = createAsgNode($1, $3);};
Brkstmt: BREAK ';'				{$$= createBrkNode();};

Contstmt: CONTINUE ';'				{$$= createContNode();};


Expr :	 Expr PLUS Expr				{$$ = createOpNode('+',$1,$3);}

	 | Expr MINUS Expr  			{$$ = createOpNode('-',$1,$3);}

	 | Expr MUL Expr			{$$ = createOpNode('*',$1,$3);}

	 | Expr DIV Expr			{$$ = createOpNode('/',$1,$3);}

	 | '(' Expr ')'				{$$ = $2;}
	 | Expr LT Expr 			{$$= createLogNode(tLT,$1,$3);}
	 | Expr GT Expr				{$$= createLogNode(tGT,$1,$3);}
	 | Expr LTE Expr			{$$= createLogNode(tLTE,$1,$3);}	
	 | Expr GTE Expr			{$$= createLogNode(tGTE,$1,$3);}
	 | Expr NEQ Expr			{$$= createLogNode(tNEQ,$1,$3);}
	 | Expr ISEQ Expr			{$$= createLogNode(tISEQ,$1,$3);}
	 | NUM					{$$ = $1;}
	 | ID '[' Expr ']' '['Expr']'		{if ($3->type!=inttype || $5->type!=inttype)						
							{
							printf("syntax error");
							exit(0);
							}
						
						 $$= create2DNode($1,$3,$6);}
	 | ID '[' Expr ']'			{if ($3->type!=inttype)						
							{
							printf("syntax error");
							exit(0);
							}
						}
						{$$= createArrNode($1,$3);}
	 | ID					{$$ = $1;}
	 | STR					{$$ = $1;}
	 | MUL ID				{$$ = createPtrNode($2);}
	 | '&' ID				{$$=createAddrNode($2);}
	 | ID '(' ')'
	// | ID '(' ArgList ')'

	 ;
Ifstmt: IF '(' Expr ')' THEN Slist ELSE Slist ENDIF ';'	{struct tnode* x=createTree(NULL,tCONNECT,NULL,$6,$8);
							 $$=createTree(NULL,tIF,NULL,$3,x);}
	 | IF '(' Expr ')' THEN Slist ENDIF ';'		{$$=createTree(NULL,tIF,NULL,$3,$6);};
Whilestmt: WHILE '(' Expr ')' DO Slist ENDWHILE ';'	{$$=createTree(NULL,tWHILE,NULL,$3,$6);};
Repeatstmt: REPEAT  Slist  UNTIL '(' Expr ')' ';'	{$$=createTree(NULL,tREPEAT,NULL,$5,$2);};
Dostmt: DO  Slist  WHILE '(' Expr ')' ';'		{$$=createTree(NULL,tDO,NULL,$5,$2);};
%%



int main()
{	FILE *fp = fopen("IO.txt" , "r");
	extern FILE *yyin;
	if (fp)
		yyin=fp;
	
	for(int i=0;i<20;i++)
		reg[i]=0;
	for(int j=0;j<30;j++)
		vars[j]=0;
	
	target_file = fopen("exptree.xsm","w");
	yyparse();
	return 0;
}
