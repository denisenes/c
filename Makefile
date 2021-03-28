all: main.c lexer.c syntax_parser.c
	gcc -o fc main.c lexer.c syntax_parser.c