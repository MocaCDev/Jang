#ifndef PARSER
#define PARSER
#include "tokens.h"
#include "lexer.h"

typedef struct parser {
    Tokens_* current_token_info;
    Tokens_* last_token_info;
    lexer_* lexer;
    char* active_file;

    struct {
        char* PKG_NAME;
        char* PKG_VERSION;
        size_t amount_of_imports;
        void** imports;
    } *PKG_INFO;
} parser_;

parser_* init_parser(lexer_* lexer, char* active_file);
parser_* parse(parser_* parser);

#endif