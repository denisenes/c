#include "decl.h"
#include "def.h"
#include "global.h"


static int AST_translate(ASTnode * node) {
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

void genPreambul() {
    asm64_pre();
}

void genPost() {
    asm64_post();
}

void genPrint(int res) {
    println(res);
}

void genFreeAllRegs() {
    asm64_freeall_reg();
}

static void match(int tokentype, char * expected) {
    if (tokentype == Cur_Token.token_type) {
        getToken();
    } else {
        fprintf(stderr, "%s expected on line %d\n", expected, Cur_Line);
        exit(1);
    }
}

void statements_translate() {
    ASTnode * ASTtree;
    int reg;

    while (1) {

        match(T_PRINT, "print");

        ASTtree = prattParser(0);
        reg = AST_translate(ASTtree);
        genPrint(reg);
        genFreeAllRegs();

        match(T_SEMI, ";");
        if (Cur_Token.token_type == T_EOF)
            return;
    }
}

void generateCode() {
    genPreambul();
    statements_translate();
    genPost();
}