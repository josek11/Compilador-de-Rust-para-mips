// AST constructor functions

#include <stdlib.h> // for malloc
#include <string.h> // for strings
#include <stdio.h> // for I/O
#include "ast.h" // AST header

// INT
Expr* ast_integer
(int v) {
  //printf("integer\n");
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_INTEGER;
  node->attr.value = v;
  return node;
}

Expr* ast_operation
(int operator, Expr* left, Expr* right) {
  //printf("op\n");
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_OPERATION;
  node->attr.op.operator = operator;
  node->attr.op.left = left;
  node->attr.op.right = right;
  return node;
}

// VAR

Expr* ast_variable
(char* var) {
  //printf("var %s\n", var);
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_VARIABLE;
  node->attr.var = strdup(var);
  return node;
}

// BOOLEANS

BoolExpr* ast_booloperation
(int booloperator, Expr* left, Expr* right) {
  BoolExpr* node = (BoolExpr*) malloc(sizeof(BoolExpr));
  node->boolop=booloperator;
  node->left=left;
  node->right=right;
  return node;
}

// CMD

Cmd* ast_atribution
(char* var, Expr* right) {
  //printf("cmd atrib %s\n",var);
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = CMD_ATRIB;
  node->comando.cmdA.var=var;
  node->comando.cmdA.right=right;
  return node;
}

Cmd* ast_if_statement
(BoolExpr* bexpr, CmdList* if_cmd) {
  //printf("cmd if\n");
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = CMD_IF;
  node->comando.cmdI.condition=bexpr;
  node->comando.cmdI.cmd_if=if_cmd;
  return node;
}

Cmd* ast_ifelse_statement
(BoolExpr* bexpr, CmdList* if_cmd, CmdList* else_cmd) {
  //printf("cmd if else\n");
  Cmd* node = ast_if_statement(bexpr,if_cmd);
  node->comando.cmdI.cmd_else=else_cmd;
  return node;
}

Cmd* ast_while_statement
(BoolExpr* bexpr, CmdList* while_cmd) {
  //printf("cmd while\n");
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = CMD_WHILE;
  node->comando.cmdW.condition=bexpr;
  node->comando.cmdW.cmd_while=while_cmd;
  return node;
}

Cmd* ast_print_statement
(char* texto) {
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = CMD_PRINT;
  node->comando.cmdP.texto = strdup(texto);
  return node;
}

Cmd* ast_scan_statement(char* var){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = CMD_SCAN;
  node->comando.cmdS.var = strdup(var);
  return node;
}
// CMD LIST

CmdList* ast_cmdlist
(Cmd* cmd, CmdList* next) {
//printf("cmdlist");
	CmdList* node= (CmdList*) malloc(sizeof(CmdList));
	node->cmd = cmd;
	node->next = next;
	return node;
}

