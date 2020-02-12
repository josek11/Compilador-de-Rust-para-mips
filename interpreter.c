#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "code.h"
#include "compiler.h"
#include "printMips.h"

void printTabs(int tabs){
    for(int i=0;i<tabs;i++) {
	printf("    ");
    }
}


void eval(Expr* expr,int tabs) {

  printTabs(tabs);

  if (expr == 0) {
    yyerror("Null expression!!");
  }
  else if (expr->kind == E_INTEGER) {
    printf("%d\n",expr->attr.value);
  }
  else if (expr->kind == E_VARIABLE) {
    printf("%s\n",expr->attr.var);
  }
  else if (expr->kind == E_OPERATION) {
    //printf("OPERACAO ");
    switch (expr->attr.op.operator) {
      case PLUS:
        	printf("+\n");
		eval(expr->attr.op.left,tabs+1);
		eval(expr->attr.op.right,tabs+1);
        break;
      case MINUS: 
        	printf("-\n");
		eval(expr->attr.op.left,tabs+1);
		eval(expr->attr.op.right,tabs+1);
        break;
      case MULT: 
        	printf("*\n");
		eval(expr->attr.op.left,tabs+1);
		eval(expr->attr.op.right,tabs+1);
        break;
      case DIV: 
        	printf("/\n");
		eval(expr->attr.op.left,tabs+1);
		eval(expr->attr.op.right,tabs+1);
        break;
      case MOD: 
        	printf("%%\n");
		eval(expr->attr.op.left,tabs+1);
		eval(expr->attr.op.right,tabs+1);
        break;
      default: yyerror("Unknown operator!");
    }
  }
}

void eval2(BoolExpr* bexpr, int tabs) {

  printTabs(tabs);
  
  if (bexpr == 0) {
    yyerror("Null expression!!");
  }

  else {

    switch (bexpr->boolop) {
      case EQ:
		printf("==\n");
		eval(bexpr->left,tabs+1);
		eval(bexpr->right,tabs+1);
        	break;
      case DIF: 
		printf("!=\n");
		eval(bexpr->left,tabs+1);
		eval(bexpr->right,tabs+1);
       		break;
      case GT: 
        	printf(">\n");
		eval(bexpr->left,tabs+1);
		eval(bexpr->right,tabs+1);
        	break;
      case GET: 
        	printf(">=\n");
		eval(bexpr->left,tabs+1);
		eval(bexpr->right,tabs+1);
        	break;
      case LT: 
        	printf("<\n");
		eval(bexpr->left,tabs+1);
		eval(bexpr->right,tabs+1);
		break;
      case LET:
		printf("<=\n");
		eval(bexpr->left,tabs+1);
		eval(bexpr->right,tabs+1);
		break;
      default: yyerror("Unknown operator!");
    }
  }
}

void eval_cmd(Cmd* cmd, int tabs) {
	printTabs(tabs);
	if(cmd!=0) {
		if(cmd->kind==CMD_ATRIB) {
			printf("=\n");
			printTabs(tabs+1);
			printf("%s\n",cmd->comando.cmdA.var);
			eval(cmd->comando.cmdA.right,tabs+1);
		}
		else if(cmd->kind==CMD_IF) {
			printf("if\n");
			eval2(cmd->comando.cmdI.condition,tabs+1);
			CmdList* comandosIF = cmd->comando.cmdI.cmd_if;
			while(comandosIF!=NULL) {
				eval_cmd(comandosIF->cmd,tabs+1);
				comandosIF=comandosIF->next;
			}
			if(cmd->comando.cmdI.cmd_else!=NULL) {
				printTabs(tabs);
				printf("else\n");
				CmdList* comandosELSE = cmd->comando.cmdI.cmd_else;
				while(comandosELSE!=NULL) {
					eval_cmd(comandosELSE->cmd,tabs+1);
					comandosELSE=comandosELSE->next;
				}
			}
		}
		else if(cmd->kind==CMD_WHILE) {
			printf("while\n");
			eval2(cmd->comando.cmdW.condition,tabs+1);
			CmdList* comandosWHILE = cmd->comando.cmdW.cmd_while;
				while(comandosWHILE!=NULL) {
					eval_cmd(comandosWHILE->cmd,tabs+1);
					comandosWHILE=comandosWHILE->next;
				}
		}
		else if(cmd->kind==CMD_PRINT) {
			printf("println!\n");
			printTabs(tabs+1);
			printf("%s\n",cmd->comando.cmdP.texto);
		}
		else if(cmd->kind==CMD_SCAN) {
			printf("read_line\n");
			printTabs(tabs+1);
			printf("%s\n",cmd->comando.cmdS.var);
		}
	}
}

int evalprogram (CmdList* comandos, int tabs) {
  printf("fn main()\n");
  tabs++;
  while(comandos!=NULL) {
    eval_cmd(comandos->cmd,tabs);
    comandos=comandos->next;
  }
  return 0;
}

int main(int argc, char** argv) {
  --argc; ++argv;
  if (argc != 0) {
    yyin = fopen(*argv, "r");
    if (!yyin) {
      printf("'%s': could not open file\n", *argv);
      return 1;
    }
  } //  yyin = stdin
  if (yyparse() == 0) {
      /*printf("\n------------------ABSTree------------------\n");
      evalprogram(root,0);
      printf("\n-------------------///---------------------\n");
      */
      List* MidCode = compileCmdList(root);
      
      printf("--------------Intermediate Code--------------\n");
      printList(MidCode);
      printf("---------------------///---------------------\n");
      
      printMIPS(MidCode);
  }
  return 0;

}
