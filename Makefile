.PHONY: main.c

main.c:
	gcc -Wall -o main.o main.c src/lexer.c src/parser.c src/file_reader.c src/runtime.c src/tokens.c

run: main.c src/lexer.c src/parser.c src/file_reader.c src/runtime.c src/tokens.c
	./main.o