#include "decl.h"
#include "def.h"
#include "global.h"

// Allocate new AST node, write input values into it and return it
static ASTnode * makeASTnode(int32_t op, ASTnode * left, ASTnode * right, int32_t value) {
    ASTnode * newNode = (ASTnode *) malloc(sizeof(ASTnode));
    if (newNode == NULL) {
        //Heap is so small...
        fprintf(stderr, "%s\n", strerror(errno));
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

// Build AST from token stream
ASTnode * prattParser() {

}