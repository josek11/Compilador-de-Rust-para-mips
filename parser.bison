// Tokens
%token 
  INT VAR
  PLUS MINUS MULT DIV MOD ATRB
  EQ DIF GT GET ST SET
  FUNC END LATRB
  CA CF PA PF
  IF ELSE WHILE
  PRINT READ STRING

// Operator associativity & precedence
%left GT GET LT LET EQ DIF
%left PLUS MINUS
%left MULT DIV MOD

// Root-level grammar symbol
%start program;

// Types/values in association to grammar symbols.
%union {
  int intValue;
  char* var;
  char* string;
  Expr* exprValue;
  BoolExpr* boolValue;
  Cmd* cmd;
  CmdList* cmdlist;
}

%type <intValue> INT
%type <var> VAR
%type <string> STRING
%type <exprValue> expr
%type <boolValue> bexpr
%type <cmd> cmd
%type <cmdlist> cmd_list

// Use "%code requires" to make declarations go
// into both parser.c and parser.h
%code requires {
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylex();
extern int yyline;
extern char* yytext;
extern FILE* yyin;
extern void yyerror(const char* msg);
CmdList* root;
}

%%
program: FUNC CA cmd_list CF { root = $3; }
 
cmd_list:
  {
    $$ = NULL;
  }
  |
  cmd cmd_list {
    $$ = ast_cmdlist($1,$2);
  }
  ;

expr:
  INT { 
    $$ = ast_integer($1); 
  }
  |
  VAR {
    $$ = ast_variable($1);
  }
  | 
  expr PLUS expr { 
    $$ = ast_operation(PLUS, $1, $3); 
  }
  | 
  expr MINUS expr { 
    $$ = ast_operation(MINUS, $1, $3); 
  }
  | 
  expr MULT expr { 
    $$ = ast_operation(MULT, $1, $3); 
  }
  | 
  expr DIV expr { 
    $$ = ast_operation(DIV, $1, $3); 
  }
  | 
  expr MOD expr { 
    $$ = ast_operation(MOD, $1, $3); 
  }
  |
  PA expr PF { 
    $$ = $2; 
  }
  ;

bexpr:
  expr EQ expr {
    $$ = ast_booloperation(EQ, $1, $3);
  }
  |
  expr DIF expr {
    $$ = ast_booloperation(DIF, $1, $3);
  }
  |
  expr GT expr {
    $$ = ast_booloperation(GT, $1, $3);
  }
  |
  expr GET expr {
    $$ = ast_booloperation(GET, $1, $3);
  }
  |
  expr LT expr {
    $$ = ast_booloperation(LT, $1, $3);
  }
  |
  expr LET expr {
    $$ = ast_booloperation(LET, $1, $3);
  }
  |
  PA bexpr PF {
    $$ = $2;
  }
  ;

cmd:
  LATRB VAR ATRB expr END {
    $$ = ast_atribution($2,$4);
  }
  |
  IF bexpr CA cmd_list CF {
    $$ = ast_if_statement($2,$4);
  }
  |
  IF bexpr CA cmd_list CF ELSE CA cmd_list CF {
    $$ = ast_ifelse_statement($2,$4,$8);
  }
  |
  WHILE bexpr CA cmd_list CF {
    $$ = ast_while_statement($2,$4);
  }
  |
  PRINT PA STRING PF END{
    $$ = ast_print_statement($3);
  }
  |
  READ PA VAR PF END {
  	$$ = ast_scan_statement($3);
  }
  ;

%%

void yyerror(const char* err) {
  printf("Line %d: %s - '%s'\n", yyline, err, yytext  );
}

