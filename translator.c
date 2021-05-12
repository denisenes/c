#include "decl.h"
#include "def.h"
#include "global.h"

int genAdd(int left, int right) {
    return asm64_add(left, right);
}

int genSub(int left, int right) {
    return asm64_sub(left, right);
}

int genMul(int left, int right) {
    return asm64_mul(left, right);
}

int genDiv(int left, int right) {
    return asm64_div(left, right);
}

int genLoadConst(int32_t value) {
    return asm64_ld_const(value);
}

int genLd(char * ident) {
    return asm64_ld(ident);
}

int genSt(int reg, char * ident) {
    return asm64_st(reg, ident);
}

void genGlobSymbol(char * symbol) {
    asm64_symbol(symbol);
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

static int AST_translate(ASTnode * node, int reg) {
    int left_reg, right_reg;

    if (node->left) left_reg = AST_translate(node->left, -1);
    if (node->left) right_reg = AST_translate(node->right, left_reg);

    switch (node->op)
    {
    case AST_ADD:
        return genAdd(left_reg, right_reg);
    case AST_SUB:
        return genSub(left_reg, right_reg);
    case AST_DIV:
        return genDiv(left_reg, right_reg);
    case AST_MUL:
        return genMul(left_reg, right_reg);
    case AST_INTLIT:
        return genLoadConst(node->v.intvalue);
    case AST_IDENT:
        return genLd(globalTable[node->v.id].ident);
    case AST_LVALUE:
        return genSt(reg, globalTable[node->v.id].ident);
    case AST_ASSIGN:
        return right_reg;
    default:
        fprintf(stderr, "Unknown AST operator: %s\n", strerror(errno));
        exit(1);
    }
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
    reg = AST_translate(ASTtree, -1);
    genPrint(reg);
    genFreeAllRegs();

    match(T_SEMI, ";");
    if (Cur_Token.token_type == T_EOF)
        return;
}

void assignment_translate() {
    ASTnode * right;
    ASTnode * left;
    ASTnode * localTree;
    int id;
    
    match(T_IDENT, "identifier");

    // check this identifier in global symbol table
    id = findInTable(buf);
    if (id == -1) {
        fprintf(stderr, "Unresolved symbol \"%s\": line %d symbol %d", (char *) &buf, Cur_Line, Cur_Symbol);
        exit(1);
    }

    // save id of variable in the right leaf
    right = makeASTleaf(AST_LVALUE, id);

    match(T_EQ, "=");

    // parse expression on the right side and save it in the left leaf
    left = prattParser(-1);

    localTree = makeASTnode(AST_ASSIGN, left, right, 0);

    // then turn it into asm code...
    AST_translate(localTree, -1);
    genFreeAllRegs();

    match(T_SEMI, ";");
}

void declaration_translate() {
    match(T_INT, "int");
    match(T_IDENT, "identifier");
    addInTable(buf);
    genGlobSymbol(buf);
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
            break;
        case T_INT:
            declaration_translate();
            break;
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