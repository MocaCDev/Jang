.PHONY: Jang

Jang:
	gcc -Wall -o main.jang.o main.c src/syn_tree.c src/lexer.c src/parser.c src/file_reader.c src/runtime.c src/tokens.c