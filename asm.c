#include "decl.h"
#include "def.h"
#include "global.h"

static char * regs[4] = {"%r8", "%r9", "%r10", "%r11"};
static int reg_pool[4];

static int asm64_allocate_reg() {
    for (int i = 0; i < 4; i++) {
        if (reg_pool[i] == 0) {
            reg_pool[i] = 1;
            return i;
        }
    }
}

static void asm64_free_reg(int i) {
    reg_pool[i] = 0;
}

void asm64_freeall_reg() {
    for (int i = 0; i < 4; i++) {
        reg_pool[i] = 0;
    }
}

int asm64_add(int reg1, int reg2) {
    fprintf(output_file, "\taddq\t%s, %s\n", regs[reg2], regs[reg1]);
    asm64_free_reg(reg2);
    return reg1;
}

int asm64_sub(int reg1, int reg2) {
    fprintf(output_file, "\tsubq\t%s, %s\n", regs[reg2], regs[reg1]);
    asm64_free_reg(reg2);
    return reg1;
}

int asm64_div(int reg1, int reg2) {
    fprintf(output_file, "\tmovq\t%s,%%rax\n", regs[reg1]);
    fprintf(output_file, "\tcqo\n");
    fprintf(output_file, "\tidivq\t%s\n", regs[reg2]);
    fprintf(output_file, "\tmovq\t%%rax,%s\n", regs[reg1]);
    asm64_free_reg(reg2);
    return reg1;
}

int asm64_mul(int reg1, int reg2) {
    fprintf(output_file, "\timulq\t%s, %s\n", regs[reg2], regs[reg1]);
    asm64_free_reg(reg2);
    return reg1;
}

int asm64_ld(int val) {
    int r = asm64_allocate_reg();
    fprintf(output_file, "\tmovq\t$%d, %s\n", val, regs[r]);
    return r;
}

void asm64_pre() {
    asm64_freeall_reg();
    fputs(
	"\t.text\n"
	".LC0:\n"
	"\t.string\t\"%d\\n\"\n"
	"printint:\n"
	"\tpushq\t%rbp\n"
	"\tmovq\t%rsp, %rbp\n"
	"\tsubq\t$16, %rsp\n"
	"\tmovl\t%edi, -4(%rbp)\n"
	"\tmovl\t-4(%rbp), %eax\n"
	"\tmovl\t%eax, %esi\n"
	"\tleaq	.LC0(%rip), %rdi\n"
	"\tmovl	$0, %eax\n"
	"\tcall	printf@PLT\n"
	"\tnop\n"
	"\tleave\n"
	"\tret\n"
	"\n"
	"\t.globl\tmain\n"
	"\t.type\tmain, @function\n"
	"main:\n"
	"\tpushq\t%rbp\n"
	"\tmovq	%rsp, %rbp\n",
    output_file);
}

void asm64_post() {
    asm64_freeall_reg();
    fputs(
	"\tmovl	$0, %eax\n"
	"\tpopq	%rbp\n"
	"\tret\n",
    output_file);
}

void println(int res) {
    fprintf(output_file, "\tmovq\t%s, %%rdi\n", regs[res]);
    fprintf(output_file, "\tcall\tprintint\n");
    asm64_free_reg(res);
}