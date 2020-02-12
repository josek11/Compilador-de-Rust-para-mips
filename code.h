#ifndef __code_h__
#define __code_h__

struct _Atom {
	enum{ A_VAR, A_INT, } kind;
	union{
		char* var;
		int num;
	} type;
};

struct _Instr {
	enum {  I_SIMP,
			I_PLUS,
			I_MINUS,
			I_DIV, 
			I_MULT, 
			I_GOTO,
			I_LAB,
			I_EQUAL, 
			I_DIF , 
			I_LESS, 
			I_GREATER, 
			I_LE, 
			I_GE,
	} kind;
	union{
		struct{
			char *var; 
			struct _Atom* a;
		}simple;
		struct{
			char * var;
			struct _Atom* a;
			struct _Atom* b;
		} expr;
		struct{
			char * Glabel_id;
		} gotolabel;
		struct{
			char* var;
			struct _Atom* a;
			char* lif;
			char* lelse;
		} ifwhile;
		struct{
			char * Llabel_id;
		} label;
	} type;
};

struct _List{
	struct _Instr* current;
	struct _List* next;
	struct _List* first;
};

typedef struct _Atom Atom;
typedef struct _Instr Instr;
typedef struct _List List;

char* newVar();

Atom* varAtom(char *s);
Atom* intAtom(int v);


Instr* mkEmptyInstr(char* variab);
Instr* mkSimpInstr(char* var, Atom* a);
Instr* mkPlusInstr(char* var, Atom* a, Atom* b);
Instr* mkMinusInstr(char* var, Atom* a, Atom* b);
Instr* mkDivInstr(char* var, Atom* a, Atom* b);
Instr* mkMultInstr(char* var, Atom* a, Atom* b);
Instr* mkGotoInstr(char* label);
Instr* mkLabelInstr(char* label);
Instr* mkIfWhileInstr(char*  var, Atom* a, char* labelif, char* labelelse);
Instr* mkEqualInstr(char* var, Atom* a, char* labelif, char* labelelse);
Instr* mkDifInstr(char* var, Atom* a, char* labelif, char* labelelse);
Instr* mkLessInstr(char* var, Atom* a, char* labelif, char* labelelse);
Instr* mkGreaterInstr(char* var, Atom* a, char* labelif, char* labelelse);
Instr* mkLeInstr(char* var, Atom* a, char* labelif, char* labelelse);
Instr* mkGeInstr(char* var, Atom* a, char* labelif, char* labelelse);

Atom* mkintAtom(int val);
Atom* mkcharAtom(char* val);

List* append(List* l, List * m);
Instr* getNext(List* list);
Instr* getFirst(List* list);
Instr* getLast(List* list);
Instr* getPenultima(List* list);
void RemoveLast(List* l);
void ListAdd(List* l, Instr* instr);
List* mkNewList(Instr* instr);
void printAtom(Atom* atom);
void printAtom2(Instr* instr, char a);
void printIfWhile(Instr* instr, char* a);
void printInstr(Instr* instr);
void printList(List* l);
List* compileExp(Expr* e, char *r);



#endif