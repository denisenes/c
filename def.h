#ifndef HAHA_BENIS_DEF
#define HAHA_BENIS_DEF

// Token types
enum {
  T_EOF, T_PLUS, T_MINUS, T_STAR, T_SLASH, T_INTLIT
};

// Token structure
typedef struct Token_m {
    uint32_t token_type;
    uint32_t int_value; // if token is an integer literal
} Token;

// AST node types
enum {
  AST_ADD, AST_SUBTRACT, AST_MULTIPLY, AST_DIVIDE, AST_INTLIT
};

// Abstract Syntax Tree structure
typedef struct ASTnode_m {
  int op;				// "Operation" to be performed on this tree
  struct ASTnode_m * left;			// Left child
  struct ASTnode_m * right;    // Right child
  int intvalue;				// value of A_INTLIT
} ASTnode;

#endif