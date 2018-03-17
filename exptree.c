struct tnode* createTree(int val, int nodetype, char *c, struct tnode *l, struct tnode *r){
	struct tnode* t = (struct tnode*)malloc(sizeof(struct tnode));
	t->val = val;
	t->nodetype= nodetype;
	t->varname=(char *)malloc(sizeof(char*));
	t->varname=c;
	t->left = l;
	t->right = r;
	if(t->nodetype>=tLT && t->nodetype<=tNEQ)
		{if (t->right->type!=inttype || t->left->type!=inttype)
			{yyerror("TYPE ERROR IN BOOLCHECK");
			exit(0);
			}
		else
		t->type=booltype;
		}
	else if(t->nodetype==tIF || t->nodetype==tWHILE ||t->nodetype==tDO || t->nodetype==tREPEAT )
		{
			if(t->left->type==inttype)
			{
				yyerror("type mismatch");
				exit(0);
			}
		}
	else if(t->nodetype>=tADD && t->nodetype<=tDIV)
	{
		if(t->left->type!=inttype || t->right->type!=inttype)
		{
			yyerror("type mismatch");
			 exit(0);
		}
		t->type=inttype;
	}
	else if(t->nodetype==tNUM) 
		t->type=inttype;
	else if(t->nodetype==tARR || t->nodetype==t2D || t->nodetype==tVAR || t->nodetype==tPTR || t->nodetype==tADDR)
	{			
		t->Gentry=Lookup(t->varname);
		if(t->Gentry!=NULL && t->Gentry->type==SSTR)
			t->type=strtype;
		else if(t->Gentry!=NULL && t->Gentry->type==SINT)
			t->type=inttype;
	}
	
	if(t->nodetype==tSTR)
		t->type = strtype;
			
	return t;
}

struct tnode* createBrkNode()
{
	return createTree(NULL,tBRK,NULL,NULL,NULL);
}
struct tnode* createPtrNode(struct tnode *p)
{
	return createTree(NULL,tPTR,p->varname,NULL,NULL);
}
struct tnode* createAddrNode(struct tnode *p)
{
	return createTree(NULL,tADDR,p->varname,NULL,NULL);
}
struct tnode* createContNode()
{
	return createTree(NULL,tCONT,NULL,NULL,NULL);
}
struct tnode* createNumNode(int val)
{
	return createTree(val,tNUM, NULL, NULL, NULL);
}

struct tnode* createOpNode(char c, struct tnode *l, struct tnode *r)
{
	int tOP =0;
	switch (c){
	case '+': tOP=tADD;
		break;
	case '-': tOP=tSUB;
		break;
	case '*': tOP=tMUL;
		break;
	case '/': tOP=tDIV;
		break;
	}
	return createTree(NULL, tOP, NULL,l, r);
}

struct tnode* createLogNode(int tOP, struct tnode *l, struct tnode *r)
{
	return createTree(NULL,tOP,NULL,l,r);
}

struct tnode* createArrNode(struct tnode *l, struct tnode *r)
{
	return createTree(r->val,tARR,l->varname,l,r);
}

struct tnode* create2DNode(struct tnode *l, struct tnode *x,struct tnode *y)
{
	return createTree(NULL,t2D, l->varname, x, y);
}

struct tnode* createVarNode(char *c)
{
	return createTree(NULL,tVAR, c, NULL, NULL);
}

struct tnode* createStrNode(char *c)
{
	return createTree(NULL,tSTR, c, NULL, NULL);
}

struct tnode* createAsgNode(struct tnode *l, struct tnode *r)
{
	return createTree(NULL, tASSIGN, NULL, l, r);
}
struct tnode* createReadNode(struct tnode *r)
{
	return createTree(NULL, tREAD, NULL, NULL, r);
}
struct tnode* createWriteNode(struct tnode *r)
{
	return createTree(NULL, tWRITE, NULL, NULL, r);
}

int getLabel()
{
	label=label+1;
	return label;
}


int getReg()
{
	for(int i=0;i<20;i++)
	{
		if(reg[i]==0)
		{
			reg[i]=1;
			return i;
		}
	}
	yyerror("Out of registers\n");
	exit(1);
}

/*REGISTER DEALLOCATION*/
void freeReg()
{
	for(int i=19;i>=0;i--)
	{
		if(reg[i] == 1)
		{
			reg[i]=0;
			return;
		}
	}

	//yyerror("Out of registers\n");
	//exit(1);
}
void Readfn(char *ch,int n)
{
	Gsymbol *temp=Lookup(ch);
	if(temp==NULL)
	{
		printf("Undeclared variable");
		exit(0);
	}	
	if(n==-1) //VARIABLE
	{	int loc=temp->binding;
		int a=getReg();
		fprintf(target_file,"MOV R%d,\"Read\"\nPUSH R%d\nMOV R%d, -1\nPUSH R%d\nMOV R%d, %d\nPUSH R%d\nPUSH R%d\nPUSH R%d\nCALL 0",a,a,a,a,a,loc,a,a,a);
		fprintf(target_file,"\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\n",a,a,a,a,a);
		freeReg();
	}
	else if(n>=0) //ARRAY
	{
		int a=getReg();			
		fprintf(target_file,"MOV R%d,\"Read\"\nPUSH R%d\nMOV R%d, -1\nPUSH R%d\nPUSH R%d\nPUSH R%d\nPUSH R%d\nCALL 0",a,a,a,a,n,a,a);
		fprintf(target_file,"\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\n",a,a,a,a,a);
		freeReg();
		freeReg();
	}
}
int decode(struct tnode *t)
{
	if (t->nodetype==tNUM)
	{
		int p=getReg();
		fprintf(target_file,"MOV R%d,%d\n",p,t->val);
		return p;
	}
	else if (t->nodetype==tVAR)
		{
			if(t->Gentry==NULL)
			{
				yyerror("Undeclared variable");
				exit(0);
			}			
			int p=getReg();
			fprintf(target_file,"MOV R%d,[%d]\n",p,t->Gentry->binding);
			return p;
		}
	else if(t->nodetype==tADDR)
	{			
		if(t->Gentry==NULL)
		{
			yyerror("Usage of undeclared variable");
			exit(0);
		}
		int p=getReg();
		fprintf(target_file,"MOV R%d,%d\n",p,t->Gentry->binding);
		return p;
	}
	else if (t->nodetype==tARR)
	{	
		//printf("HIII1");
		if(t->Gentry==NULL)
		{
			yyerror("Usage of undeclared variable");
			exit(0);
		}	
		int p=codeGen(t->right);
		if(p>30)
		{
			int a=getReg();
			fprintf(target_file,"MOV R%d,[%d]\n",a,p);
			fprintf(target_file,"ADD R%d,%d\n",a,t->Gentry->binding);
			fprintf(target_file,"MOV R%d,[R%d]\n",a,a);		
			return a;
		}
		//printf("HIII");
		fprintf(target_file,"MOV R%d,[R%d]\n",p,p);
		fprintf(target_file,"ADD R%d,%d\n",p,t->Gentry->binding);
		fprintf(target_file,"MOV R%d,[R%d]\n",p,p);
	
		return p;				
	}
	else if (t->nodetype==t2D)
	{	
		
		if(t->Gentry==NULL)
		{
			yyerror("Usage of undeclared variable");
			exit(0);
		}	
		int p=codeGen(t->right);
		int a;		
		if(p>30)
		{
			a=getReg();
			fprintf(target_file,"MOV R%d,[%d]\n",a,p);
			fprintf(target_file,"ADD R%d,%d\n",a,t->Gentry->binding);
			p=a;
		//	return a;
		}
		else
		{
			fprintf(target_file,"MOV R%d,[R%d]\n",p,p);
			fprintf(target_file,"ADD R%d,%d\n",p,t->Gentry->binding);
		//	return p;
		}
		a=codeGen(t->left);
		if(a>30)
		{
			int b=getReg();
			fprintf(target_file,"MOV R%d,[%d]\n",b,a);
			fprintf(target_file,"MUL R%d,%d\n",b,t->Gentry->size);
			fprintf(target_file,"ADD R%d,R%d\n",p,b);
			freeReg();
		//	return a;
		}
		else
		{
			fprintf(target_file,"MOV R%d,[R%d]\n",a,a);
			fprintf(target_file,"MUL R%d,%d\n",a,t->Gentry->size);
			fprintf(target_file,"ADD R%d,R%d\n",p,a);
			freeReg();
		//	return p;
		}
		fprintf(target_file,"MOV R%d,[R%d]\n",p,p);
		
		return p;
				
	}
	else if (t->nodetype==tPTR)
	{
		if(t->Gentry==NULL)
		{
			yyerror("Usage of undeclared variables");
			exit(0);
		}
		if(t->Gentry->ptr==0)
		{
			yyerror("Incorrect usage of dereferencing");
			exit(0);
		}
		int p=getReg();
		fprintf(target_file,"MOV R%d,[%d]\n",p,t->Gentry->binding);
		fprintf(target_file,"MOV R%d, [R%d]\n",p,p);
		return p;
	}

	else if (t->nodetype==tSTR)
	{
		int p=getReg();
		fprintf(target_file,"MOV R%d,%s\n",p,t->varname);
		return p;
	}	

	else if	((t->nodetype>=tADD) && (t->nodetype<=tDIV))
	{
		int l = -1,r = -1;
		if(t->left!=NULL)
			l= decode(t->left);
		if(t->right!=NULL)
			r= decode(t->right);
		
		if(l>30)
		{
			int a=getReg();
			fprintf(target_file,"MOV R%d,[%d]\n",a,l);
			l=a;
		}
		if(r>30)
		{
			int a=getReg();
			fprintf(target_file,"MOV R%d,[%d]\n",a,r);
			r=a;
		}
		
		switch(t->nodetype)
		{
		
			case tADD:fprintf(target_file,"ADD R%d,R%d\n",l,r);
				break;
			case tSUB:fprintf(target_file,"SUB R%d,R%d\n",l,r);
				break;
			case tMUL:fprintf(target_file,"MUL R%d,R%d\n",l,r);
				break;
			case tDIV:fprintf(target_file,"DIV R%d,R%d\n",l,r);
				break;
		}		
		freeReg();
		return l;
	}
}

void Writefn(int n)
{	
	//int loc=4096+ (ch-'a');
	int a=getReg();
	fprintf(target_file,"MOV R%d,\"Write\"\nPUSH R%d\nMOV R%d, -2\nPUSH R%d\nPUSH R%d\nPUSH R%d\nPUSH R%d\nCALL 0",a,a,a,a,n,a,a);
	fprintf(target_file,"\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\n",a,a,a,a,a);
	freeReg();
	freeReg();
}


int boolCheck(struct tnode* t)
{
	int p;
	int q;
	if(t->left->nodetype==tVAR )
	{
		p=getReg();
		fprintf(target_file, "MOV R%d, [%d]\n",p,t->left->Gentry->binding);
	}
	else if(t->left->nodetype==tNUM)
	{
		p=getReg();
		fprintf(target_file, "MOV R%d, %d\n",p, t->left->val);
	}
	else if(t->left->nodetype==tARR || t->left->nodetype==t2D ||t->left->nodetype==tPTR)
	{	
		p=codeGen(t->left);
		fprintf(target_file, "MOV R%d, [R%d]\n",p, p);
	}
	if(t->right->nodetype==tVAR )
	{
		q=getReg();
		fprintf(target_file, "MOV R%d, [%d]\n",q,t->right->Gentry->binding);
	}
	else if(t->right->nodetype==tNUM)
	{
		q=getReg();
		fprintf(target_file, "MOV R%d, %d\n",q, t->right->val);
	}
	else if(t->right->nodetype==tARR || t->right->nodetype==t2D ||t->right->nodetype==tPTR)
	{	
		q=codeGen(t->right);
		fprintf(target_file, "MOV R%d, [R%d]\n",q, q);
	}
	
	switch(t->nodetype)
	{
		case tLT:	fprintf(target_file, "LT R%d, R%d\n",p,q);
				break;
		case tLTE:	fprintf(target_file, "LE R%d, R%d\n",p,q);
				break;
		case tGT:	fprintf(target_file, "GT R%d, R%d\n",p,q);
				break;
		case tGTE:	fprintf(target_file, "GE R%d, R%d\n",p,q);
				break;
		case tISEQ:	fprintf(target_file, "EQ R%d, R%d\n",p,q);
				break;
		case tNEQ:	fprintf(target_file, "NE R%d, R%d\n",p,q);
				break;	
	}
	freeReg();
	
	return p;
}

	 	
	



/*CODE GENERATION MODULE*/
int codeGen(struct tnode *t)
{
	int l1,l2,reg;
	struct Gsymbol *temp;
	if (t->nodetype==tNUM)
	{
		int p=getReg();
		fprintf(target_file,"MOV R%d,%d\n",p,t->val);
		return p;
	}
	else if (t->nodetype==tSTR)
	{
		int p=getReg();
		fprintf(target_file,"MOV R%d,%s\n",p,t->varname);
		return p;
	}
	else if (t->nodetype==tVAR )
	{	
		
		if(t->Gentry==NULL)
		{
			yyerror("Usage of undeclared variable");
			exit(0);
		}		
		return t->Gentry->binding;
	}
	else if(t->nodetype==tADDR)
	{			
		if(t->Gentry==NULL)
		{
			yyerror("Usage of undeclared variable");
			exit(0);
		}
		int p=getReg();
		fprintf(target_file,"MOV R%d,%d\n",p,t->Gentry->binding);
		return p;
	}
				
	else if (t->nodetype==tARR)
	{	
		
		if(t->Gentry==NULL)
		{
			yyerror("Usage of undeclared variable");
			exit(0);
		}	
		int p=codeGen(t->right);
		if(p>30)
		{
			int a=getReg();
			fprintf(target_file,"MOV R%d,[%d]\n",a,p);
			fprintf(target_file,"ADD R%d,%d\n",a,t->Gentry->binding);
			return a;
		}
		fprintf(target_file,"MOV R%d,[R%d]\n",p,p);
		fprintf(target_file,"ADD R%d,%d\n",p,t->Gentry->binding);
		return p;
				
	}
	else if (t->nodetype==tPTR)
	{
		if(t->Gentry==NULL)
		{
			yyerror("Usage of undeclared variables");
			exit(0);
		}
		if(t->Gentry->ptr==0)
		{
			yyerror("Incorrect usage of dereferencing");
			exit(0);
		}
		int p=getReg();
		fprintf(target_file,"MOV R%d,[%d]\n",p,t->Gentry->binding);
		
		return p;
	}
	else if (t->nodetype==t2D)
	{	
		
		if(t->Gentry==NULL)
		{
			yyerror("Usage of undeclared variable");
			exit(0);
		}	
		int p=codeGen(t->right);
		int a;		
		if(p>30)
		{
			a=getReg();
			fprintf(target_file,"MOV R%d,[%d]\n",a,p);
			fprintf(target_file,"ADD R%d,%d\n",a,t->Gentry->binding);
			p=a;
		//	return a;
		}
		else
		{
			fprintf(target_file,"MOV R%d,[R%d]\n",p,p);
			fprintf(target_file,"ADD R%d,%d\n",p,t->Gentry->binding);
		//	return p;
		}
		a=codeGen(t->left);
		if(a>30)
		{
			int b=getReg();
			fprintf(target_file,"MOV R%d,[%d]\n",b,a);
			fprintf(target_file,"MUL R%d,%d\n",b,t->Gentry->size);
			fprintf(target_file,"ADD R%d,R%d\n",p,b);
			freeReg();
		//	return a;
		}
		else
		{
			fprintf(target_file,"MOV R%d,[R%d]\n",a,a);
			fprintf(target_file,"MUL R%d,%d\n",a,t->Gentry->size);
			fprintf(target_file,"ADD R%d,R%d\n",p,a);
			freeReg();
		//	return p;
		}
		return p;
				
	}
	else if (t->nodetype==tCONNECT)
	{
		codeGen(t->left);
		codeGen(t->right);
	}
	else if (t->nodetype==tREAD)
	{
		if(t->right->Gentry==NULL)
		{
			yyerror("Usage of undeclared variable");
			exit(0);
		}
		if(t->right->nodetype==tVAR)
			Readfn(t->right->varname,-1);
		else
			{//printf("%dFIRST", t->right->val);
			//int a=getReg();
			//if (t->right->type==inttype){
				int a=codeGen(t->right);
				Readfn(t->right->varname,a);
				freeReg();
			//}
			
			
			}
	}
	else if (t->nodetype==tWRITE)
		{
			int p=decode(t->right);
			Writefn(p);
		}
	else if	(t->nodetype>=tADD && t->nodetype<=tDIV)
	{
		int l = -1,r = -1;
		if(t->left!=NULL)
			{
				l= codeGen(t->left);
				if(t->left->nodetype==tARR || t->left->nodetype==t2D || t->left->nodetype==tPTR)
					fprintf(target_file,"MOV R%d,[R%d]\n",l,l);
			}
		if(t->right!=NULL)
			{
				r= codeGen(t->right);
				if(t->right->nodetype==tARR || t->right->nodetype==t2D || t->right->nodetype==tPTR)
					fprintf(target_file,"MOV R%d,[R%d]\n",r,r);
			}
		
		if(l>30)
		{
						
			int a=getReg();
			fprintf(target_file,"MOV R%d,[%d]\n",a,l);
			l=a;
		}
		if(r>30)
		{
			int a=getReg();
			fprintf(target_file,"MOV R%d,[%d]\n",a,r);
			r=a;
		}
		
		switch(t->nodetype)
		{
		
			case tADD:fprintf(target_file,"ADD R%d,R%d\n",l,r);
				break;
			case tSUB:fprintf(target_file,"SUB R%d,R%d\n",l,r);
				break;
			case tMUL:fprintf(target_file,"MUL R%d,R%d\n",l,r);
				break;
			case tDIV:fprintf(target_file,"DIV R%d,R%d\n",l,r);
				break;
		}		
		freeReg();
		return l;
	}
	else if (t->nodetype==tASSIGN)
	{
		int flag=0,p;
		int a=codeGen(t->right);
		if(t->left->nodetype!=tVAR && t->left->nodetype!=tARR && t->left->nodetype!=t2D && (t->left->Gentry->ptr!=1 || t->right->nodetype!=tADDR) && t->left->nodetype!=tPTR)
		{
			printf("nodetype %d",t->left->nodetype);
			yyerror("syntax error");
			exit(0);
		}
		if(t->left->nodetype==tVAR && t->left->Gentry->ptr==0)
		{
			if(a>30)
			{
				flag=1;
				p=getReg();
				fprintf(target_file,"MOV R%d, [%d] \n",p,a);
				fprintf(target_file,"MOV [%d], R%d \n",t->left->Gentry->binding,p);
			}
		
				
			if(flag==0)	
				fprintf(target_file,"MOV [%d], R%d \n",t->left->Gentry->binding,a);
			
				if(flag==1)
				freeReg();
		}
		else if(t->left->nodetype==tVAR && t->left->Gentry->ptr==1)
		{
			p=codeGen(t->left);
			//fprintf(target_file,"MOV R%d, [R%d]\n",p,p);
			fprintf(target_file,"MOV [%d], R%d \n",p,a);
			freeReg();
		}
		else
		{
			int x=codeGen(t->left);
			if(a>30)
			{
				flag=1;
				//p=getReg();
				fprintf(target_file,"MOV [R%d], [%d] \n",x,a);
				//fprintf(target_file,"MOV [R%d], R%d \n",x,p);
			}
		
			 if(flag==0)	
				fprintf(target_file,"MOV [R%d], R%d \n",x,a);
								
			freeReg();
		}
	}	
	switch(t->nodetype)
	{
		case tWHILE:	l1=getLabel();
				l2=getLabel();
				fprintf (target_file,"L%d:\n", l1); // Place the first label here.
				reg=boolCheck(t->left);					//Generate code for the guard expression.
		   		fprintf(target_file, "JZ R%d, L%d\n",reg,l2);//code to compare the result tozeroand if sojumptoloopexit				
				freeReg();
				bsp++;
				brkstack[bsp]=l2;
				csp++;
				contstack[csp]=l1;
				codeGen(t->right);//Generate code for the body of the while loop.
				bsp--;		
				csp--;		
				fprintf(target_file, "JMP L%d\n", l1); // return to the beginning of the loop.
				fprintf(target_file, "L%d:\n", l2); // Place the second label here
				return ;

		case tDO:	l1=getLabel();
				l2=getLabel();
				fprintf (target_file,"L%d:\n", l1); // Place the first label here.
				bsp++;
				brkstack[bsp]=l2;
				csp++;
				contstack[csp]=l1;
				codeGen(t->right);//Generate code for the body of the while loop.
				bsp--;		
				csp--;	
				reg=boolCheck(t->left);					//Generate code for the guard expression.
		   		fprintf(target_file, "JZ R%d, L%d\n",reg,l2);//code to compare the result tozeroand if sojumptoloopexit				
				freeReg();
				fprintf(target_file, "JMP L%d\n", l1); // return to the beginning of the loop.
				fprintf(target_file, "L%d:\n", l2); // Place the second label here
				return ;

		case tREPEAT:	l1=getLabel();
				l2=getLabel();
				fprintf (target_file,"L%d:\n", l1); // Place the first label here.
				bsp++;
				brkstack[bsp]=l2;
				csp++;
				contstack[csp]=l1;
				codeGen(t->right);//Generate code for the body of the while loop.
				bsp--;		
				csp--;	
				reg=boolCheck(t->left);					//Generate code for the guard expression.
		   		fprintf(target_file, "JNZ R%d, L%d\n",reg,l2);//compare the result tooneand if sojumptoloopexit				
				freeReg();
				fprintf(target_file, "JMP L%d\n", l1); // return to the beginning of the loop.
				fprintf(target_file, "L%d:\n", l2); // Place the second label here
				return ;

		case tIF:	l1=getLabel();
				int flag=0;
				if(t->right->nodetype==tCONNECT)
					flag=1;
				reg=boolCheck(t->left);
				fprintf(target_file, "JZ R%d, L%d\n",reg,l1);
				freeReg();				
				if(flag==1)
				{	l2=getLabel();
					codeGen(t->right->left);
					fprintf(target_file, "JMP L%d\n", l2);
				}
					
				else 
				{	codeGen(t->right);
					fprintf (target_file,"L%d:\n", l1);
				}					
				if(flag==1)
				{
					fprintf (target_file,"L%d:\n", l1);
					codeGen(t->right->right);
					fprintf (target_file,"L%d:\n", l2);
				}	
				return;		
		
		case tBRK:	if(bsp!=-1)
					{fprintf(target_file, "JMP L%d\n", brkstack[bsp]);
					bsp--;}
				return;


		case tCONT:	if(csp!=-1)
					{fprintf(target_file, "JMP L%d\n", contstack[csp]);
					csp--;}
				return;		
		
				
	}
	
}




struct Gsymbol *Lookup(char * name)// Returns a pointer to the symbol table entry for the variable, returns NULL otherwise.
{
	if(shead==NULL)
		return NULL;
	struct Gsymbol *temp=shead;
	while(temp!=NULL)
	{
		if(strcmp(temp->name,name)==0)
			return temp;
		else
			temp=temp->next;
	}
	return NULL;
}

void Install(char *name, int type, int size,int size2, int ptr) // Creates a symbol table entry.
{
	if(Lookup(name)!=NULL)
	{
		yyerror("Multiple declaration of variable");
		exit(0);
	}
	int j,i;
	if(shead==NULL)
	{
		shead =(struct Gsymbol*) malloc(sizeof(struct Gsymbol));
		shead->name=(char *)malloc(sizeof(char*));
		shead->name=name;
		shead->type=type;
		shead->size=size;
		shead->size2=size2;
		shead->ptr=ptr;
		//int i;	
		if(size2!=NULL)
			size=size*size2;
				
		for(i=0;i<100;i++)
		{
			if(vars[i]==0)				
			{	vars[i]=1;
				shead->binding=4096+i;
				break;
			}
		}
		for(j=0,i=i+1;j<size-1 && i<100;i++, j++)
			vars[i]=1;
		if((j!=size-1)||(i>100))
		{
			printf("Insufficient space for variables");
				exit(0);
		}
		shead->next=NULL;		
		
	}
	else 
	{
		struct Gsymbol *temp=shead;
		while(temp->next!=NULL)
			temp=temp->next;
		temp->next= (struct Gsymbol*) malloc(sizeof(struct Gsymbol));
		temp=temp->next;
		temp->name=(char *)malloc(sizeof(char*));
		temp->name=name;
		temp->type=type;
		temp->size=size;
		temp->size2=size2;
		temp->ptr=ptr;
		//int j;
		if(size2!=NULL)
			size=size*size2;	
		for(j=0;j<100;j++)
		{
			if(vars[j]==0)	
			{	vars[j]=1;
				temp->binding=4096+j;
				break;
			}
		}
		for(i=0,j=j+1;i<size-1 && j<100;j++, i++)
		vars[j]=1;
		
		if((i!=size-1)|| (j>100))
		{
			printf("Insufficient space : %d : %d", size, i);
			exit(0);
		}
		temp->next=NULL;
	
	}
}

void assignType()
{	if(shead==NULL)
		yyerror("Error at declaring");
	struct Gsymbol *temp=shead;
	while(temp!=NULL)
	{
		if(temp->type==NULL)
			temp->type=stype;
		temp=temp->next;
	}	
}		



void yyerror(char *s)
{
	printf("yyerror %s",s);
}
	
	


void Ginstall(char *name, int type, int size,int size2, int ptr,struct tnode *paramlist) // Creates a symbol table entry.
{
	if(Lookup(name)!=NULL)
	{
		yyerror("Multiple declaration of variable");
		exit(0);
	}
	int j,i;
	if(shead==NULL)
	{
		shead =(struct Gsymbol*) malloc(sizeof(struct Gsymbol));
		shead->name=(char *)malloc(sizeof(char*));
		shead->name=name;
		shead->type=type;
		shead->size=size;
		shead->size2=size2;
		shead->ptr=ptr;
		//int i;	
		if(size2!=NULL)
			size=size*size2;
				
		for(i=0;i<100;i++)
		{
			if(vars[i]==0)				
			{	vars[i]=1;
				shead->binding=4096+i;
				break;
			}
		}
		for(j=0,i=i+1;j<size-1 && i<100;i++, j++)
			vars[i]=1;
		if((j!=size-1)||(i>100))
		{
			printf("Insufficient space for variables");
				exit(0);
		}
		shead->next=NULL;		
		shead->flabel=getLabel();
		if(paramlist!=NULL)
			shead->paramlist=(struct tnode *)malloc(sizeof(struct tnode*));
		shead->paramlist=paramlist;
	}
	else 
	{
		struct Gsymbol *temp=shead;
		while(temp->next!=NULL)
			temp=temp->next;
		temp->next= (struct Gsymbol*) malloc(sizeof(struct Gsymbol));
		temp=temp->next;
		temp->name=(char *)malloc(sizeof(char*));
		temp->name=name;
		temp->type=type;
		temp->size=size;
		temp->size2=size2;
		temp->ptr=ptr;
		//int j;
		if(size2!=NULL)
			size=size*size2;	
		for(j=0;j<100;j++)
		{
			if(vars[j]==0)	
			{	vars[j]=1;
				temp->binding=4096+j;
				break;
			}
		}
		for(i=0,j=j+1;i<size-1 && j<100;j++, i++)
		vars[j]=1;
		
		if((i!=size-1)|| (j>100))
		{
			printf("Insufficient space : %d : %d", size, i);
			exit(0);
		}
		temp->next=NULL;
		temp->flabel=getLabel();
		if(paramlist!=NULL)
			temp->paramlist=(struct tnode *)malloc(sizeof(struct tnode*));
		temp->paramlist=paramlist;	
	}
}
