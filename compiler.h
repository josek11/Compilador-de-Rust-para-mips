int countLabel = 1;
int countVar = 1;


char* newVar()
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


char* getRegister(List *l) {
  Instr* i = getLast(l);
  //printInstr(i);
  if(i->kind==I_SIMP)
    return i->type.simple.var;
  else
    return i->type.expr.var;
}

char* newLabel() {

    int temp;
    char*c = "label";

    temp = countLabel;

    char *var = malloc(sizeof(c) + 1);     
    sprintf(var, "%s%d", c, temp);
  
    countLabel++;  
  return var;
}

// Compiles the VAR and Atom of the BoolExpr
List* compileCondition(BoolExpr* e) {
  Instr* label = mkLabelInstr(newLabel());
  List* condlist = mkNewList(label);
  char* x = e->left->attr.var;
  Atom* ax = varAtom(x);
  char* t1 = newVar();
  Instr* left  = mkSimpInstr(t1,ax);  //t1=x
  ListAdd(condlist,left);
  if(e->right->kind==E_INTEGER) { 
    int i = e->right->attr.value;
    Atom* ai = intAtom(i);
    char* t2 = newVar();
    Instr* in = mkSimpInstr(t2,ai);//t2=1
    ListAdd(condlist,in);
    Atom* a1 = varAtom(t1);
    Atom* a2 = varAtom(t2);
    Instr* right =  mkMinusInstr(newVar(),a1,a2);//t3=t1-t2
    ListAdd(condlist,right);
  }
  else if(e->right->kind==E_VARIABLE) { //x>y
    char* v = e->right->attr.var;
    Atom* av = varAtom(v);
    char* t3 = newVar();
    Instr* in = mkSimpInstr(t3,av);//t2=y
    ListAdd(condlist,in);
    Atom* a1 = varAtom(t1);
    Atom* a2 = varAtom(t3);
    Instr* right2 =  mkMinusInstr(newVar(),a1,a2);//t3=t1-t2
    ListAdd(condlist,right2);
  }
  return condlist;
}

// Compiles Instr-> IF VAR relop atom THEN label ELSE label
List* compileBool( BoolExpr* be, char *labelTrue, char *labelFalse) {
  List* InstrCond=compileCondition(be);
  
  Instr* l = getLast(InstrCond);
  char * var = l->type.expr.var;
  Atom* atom = intAtom(0);
  switch (be->boolop) {
    case EQ:
      l=mkEqualInstr(var,atom,labelTrue,labelFalse);
      break;
    case DIF: 
      l=mkDifInstr(var,atom,labelTrue,labelFalse);
      break;
    case GT:
      l=mkGreaterInstr(var,atom,labelTrue,labelFalse);
      break;
    case GET: 
      l=mkGeInstr(var,atom,labelTrue,labelFalse);
      break;
    case LT: 
      l=mkLessInstr(var,atom,labelTrue,labelFalse);
      break;
    case LET:
      l=mkLeInstr(var,atom,labelTrue,labelFalse);
      break;
    default: yyerror("Unknown operator!");
  }
  ListAdd(InstrCond,l);
  return InstrCond;
}

void compileCmd(Cmd* cmd,List* list) {
  if(cmd->kind==CMD_ATRIB) { 
    char* c_var = cmd->comando.cmdA.var;
    List* exprList = compileExp(cmd->comando.cmdA.right,"pr");
    char* s = getRegister(exprList);
    Atom* atA = varAtom(strdup(s));
    Instr* aux = mkSimpInstr(c_var,atA);
    ListAdd(exprList,aux);
    list = append(list,exprList);
  }
  else if(cmd->kind==CMD_WHILE) {
    char* labelt = newLabel();
    char* labelf = newLabel();
    List* boolist = compileBool(cmd->comando.cmdW.condition,labelt,labelf);
    Instr* lab1 = mkLabelInstr(labelt);
    ListAdd(boolist,lab1);
    CmdList* whileCmds = cmd->comando.cmdW.cmd_while;
    while(whileCmds!=NULL) {
      Cmd* auxcmd = whileCmds->cmd;
      compileCmd(auxcmd,boolist);
      whileCmds = whileCmds->next;
    }
    Instr* labelW = getFirst(boolist);
    char* nome_lab = labelW->type.label.Llabel_id;
    Instr* gotol = mkGotoInstr(nome_lab); //goto label while 
    ListAdd(boolist,gotol);
    Instr* lab2 = mkLabelInstr(labelf);
    ListAdd(boolist,lab2);
    list=append(list,boolist);
  }
  else if(cmd->kind==CMD_IF) {
    char* labelt2 = newLabel();
    char* labelf2 = newLabel();
    List* boolist2 = compileBool(cmd->comando.cmdI.condition,labelt2,labelf2);
    Instr* lab11 = mkLabelInstr(labelt2);
    ListAdd(boolist2,lab11);
    CmdList* ifCmds = cmd->comando.cmdI.cmd_if;
    while(ifCmds!=NULL) {
      Cmd* auxcmdi = ifCmds->cmd;
      compileCmd(auxcmdi,boolist2);
      ifCmds = ifCmds->next;
    }
    Instr* lab12 = mkLabelInstr(labelf2);
    ListAdd(boolist2,lab12);
    CmdList* elseCmds = cmd->comando.cmdI.cmd_else;
    while(elseCmds!=NULL) {
      Cmd* auxcmde = elseCmds->cmd;
      compileCmd(auxcmde,boolist2);
      elseCmds = elseCmds->next;
    }
    list=append(list,boolist2);
  }
  else if(cmd->kind==CMD_PRINT) {
    //printf("printz\n");
  }
  else if(cmd->kind==CMD_SCAN) {
    //printf("scanz\n");
  }
  else {
    //printf("Unknown kind\n");
  }
}

List* compileCmdList(CmdList* comandos) {
  Atom* at = varAtom(strdup("start"));
  Instr* inst = mkSimpInstr("first",at);
  List* list = mkNewList(inst);
  while(comandos!=NULL) {
    compileCmd(comandos->cmd,list);
    comandos=comandos->next;
  }
  return list;
}