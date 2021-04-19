#include "decl.h"
#include "def.h"

int AST_translate(ASTnode * node) {
    int left_reg, right_reg;

    if (node->left) left_reg = AST_translate(node->left);
    if (node->left) right_reg = AST_translate(node->right);

    switch (node->op)
    {
    case AST_ADD:
        return asm64_add(left_reg, right_reg);
    case AST_SUB:
        return asm64_sub(left_reg, right_reg);
    case AST_DIV:
        return asm64_div(left_reg, right_reg);
    case AST_MUL:
        return asm64_mul(left_reg, right_reg);
    case AST_INTLIT:
        return asm64_ld(node->intvalue);
    default:
        fprintf(stderr, "Unknown AST operator: %s\n", strerror(errno));
        exit(1);
    }
}

void generateCode(ASTnode * node) {
    asm64_pre();
    int res = AST_translate(node);
    println(res);
    asm64_post();
}