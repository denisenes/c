#include "decl.h"
#include "def.h"

//GLOBAL VARIABLES
int16_t Putback;
Token Cur_Token;
FILE * source_file;

//for debug output
uint32_t Cur_Line;
uint32_t Cur_Symdol;


void usage() {
    printf("Usage: fc filename.c");
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



    /*//body
    getToken(source_fd);

    //for (int i = 0; i < 10; i++) {
    while (Cur_Token.token_type != T_EOF) {
        printf("Token type: %d\n", Cur_Token.token_type);
        if (Cur_Token.token_type == T_INTLIT) {
            printf("Token value: %d\n", Cur_Token.int_value);
        }
        printf("=======================\n");
        getToken(source_fd);
    }*/



    fclose(source_file);
    return 0;
}