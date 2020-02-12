#include <stdbool.h>
#include "code.h"
FILE *fp;
int countVar2=0;

char* newVar2()
{
  int temp;
  char c = 't';
  if(countVar > 9)
    {
      temp = countVar - 9;
      c = 's';
    }
  else if(countVar > 16)
    {
      countVar = 0;
      temp = countVar;
    }
  else
      temp = countVar;

  char *var = malloc(sizeof(int) + 2);     
  sprintf(var, "$%c%d", c, temp);
  
  countVar++;  
  return var;
}

// AUXILIAR FUNCTIONS

bool StartsWith(const char *a, const char *b){
   if(strncmp(a, b, strlen(b)) == 0) return 1;
   return 0;
}

bool notRepeated(char* s, char *strings[],int count) { // checks if string s is on array strings
	int k=0;
	char* auxs;
	for(k=0;k<count;k++) {
		auxs = strings[k];
		if(strcmp(s,auxs)==0) {
			return false;
		}
	}
	return true;
}

void writeData(List* list) { // .data segment for global variables
	List * aux = list;
	int count=0;
	char *vars[10];
	while(aux!=NULL) {
		Instr* iaux = aux->current;
		if(iaux->kind==I_SIMP) {
			if(!StartsWith(iaux->type.simple.var,"$") && notRepeated(iaux->type.simple.var,vars,count)) {// not a register(variable)
				fprintf(fp, "\t%s:\t\t.space 4\n",iaux->type.simple.var);
				vars[count]=strdup(iaux->type.simple.var);
				count++;
			}
		}
		aux=aux->next;
	}
}

//MIPS PRINT

void evalAtom(Atom* a, char*var) {	
	if (a->kind==A_VAR)
		fprintf(fp, "\tlw\t%s,\t%s\n",var,a->type.var);
	else
		fprintf(fp, "\tli\t%s,\t%d\n",var,a->type.num);
}

void printMipsInstr(Instr* inst) {

	if(inst->kind==I_SIMP) {
		if(StartsWith(inst->type.simple.var,"$")) //li ou lw
			evalAtom(inst->type.simple.a,inst->type.simple.var);
		else  //sw
			fprintf(fp, "\tsw\t%s,\t%s\n",newVar2(),inst->type.simple.var);
	}

	else if(inst->kind==I_PLUS) {
		Atom*left = inst->type.expr.a;
		Atom*right = inst->type.expr.b;
		fprintf(fp, "\tadd\t%s,\t%s,\t%s\n",inst->type.expr.var,left->type.var,right->type.var);
	}

	else if(inst->kind==I_MINUS) {
		Atom*left = inst->type.expr.a;
		Atom*right = inst->type.expr.b;
		fprintf(fp, "\tsub\t%s,\t%s,\t%s\n",inst->type.expr.var,left->type.var,right->type.var);
	}

	else if(inst->kind==I_MULT) {
		Atom*left = inst->type.expr.a;
		Atom*right = inst->type.expr.b;
		fprintf(fp, "\tmul\t%s,\t%s,\t%s\n",inst->type.expr.var,left->type.var,right->type.var);
	}

	else if(inst->kind==I_DIV) {
		Atom*left = inst->type.expr.a;
		Atom*right = inst->type.expr.b;
		fprintf(fp, "\tdiv\t%s,\t%s,\t%s\n",inst->type.expr.var,left->type.var,right->type.var);
	}
	else if(inst->kind==I_GOTO) { // p completar
		fprintf(fp, "\tj\t%s\n",inst->type.gotolabel.Glabel_id);
	}
	else if(inst->kind==I_LAB) { // p completar
		fprintf(fp, "\n%s:\n",inst->type.label.Llabel_id);
	}
	else if(inst->kind==I_EQUAL) {
		fprintf(fp, "\tbeq\t%s,\t($zero),\t%s \n",inst->type.ifwhile.var,inst->type.ifwhile.lif);
	}
	else if(inst->kind==I_DIF) {
		fprintf(fp, "\tbne\t%s,\t($zero),\t%s \n",inst->type.ifwhile.var,inst->type.ifwhile.lif);
	}
	else if(inst->kind==I_LESS) {
		fprintf(fp, "\tblt\t%s,\t($zero),\t%s \n",inst->type.ifwhile.var,inst->type.ifwhile.lif);
	}
	else if(inst->kind==I_GREATER) {
		fprintf(fp, "\tbgt\t%s,\t($zero),\t%s \n",inst->type.ifwhile.var,inst->type.ifwhile.lif);
	}
	else if(inst->kind==I_LE) {
		fprintf(fp, "\tble\t%s,\t($zero),\t%s \n",inst->type.ifwhile.var,inst->type.ifwhile.lif);
	}
	else if(inst->kind==I_GE) {
		fprintf(fp, "\tbge\t%s,\t($zero),\t%s \n",inst->type.ifwhile.var,inst->type.ifwhile.lif);
	}
}

void printMIPS(List* list) {
	list = list->next;
	fp = fopen("mipscode.txt","w");
	fprintf(fp, ".data\n");
	writeData(list);
	fprintf(fp, ".text\n");
	while(list!=NULL) {
		printMipsInstr(list->current);
		list=list->next;
	}
	fprintf(fp, "\tli\t$v0,\t10\n\tsyscall\n");
	fclose(fp);
}