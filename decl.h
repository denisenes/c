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
void buildAST();
ASTnode * prattParser(int ptp);
void testParser(ASTnode * cur_node, ASTnode * parent, int side);

// translator.c
void generateCode();

// asm.c
void asm64_freeall_reg();
int asm64_add(int reg1, int reg2);
int asm64_sub(int reg1, int reg2);
int asm64_div(int reg1, int reg2);
int asm64_mul(int reg1, int reg2);
int asm64_ld(int val);
void asm64_pre();
void asm64_post();
void println(int res);

// sym.c
int findInTable(char * ident);
int addInTable(char * ident);

#endif