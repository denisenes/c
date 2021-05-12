#ifndef HAHA_BENIS_DEF
#define HAHA_BENIS_DEF

#define _TOKEN_MAX_ID 5

#define ASCII_DIGITS_OFFSET 48
#define KEYWORD_BUFF_LEN 512

#define SYMTABLE_SIZE 2048

// Token types
enum {
  T_EOF,
  T_PLUS,
  T_MINUS,
  T_STAR,
  T_SLASH,
  T_INTLIT,
  T_SEMI,
  T_PRINT,
  T_EQ,
  T_INT,
  T_IDENT
};

// Token structure
typedef struct Token_m {
  uint32_t token_type;
  int32_t int_value; // if token is an integer literal
} Token;

// AST node types
enum {
  AST_ADD,
  AST_SUB,
  AST_MUL,
  AST_DIV,
  AST_INTLIT,
  AST_LVALUE,
  AST_IDENT,
  AST_ASSIGN
};

// Abstract Syntax Tree structure
typedef struct ASTnode_m {
  int op;				// "Operation" to be performed on this tree
  struct ASTnode_m * left;			// Left child
  struct ASTnode_m * right;    // Right child
  union {
      int intvalue;                // value of A_INTLIT
      int id;                      // id of variable in the symbol table
  } v;
} ASTnode;

typedef struct SymTable_m {
  char * ident; 
} SymTable;

#endif