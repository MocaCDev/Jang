#ifndef PARSER
#define PARSER
#include "tokens.h"
#include "lexer.h"
#include "syn_tree.h"
#include <stdlib.h>

typedef struct parser {
    Tokens_* current_token_info;
    Tokens_* last_token_info;
    lexer_* lexer;
    char* active_file;

    struct {
        char* PKG_NAME;
        char* PKG_VERSION;
        size_t amount_of_imports;
        char* current_import_name;
        //char** imports; ~ This was not needed since we are storing a parser_ struct array for out syntax tree and runtime. All checkings will happen there
    } *PKG_INFO;
} parser_;

parser_* init_parser(lexer_* lexer, char* active_file);
SYN_TREE_* parse(parser_* parser);

#endif