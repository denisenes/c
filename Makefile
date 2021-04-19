all: main.c lexer.c syntax_parser.c translator.c asm.c
	gcc -o fc main.c lexer.c syntax_parser.c translator.c asm.c