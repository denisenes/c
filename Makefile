all: main.c lexer.c syntax_parser.c translator.c asm.c sym.c
	gcc -o fc main.c lexer.c syntax_parser.c translator.c asm.c sym.c