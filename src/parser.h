#ifndef PARSER
#define PARSER
#include "tokens.h"
#include "lexer.h"

typedef struct parser {
    Tokens_* current_token_info;
    Tokens_* last_token_info;
    lexer_* lexer;
} parser_;

parser_* init_parser(lexer_* lexer);

#endif