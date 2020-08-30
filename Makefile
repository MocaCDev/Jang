.PHONY: Jang

Jang:
	gcc -Wall -o Jang.exe main.c src/lexer.c src/parser.c src/file_reader.c src/runtime.c src/tokens.c