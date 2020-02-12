%{
// HEADERS
#include <stdlib.h>
#include "parser.h"

// variables maintained by the lexical analyser
int yyline = 1;
%}

%option noyywrap

%%
[ \t]+ {  }
#.*\n { yyline++; }
\n { yyline++; }

[0-9]+ { 
   yylval.intValue = atoi(yytext);
   return INT; 
}
"+" { return PLUS; }
"-" { return MINUS; }
"*" { return MULT; }
"/" { return DIV; }
"%" { return MOD; }
"==" { return EQ; }
"!=" { return DIF; }
">" { return GT; }
">=" { return GET; }
"<" { return LT; }
"<=" { return LET; }
";" { return END; }
"=" { return ATRB; }
"if" { return IF; }
"else" { return ELSE; }
"while" { return WHILE; }
"fn main()" { return FUNC; }
"let" { return LATRB; }
"{" { return CA; }
"}" { return CF; }
"(" { return PA; }
")" { return PF; }
"println!" { return PRINT; }
"io::stdin().read_line" { return READ; } 
[a-z][a-zA-Z0-9]* {
   yylval.var = strdup(yytext);
   return VAR;
}
\".*\" {
   yylval.string = strdup(yytext);
   return STRING; }
.  { yyerror("unexpected character"); }
%%

