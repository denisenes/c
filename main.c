#include "decl.h"
#include "def.h"

//GLOBAL VARIABLES
int16_t Putback;
Token Cur_Token;
uint8_t buf[KEYWORD_BUFF_LEN];

ASTnode * AST_Tree;
FILE * source_file;
FILE * output_file;

int32_t globSeek;
SymTable globalTable[SYMTABLE_SIZE];


//for debug output
uint32_t Cur_Line;
uint32_t Cur_Symbol;


void usage() {
    printf("Usage: ./fc %%filename%%.c");
}

//init global variables
void init_global() {
    Cur_Line = 1;
    Cur_Symbol = 0;
    Putback = -1;
}

int main(int argc, char ** argv) {

    init_global();

    if (argc <= 1) {
        usage();
        exit(0);
    }
    
    printf("%s\n", argv[1]);
    source_file = fopen(argv[1], "r");
    if (source_file == NULL) {
        fprintf(stderr, "%s\n", strerror(errno));
    }

    getToken();
    output_file = fopen("out.s", "w");
    generateCode();
    
    
    fclose(source_file);
    fclose(output_file);
    return 0;
}