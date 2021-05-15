#include "decl.h"
#include "def.h"
#include "global.h"

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

    c = (int16_t) fgetc(source_file);
    //printf("--debug: sym %d\n", c);

    if (c == '\n') {
        Cur_Line++;
        Cur_Symbol = 0;
    } else {
        Cur_Symbol++;
    }

    return c;
}

// Get next SCur_Symbol that is not a delimeter
static int16_t next_ignore_delims() {
    int16_t c = next();

    while (strchr(delims, c) != NULL) {
        c = next(source_file);
    }

    return c;
}

static int get_int() {
    int intlit = 0;
    int16_t c = next();

    while (isdigit(c)) {
        intlit *= 10; //shift digits left
        intlit += c - ASCII_DIGITS_OFFSET;
        c = next();
    }

    Putback = c;
    return intlit;
}

static int match_keyword() {
    switch (buf[0]) {
        case 'p':
            if (!strcmp(buf, "print\0"))
                return T_PRINT;
        case 'i':
            if (!strcmp(buf, "int\0"))
                return T_INT;
        default:
            //fprintf(stderr, "Syntax error: unknown identifier %s\n", buf); old, I'll delete this soon
            return T_IDENT;
    }
}

static int get_keyword() {
    int16_t c = next();
    int i = 0;

    while (isalpha(c) || c == '_' || isdigit(c)) {
        if (i >= KEYWORD_BUFF_LEN) {
            fprintf(stderr, "Identifier is too long on line %d\n", Cur_Line);
        }
        buf[i] = c;
        c = next();
        i++;
    }

    buf[i] = 0;
    Putback = c;
    return match_keyword();
}

void getToken() {
    int16_t c = next_ignore_delims();

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
    case ';':
        Cur_Token.token_type = T_SEMI;
        break;
    case '=':
        c = next();
        if (c == '=') {
            Cur_Token.token_type = T_EQ;
        } else {
            Putback = c;
            Cur_Token.token_type = T_ASSIGN;
        }
        break;
    case '!':
        c = next();
        if (c == '=') {
            Cur_Token.token_type = T_NEQ;
        } else {
            fprintf(stderr, "Unexpected symbol '%c': line %d symbol %d\n", c, Cur_Line, Cur_Symbol);
            exit(1);
        }
        break;
    case '>':
        c = next();
        if (c == '=') {
            Cur_Token.token_type = T_GE;
        } else {
            Cur_Token.token_type = T_GT;
        }
        break;
    case '<':
        c = next();
        if (c == '=') {
            Cur_Token.token_type = T_LE;
        } else {
            Cur_Token.token_type = T_LT;
        }
        break;
    case -1:
        Cur_Token.token_type = T_EOF;
        break;
    default:
        if (isdigit(c)) {
            Cur_Token.token_type = T_INTLIT;
            Putback = c;
            Cur_Token.int_value = get_int();
        } else if (isalpha(c) || c == '_') {
            Putback = c;
            Cur_Token.token_type = get_keyword();
        } else {
            fprintf(stderr, "Syntax error: line %d symbol %d\n", Cur_Line, Cur_Symbol);
        }
        break;
    }
}