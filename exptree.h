#define tNUM 0
#define tADD 6
#define tSUB 7
#define tMUL 8
#define tDIV 9
#define tREAD 1
#define tWRITE 2
#define tASSIGN 3
#define tVAR 4
#define tCONNECT 5
#define strtype 2
#define inttype 1
#define booltype 0
#define tLT 10
#define tLTE 11
#define tGT 12
#define tGTE 13
#define tISEQ 14
#define tNEQ 15
#define tIF 16
#define tWHILE 17
#define tELSE 18
#define tBRK 19
#define tCONT 20
#define tREPEAT 21
#define tDO 22
#define tSTR 23
#define tARR 24
#define t2D 25
#define tPTR 26
#define tADDR 27
#define tPARAM 28
#define SINT 1
#define SSTR 2


typedef struct tnode {
	int val;	// value of a number for NUM nodes.
	int type;	//type of variable
	char *varname;	//name of a variable for ID nodes
	int nodetype;  // information about non-leaf nodes - read/write/connector/+/* etc.
	struct tnode *left,*right;	//left and right branches
	struct Gsymbol *Gentry;	
	//struct Gsymbol *Gentry;  //pointer to GST entry for global variables and functions	
	//int nodetype;		//node  type - asg/opertor/if/while etc.  
}tnode;
/*struct Gsymbol {
  char* name;  // name of the variable
  int type;   // type of the variable
  int size;   // size of the type of the variable
  int binding; // stores the static memory address allocated to the variable
  struct Gsymbol *next;
} shead;
*/
typedef struct Gsymbol {
	char* name;
	int type;
	int size;
	int size2;
	int ptr;
	int binding;
	struct ParamStruct *paramlist;
	int flabel;
	struct Gsymbol *next;
}Gsymbol;	
static Gsymbol *shead=NULL;

struct Gsymbol *Lookup(char *name); // Returns a pointer to the symbol table entry for the variable, returns NULL otherwise.
void Install(char *name, int type, int size,int size2, int ptr); // Creates a symbol table entry. 
void Ginstall(char *name, int type, int size,int size2, int ptr,struct tnode *tnode); // Creates a symbol table entry. 
void assignType();

int yylex(void);
int codeGen(struct tnode *t);
int evaluate(struct tnode *t);
int getReg();
void freeReg();
void yyerror(char *s);
FILE *target_file;
int reg[20];
int vars[100];
int brkstack[20],contstack[20];
static int bsp=-1,csp=-1,label=-1, stype=0,ptype=0;

struct tnode* createTree(int val, int nodetype, char *c, struct tnode *l, struct tnode *r);
struct tnode* createNumNode(int val);
struct tnode* createOpNode(char c, struct tnode *l, struct tnode *r);
struct tnode* createVarNode(char *c);
struct tnode* createAsgNode(struct tnode *l, struct tnode *r);
struct tnode* createReadNode(struct tnode *r);
struct tnode* createWriteNode(struct tnode *r);
struct tnode* createLogNode(int tOP, struct tnode *l, struct tnode *r);
struct tnode* createBrkNode();
struct tnode* createArrNode(struct tnode *l, struct tnode *r);
struct tnode* create2DNode(struct tnode *l, struct tnode *x,struct tnode *y);
struct tnode* createPtrNode(struct tnode *p);
struct tnode* createAddrNode(struct tnode *p);
int getLabel();
int boolCheck(struct tnode* t);

