#include "decl.h"
#include "def.h"
#include "global.h"

// TODO: binary search

int findInTable(char * ident) {
    for (int i = 0; i < globSeek; i++) {
        if (!strcmp(ident, globalTable[i].ident)) {
            return i;
        }
    }
    return -1;
}

static int newInTable() {
    if (globSeek >= SYMTABLE_SIZE) {
        fprintf(stderr, "Symbol table is full\n");
        exit(1);
    }
    return globSeek;
}

int addInTable(char * ident) {
    int id = newInTable();
    globalTable[id].ident = (char *) malloc(sizeof(char) * strlen(ident));
    strcpy(globalTable[id].ident, ident);
    globSeek++;
    return id;
}