#ifndef HAHA_BENIS_DECL
#define HAHA_BENIS_DECL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#include <errno.h>
#include "def.h"

// lexer.c
void getToken();

// syntax_parser.c
ASTnode * makeASTleaf(int32_t op, int32_t value);
ASTnode * makeASTnode(int32_t op, ASTnode * left, ASTnode * right, int32_t value);
ASTnode * prattParser(int ptp);
void testParser(ASTnode * cur_node, ASTnode * parent, int side);

// translator.c
void generateCode();

// asm.c
int asm64_add(int reg1, int reg2);
int asm64_sub(int reg1, int reg2);
int asm64_div(int reg1, int reg2);
int asm64_mul(int reg1, int reg2);
int asm64_ld_const(int val);
int asm64_ld(char * ident);
int asm64_st(int reg, char * ident);
void asm64_symbol(char * symbol);
void asm64_freeall_reg();
void asm64_pre();
void asm64_post();
void println(int res);

// sym.c
int findInTable(char * ident);
int addInTable(char * ident);

#endif