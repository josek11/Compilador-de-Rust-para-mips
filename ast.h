
// AST definitions
#ifndef __ast_h__
#define __ast_h__


// AST for expressions
struct _Expr {
  enum { 
    E_VARIABLE,
    E_INTEGER,
    E_OPERATION,
  } kind;
  union {
    int value; // for integer values
	char* var; // for variables
    struct { 
      int operator; // PLUS, MINUS, etc
      struct _Expr* left;
      struct _Expr* right;
    } op; // for binary expressions
  } attr;
};

// AST for boolean expressions
struct _BoolExpr {
	int boolop;
	struct _Expr* left;
	struct _Expr* right;
};

// AST for Cmd expressions
struct _Cmd {
	enum {
		CMD_ATRIB,
		CMD_IF,
		CMD_WHILE,
		CMD_PRINT,
		CMD_SCAN,
	} kind;
	union {
		struct {
			char* var;
			struct _Expr* right;
		}cmdA;
		struct {
			struct _BoolExpr* condition;
			struct _CmdList* cmd_if;
			struct _CmdList* cmd_else;
		}cmdI;
		struct {
			struct _BoolExpr* condition;
			struct _CmdList* cmd_while;
		}cmdW;
		struct {
			char* texto;
		}cmdP;
		struct {
			char* var;
		}cmdS;
	}comando;
};

struct _CmdList {
  struct {
    struct _Cmd* cmd;
    struct _CmdList* next;
  };
};

typedef struct _CmdList CmdList;
typedef struct _Cmd Cmd;
typedef struct _BoolExpr BoolExpr; // Bollean expression typedef
typedef struct _Expr Expr; // Convenience typedef


// Constructor functions (see implementation in ast.c)
Expr* ast_integer(int v);
Expr* ast_variable(char* var);
Expr* ast_operation(int operator, Expr* left, Expr* right);
BoolExpr* ast_int(int v);
BoolExpr* ast_booloperation(int boolop, Expr* left, Expr* right);
Cmd* ast_atribution(char* var, Expr* right);
Cmd* ast_if_statement(BoolExpr* bexpr, CmdList* if_cmd);
Cmd* ast_ifelse_statement(BoolExpr* bexpr, CmdList* if_cmd, CmdList* else_cmd);
Cmd* ast_while_statement(BoolExpr* bexpr, CmdList* while_cmd);
Cmd* ast_print_statement(char* texto);
Cmd* ast_scan_statement(char* var);
CmdList* ast_cmdlist(Cmd* cmd, CmdList* next);

#endif
