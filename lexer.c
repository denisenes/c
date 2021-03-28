#include "decl.h"
#include "def.h"
#include "global.h"

#define ASCII_DIGITS_OFFSET 48

// Delimiters for lexer
uint8_t * delims = " \n";

// Get next symbol from input stream
static int16_t next() {
    int16_t c;

    if (Putback != -1) {
        c = Putback;
        //printf("--debug: sym %c\n", c);
        //printf("putback: %d\n", Putback);
        Putback = -1;
        return c;
    }

    c = fgetc(source_file);
    //printf("--debug: sym %d\n", c);

    if (c == '\n') {
        Cur_Line++;
        Cur_Symdol = 0;
    } else {
        Cur_Symdol++;
    }

    return c;
}

// Get next symdol that is not a delimeter
static int16_t next_ignore_delims() {
    int16_t c = next(source_file);

    while (strchr(delims, c) != NULL) {
        c = next(source_file);
    }

    return c;
}

static int get_int() {
    int intlit = 0;
    int16_t c = next(source_file);

    while (isdigit(c)) {
        intlit *= 10; //shift digits left
        intlit += c - ASCII_DIGITS_OFFSET;
        c = next(source_file);
    }

    Putback = c;
    return intlit;
}

void getToken() {
    int16_t c = next_ignore_delims(source_file);

    switch (c)
    {
    case '+':
        Cur_Token.token_type = T_PLUS;
        break;
    case '-':
        Cur_Token.token_type = T_MINUS;
        break;
    case '*':
        Cur_Token.token_type = T_STAR;
        break;
    case '/':
        Cur_Token.token_type = T_SLASH;
        break;
    case -1:
        Cur_Token.token_type = T_EOF;
        break;
    default:
        if (isdigit(c)) {
            Cur_Token.token_type = T_INTLIT;
            Putback = c;
            Cur_Token.int_value = get_int(source_file);
        } else {
            fprintf(stderr, "Syntax error: line %d symbol %d\n", Cur_Line, Cur_Symdol);
        }
        break;
    }
}