#include <stdio.h>
#include <stdlib.h>
#include "src/lexer.h"
#include "src/file_reader.h"
#include "src/parser.h"
#include "src/syn_tree.h"
#include "src/runtime.h"

/*
    Jang:
    File extension: .j
*/

int main(int argc, char* args[]) {

    Tokens_* tokens = calloc(1,sizeof(*tokens));
    char* main_file_path = gather_main_jang_file();
    //if(argc > 1) {
    lexer_* lexer = init_lexer(
        read_file(main_file_path),
        tokens
    );
    parser_* parser = init_parser(lexer, main_file_path);
    SYN_TREE_* tree = parse(parser);

    if(!(tree->TREE_TOKEN_TYPE == TREE_EOF)) {
        check_tree_type(tree, parser, lexer);
    }
    //} else {
        //fprintf(stderr,"\nError: The Jang compiler expects the following command:\n./main.o <filename>\n\n");
        //fflush(stderr);
        //exit(EXIT_FAILURE);
    //}

    return 0;
}