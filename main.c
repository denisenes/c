#include "decl.h"
#include "def.h"

//GLOBAL VARIABLES
int16_t Putback;
Token Cur_Token;
ASTnode * AST_Tree;
FILE * source_file;
FILE * output_file;

//for debug output
uint32_t Cur_Line;
uint32_t Cur_Symdol;


void usage() {
    printf("Usage: ./fc %%filename%%.c");
}

//init global variables
void init_global() {
    Cur_Line = 1;
    Cur_Symdol = 0;
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
    AST_Tree = prattParser(0);

    testParser(AST_Tree, NULL, 0);
    fclose(source_file);

    output_file = fopen("out.s", "w");
    generateCode(AST_Tree);
    


    fclose(output_file);
    return 0;
}