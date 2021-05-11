//GLOBAL VARIABLES
extern int16_t Putback;
extern Token Cur_Token;
extern ASTnode AST_Tree;
extern FILE * source_file;
extern FILE * output_file;
extern SymTable globalTable[SYMTABLE_SIZE];
extern uint8_t buf[KEYWORD_BUFF_LEN];

extern int32_t globSeek;

//for debug output
extern uint32_t Cur_Line;
extern uint32_t Cur_Symbol;