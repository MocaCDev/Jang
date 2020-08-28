#include <stdio.h>
#include <stdlib.h>
#include "src/lexer.h"
#include "src/file_reader.h"
#include "src/parser.h"

/*
    Jang: Json, just as a language!
    File extension: .j
*/

int main(int argc, char* args[]) {

    Tokens_* tokens = calloc(1,sizeof(*tokens));
    if(argc > 1) {
        lexer_* lexer = init_lexer(
            read_file(args[1]),
            tokens
        );
        parser_* parser = init_parser(lexer, args[1]);
        parse(parser);
    } else {
        fprintf(stderr,"\nError: The Jang compiler expects the following command:\n./main.o <filename>\n\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    return 0;
}