#include "decl.h"
#include "def.h"
#include "global.h"

// Allocate new AST node, write input values into it and return it
static ASTnode * makeASTnode(int32_t op, ASTnode * left, ASTnode * right, int32_t value) {
    ASTnode * newNode = (ASTnode *) malloc(sizeof(ASTnode));
    if (newNode == NULL) {
        //Heap is so small...
        fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }

    newNode->op = op;
    newNode->left = left;
    newNode->right = right;
    newNode->intvalue = value;

    return newNode;
}

static ASTnode * makeASTleaf(int32_t op, int32_t value) {
    return makeASTnode(op, NULL, NULL, value);
}

// operations precedence
static int OpPrec[] = { 0, 10, 10, 20, 20, 0 };

// check if token has right precedence
static int getPrec(int token) {
    if (OpPrec[token] == 0) {
        fprintf(stderr, "Unexpected token: line %d symbol %d\n", Cur_Line, Cur_Symdol);
    }
    return OpPrec[token];
}

// make AST leaf from integer literal
static ASTnode * getPrimary(void) {
    ASTnode * node;

    switch (Cur_Token.token_type)
    {
    case T_INTLIT:
        node = makeASTleaf(AST_INTLIT, Cur_Token.int_value);
        getToken();
        break;
    default:
        fprintf(stderr, "Unexpected token (int literal expected): line %d symbol %d\n", Cur_Line, Cur_Symdol);
        exit(1);
        break;
    }

    return node;
}

static ASTnode * getBinaryOp(int tokentype, ASTnode * left, ASTnode * right) {
    switch (tokentype)
    {
    case T_PLUS:
        return makeASTnode(AST_ADD, left, right, 0);
        break;
    case T_MINUS:
        return makeASTnode(AST_SUB, left, right, 0);
        break;
    case T_STAR:
        return makeASTnode(AST_MUL, left, right, 0);
        break;
    case T_SLASH:
        return makeASTnode(AST_DIV, left, right, 0);
        break;
    default:
        fprintf(stderr, "Unexpected token (binary op expected): line %d symbol %d\n", Cur_Line, Cur_Symdol);
        exit(1);
        break;
    }
}

// Build AST from token stream
ASTnode * prattParser(int ptp) {
    ASTnode * left, * right;
    int tokentype;

    //printf("1) TOKEN: %d\n", Cur_Token.token_type);
    left = getPrimary();


    tokentype = Cur_Token.token_type;
    //printf("2) TOKEN: %d\n", tokentype);
    if (tokentype == T_SEMI) {
        return left;
    }

    while (getPrec(tokentype) > ptp) {
        
        getToken();
        //printf("3) TOKEN: %d\n", tokentype);
        right = prattParser(OpPrec[tokentype]);

        left = getBinaryOp(tokentype, left, right);

        tokentype = Cur_Token.token_type;
        if (tokentype == T_SEMI) {
            return left;
        }
    }

    return left;

}

//========TEMPORARY=========
enum {
    TO_LEFT,
    TO_RIGHT
};

void testParser(ASTnode * cur_node, ASTnode * parent, int side) {

    if (cur_node == NULL) {
        return;
    }

    printf("Parent: %p ", parent);

    switch (side)
    {
    case TO_LEFT:
        printf("left node: ");
        break;
    case TO_RIGHT:
        printf("right node: "); 
        break;
    }

    switch (cur_node->op)
    {
    case AST_ADD:
        printf("AST type: ADD\n");
        break;
    case AST_SUB:
        printf("AST type: SUB\n");
        break;
    case AST_MUL:
        printf("AST type: MUL\n");
        break;
    case AST_DIV:
        printf("AST type: DIV\n");
        break;
    case AST_INTLIT:
        printf("AST type: INTLIT\n");
        break;
    } 

    testParser(cur_node->left, cur_node, TO_LEFT);
    testParser(cur_node->left, cur_node, TO_RIGHT);
}