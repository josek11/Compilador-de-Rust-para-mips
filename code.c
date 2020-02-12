#include <stdlib.h> // for malloc
#include <string.h> // for strings
#include <stdio.h> // for I/O
#include "parser.h"
#include "ast.h"
#include "code.h"

//------------MAKE ATOM-----------------------//

Atom*
varAtom(char *s) {
	Atom* a = (Atom*) malloc(sizeof(Atom));
	a->kind=A_VAR;
	a->type.var=strdup(s);
	return a;
}

Atom*
intAtom(int v) {
	Atom* a = (Atom*) malloc(sizeof(Atom));
	a->kind=A_INT;
	a->type.num=v;
	return a;
}

//------------------MAKE INSTR-----------------//

Instr* mkEmptyInstr(char* variab){
	Instr* instr = (Instr*) malloc(sizeof(Instr));
	instr->type.expr.var = strdup(variab);
	return instr;
}

Instr* mkSimpInstr(char* var, Atom* a){
	Instr* instr = mkEmptyInstr(var);
	instr->kind = I_SIMP;
	instr->type.simple.a=a;
	return instr;
}
Instr* mkPlusInstr(char* var, Atom* a, Atom* b){
	Instr* instr = mkEmptyInstr(var);
	instr->kind = I_PLUS;
	instr->type.expr.a=a;
	instr->type.expr.b=b;
	return instr;
}
Instr* mkMinusInstr(char* var, Atom* a, Atom* b){
	Instr* instr = mkEmptyInstr(var);
	instr->kind = I_MINUS;
	instr->type.expr.a=a;
	instr->type.expr.b=b;
	return instr;
}
Instr* mkDivInstr(char* var, Atom* a, Atom* b){
	Instr* instr = mkEmptyInstr(var);
	instr->kind = I_DIV;
	instr->type.expr.a=a;
	instr->type.expr.b=b;
	return instr;
}
Instr* mkMultInstr(char* var, Atom* a, Atom* b){
	Instr* instr = mkEmptyInstr(var);
	instr->kind = I_MULT;
	instr->type.expr.a=a;
	instr->type.expr.b=b;
	return instr;
}

// COMPLEX INSTR

Instr* mkGotoInstr(char* label){
	Instr* instr = (Instr*) malloc(sizeof(Instr));
	instr->kind = I_GOTO;
	instr->type.gotolabel.Glabel_id=strdup(label);
	return instr;
}

Instr* mkLabelInstr(char* label) {
	Instr* instr = (Instr*) malloc(sizeof(Instr));
	instr->kind = I_LAB;
	instr->type.label.Llabel_id=strdup(label);
	return instr;
}

// if while INSTR Maker

Instr* mkIfWhileInstr(char*  var, Atom* a, char* labelif, char* labelelse){
	Instr* instr = (Instr*) malloc(sizeof(Instr));
	instr->type.ifwhile.var = strdup(var);
	instr->type.ifwhile.a = a;
	instr->type.ifwhile.lif = strdup(labelif);
	instr->type.ifwhile.lelse = strdup(labelelse);
	return instr;
}

// Relop 

Instr* mkEqualInstr(char* var, Atom* a, char* labelif, char* labelelse){
	Instr * instr = mkIfWhileInstr(var,a,labelif,labelelse);
	instr->kind = I_EQUAL;
	return instr;
}
Instr* mkDifInstr(char* var, Atom* a, char* labelif, char* labelelse){
	Instr * instr = mkIfWhileInstr(var,a,labelif,labelelse);
	instr->kind = I_DIF;
	return instr;
}
Instr* mkLessInstr(char* var, Atom* a, char* labelif, char* labelelse){
	Instr * instr = mkIfWhileInstr(var,a,labelif,labelelse);
	instr->kind = I_LESS;
	return instr;
}
Instr* mkGreaterInstr(char* var, Atom* a, char* labelif, char* labelelse){
	Instr * instr = mkIfWhileInstr(var,a,labelif,labelelse);
	instr->kind = I_GREATER;
	return instr;
}
Instr* mkLeInstr(char* var, Atom* a, char* labelif, char* labelelse){
	Instr * instr = mkIfWhileInstr(var,a,labelif,labelelse);
	instr->kind = I_LE;
	return instr;
}
Instr* mkGeInstr(char* var, Atom* a, char* labelif, char* labelelse){
	Instr * instr = mkIfWhileInstr(var,a,labelif,labelelse);
	instr->kind = I_GE;
	return instr;
}


//---------------INTRUCTION LIST-------------//

List* mkNewList(Instr* instr){
	List* list = (List*) malloc(sizeof(List));
	list->current=instr;
	list->next=NULL;
	list->first=list;
	return list;
}

void ListAdd(List* l, Instr* instr){
	List *aux=l;
	if(aux->current==NULL) { // se nao tem elems
		aux->current=instr;
		aux->first=l->first;
	}
	else{
		
		if(aux->next==NULL){ //so tem um elem
			aux->next=mkNewList(instr);
			aux->first=l->first;
		}
	
		else{
			aux = l;
			while(aux->next!=NULL){
				aux=aux->next;
			}
			aux->next=mkNewList(instr);
			aux->first=l->first;
		}
	}
}

void RemoveLast(List* list) {
	if(list!=NULL) {
		
		if(list->next==NULL){ // se a lista tiver 1 elemento só
			list->current=NULL;
			free(list->current);
		}
		else {
			while(list!=NULL){
				if(list->next->next==NULL) {
					list->next=NULL;
					free(list->next);
				}
				list=list->next;
			}
		}
	}
}

Instr* getFirst(List* list){
	return list->first->current;
}
Instr* getNext(List* list){
	return list->next->current;
}

Instr* getLast(List* list) {
	if(list==NULL)
		return NULL;
	while(list!=NULL){
		if(list->next==NULL)
			return list->current;
		list=list->next;
	}
}

Instr* getPenultima(List* list) {
	if(list==NULL)
		return NULL;
	if(list->next==NULL)
		return NULL;
	while(list!=NULL) {
		if(list->next->next==NULL)
			return list->current;
		list=list->next;
	}
}

List* append(List* l, List * m){
	List* aux= l;
	while(aux->next!=NULL)
		aux=aux->next;
	aux->next=m;
	m->first=l;
	return aux;
}

//------------------PRINTS-----------------//

void printAtom(Atom* atom){
	switch (atom->kind){
		case A_VAR:
			printf("%s", atom->type.var);
			break;
		case A_INT:
			printf("%d", atom->type.num);
			break;
	}
}

void printAtom2(Instr* instr, char a){
	printf("%s ", instr->type.expr.var);
	printf(":= ");
	printAtom(instr->type.expr.a);
	printf(" %c ", a);
	printAtom(instr->type.expr.b);
	printf(";\n");
}

void printIfWhile(Instr* instr, char* a){
	printf("if ");
	printf("%s %s ", instr->type.ifwhile.var,a);
	printAtom(instr->type.ifwhile.a);
	printf(" then %s else %s\n",instr->type.ifwhile.lif,instr->type.ifwhile.lelse);
}

void printInstr(Instr* instr){
	switch(instr->kind){
		case I_SIMP:
			printf("%s ", instr->type.simple.var);
			printf(":= ");
			printAtom(instr->type.simple.a);
			printf(";\n");
			break;
		case I_PLUS:
			printAtom2(instr, '+');
			break;
		case I_MINUS:
			printAtom2(instr, '-');
			break;
		case I_MULT:
			printAtom2(instr, '*');
			break;
		case I_DIV:
			printAtom2(instr, '/');
			break;
		case I_GOTO:
			printf("goto %s\n",instr->type.gotolabel.Glabel_id);
			break;
		case I_LAB:
			printf("label %s\n",instr->type.label.Llabel_id);
			break;
		case I_EQUAL:
			printIfWhile(instr, "==");
			break;
		case I_DIF:
			printIfWhile(instr, "!=");
			break;
		case I_LESS:
			printIfWhile(instr, "<");
			break;
		case I_GREATER:
			printIfWhile(instr, ">");
			break;
		case I_LE:
			printIfWhile(instr, "<=");
			break;
		case I_GE:
			printIfWhile(instr, ">=");
			break;
	}
}

void printList(List* l){
	List* aux = l;
	if(l==NULL)
		printf("Lista Null\n");
	if(l->current==NULL)
		printf("Lista Vazia\n");
	else {
		while(aux->next!=NULL){
			printInstr(aux->current);
			aux = aux->next;
		}
		printInstr(aux->current);
	}
}

//---------------COMPILE EXPR-----------------------//


void compileExp2(Expr* e, char *r, List* list, List* final) {
	switch (e->kind) {
		case E_INTEGER:
			r=strdup(newVar());
			Atom* a1 = intAtom(e->attr.value);
			Instr* in = mkSimpInstr(r,a1);
			ListAdd(list,in);
			ListAdd(final,in);
			break;
		case E_VARIABLE:
			r=strdup(newVar());
			Atom* a2 = varAtom(e->attr.var);
			Instr* in2 = mkSimpInstr(r,a2);
			ListAdd(list,in2);
			ListAdd(final,in2);
			break;
		case E_OPERATION:
			compileExp2(e->attr.op.left,r,list,final);
			compileExp2(e->attr.op.right,r,list,final);
			r = newVar();
			//printf("r=%s",r);

			Instr* i1= getLast(list);
			Instr* i2= getPenultima(list);
			char *c1= strdup(i1->type.simple.var);
			//printf("->c1=%s\n", c1);
			char *c2= strdup(i2->type.simple.var);
			//printf("->c2=%s\n", c2);
			//printf("%s=%s %c %s\n", r,c1,e->attr.op.operator,c2);
			Atom* at1 = varAtom(strdup(c2));
			Atom* at2 = varAtom(strdup(c1));
			//evalOperator(e->attr.op.operator,r,at1,at2,list,final);
			if(e->attr.op.operator==PLUS) {
				//printf("PLUS +\n");
				Instr* in3 = mkPlusInstr(r,at1,at2);
				RemoveLast(list);
				RemoveLast(list);
				ListAdd(final,in3);
				ListAdd(list,in3);
				break;
			}
			else if(e->attr.op.operator==MINUS) {
				//printf("MINUS -\n");
				Instr* in4 = mkMinusInstr(r,at1,at2);
				RemoveLast(list);
				RemoveLast(list);
				ListAdd(final,in4);
				ListAdd(list,in4);
				break;
			}
			else if(e->attr.op.operator==MULT) {
				//printf("MULT *\n");
				Instr* in5 = mkMultInstr(r,at1,at2);
				RemoveLast(list);
				RemoveLast(list);
				ListAdd(final,in5);
				ListAdd(list,in5);
				break;
			}
			else if(e->attr.op.operator==DIV) {
				//printf("DIV /\n");
				Instr* in6 = mkDivInstr(r,at1,at2);
				RemoveLast(list);
				RemoveLast(list);			
				ListAdd(final,in6);
				ListAdd(list,in6);
			}
			else {
				printf("unknown OP\n");
			}

			break;

		default:
			printf("erro\n");
			break;
	}
}

//AbsTree For Expressions(Expr -> InstrList)
List* compileExp(Expr* e, char *r) {
	List* list = mkNewList(NULL);
	List* list2 = mkNewList(NULL);
	compileExp2(e,r,list,list2);
	return list2;
}