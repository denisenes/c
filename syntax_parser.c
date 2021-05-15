#include "decl.h"
#include "def.h"
#include "global.h"

// Allocate new AST node, write input values into it and return it
ASTnode *makeASTnode(int32_t op, ASTnode *left, ASTnode *right, int32_t value) {
    ASTnode *newNode = (ASTnode *) malloc(sizeof(ASTnode));
    if (newNode == NULL) {
        //Heap is so small...
        fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }

    newNode->op = op;
    newNode->left = left;
    newNode->right = right;
    newNode->v.intvalue = value;

    return newNode;
}

ASTnode *makeASTleaf(int32_t op, int32_t value) {
    return makeASTnode(op, NULL, NULL, value);
}

// operations precedence
static int OpPrec[] = {
        0, 10, 10,        // T_EOF, T_PLUS, T_MINUS
        20, 20,           // T_STAR, T_SLASH
        30, 30,           // T_EQ, T_NEQ
        40, 40, 40, 40    // T_LT, T_GT, T_LE, T_GE
};

// check if token has right precedence
static int getPrec(uint32_t token) {
    if (OpPrec[token] == 0) {
        fprintf(stderr, "Unexpected token: line %d symbol %d\n", Cur_Line, Cur_Symbol);
    }
    return OpPrec[token];
}

// make AST leaf from integer literal
static ASTnode *getPrimary(void) {
    ASTnode *node;
    int id;

    switch (Cur_Token.token_type) {
        case T_INTLIT:
            node = makeASTleaf(AST_INTLIT, Cur_Token.int_value);
            break;
        case T_IDENT:
            id = findInTable(buf);
            if (id == -1) {
                fprintf(stderr, "Unresolved symbol \"%s\": line %d symbol %d", (char *) &buf, Cur_Line, Cur_Symbol);
                exit(1);
            }
            node = makeASTleaf(AST_IDENT, id);
            break;
        default:
            fprintf(stderr, "Unexpected token (int literal expected): line %d symbol %d\n", Cur_Line, Cur_Symbol);
            exit(1);
    }
    getToken();
    return node;
}

static ASTnode *getBinaryOp(uint32_t tokentype, ASTnode *left, ASTnode *right) {
    switch (tokentype) {
        case T_PLUS:
            return makeASTnode(AST_ADD, left, right, 0);
        case T_MINUS:
            return makeASTnode(AST_SUB, left, right, 0);
        case T_STAR:
            return makeASTnode(AST_MUL, left, right, 0);
        case T_SLASH:
            return makeASTnode(AST_DIV, left, right, 0);
        case T_EQ:
            return makeASTnode(AST_EQ, left, right, 0);
        case T_NEQ:
            return makeASTnode(AST_NEQ, left, right, 0);
        case T_GT:
            return makeASTnode(AST_GT, left, right, 0);
        case T_GE:
            return makeASTnode(AST_GE, left, right, 0);
        case T_LT:
            return makeASTnode(AST_LT, left, right,0);
        case T_LE:
            return makeASTnode(AST_LE, left, right, 0);
        default:
            fprintf(stderr, "Unexpected token (binary op expected): line %d symbol %d\n", Cur_Line, Cur_Symbol);
            exit(1);
    }
}

// Build AST from token stream
ASTnode *prattParser(int ptp) {
    ASTnode *left, *right;
    uint32_t tokentype;

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

void testParser(ASTnode *cur_node, ASTnode *parent, int side) {

    if (cur_node == NULL) {
        return;
    }

    printf("Parent: %p ", parent);

    switch (side) {
        case TO_LEFT:
            printf("left node: ");
            break;
        case TO_RIGHT:
            printf("right node: ");
            break;
        default:
            fprintf(stderr, "Something went wrong\n");
    }

    switch (cur_node->op) {
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