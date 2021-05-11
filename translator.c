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

void print_translate() {
    ASTnode * ASTtree;
    int reg;

    match(T_PRINT, "print");

    ASTtree = prattParser(0);
    reg = AST_translate(ASTtree);
    genPrint(reg);
    genFreeAllRegs();

    match(T_SEMI, ";");
    if (Cur_Token.token_type == T_EOF)
        return;
}

void assignment_translate() {
    ASTnode * right;
    ASTnode * left;
    
    match(T_IDENT, "identifier");

    // check this identifier in global symbol table
    if (findInTable(buf) == -1) {
        fprintf(stderr, "Unresolved symbol \"%s\": line %d symbol %d", (char *) &buf, Cur_Line, Cur_Symbol);
        exit(1);
    }

    

}

void declaration_translate() {
    match(T_INT, "int");
    match(T_IDENT, "identifier");
    addInTable(buf);
    //translate symbol
    match(T_SEMI, ";");
}

void statements() {
    while (1) {
        switch (Cur_Token.token_type)
        {
        case T_PRINT:
            print_translate();
            break;
        case T_IDENT:
            assignment_translate();
        case T_INT:
            declaration_translate();
        case T_EOF:
            return;
        default:
            break;
        }
    }
}

void generateCode() {
    genPreambul();
    statements();
    genPost();
}