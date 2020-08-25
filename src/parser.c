#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"

parser_* init_parser(lexer_* lexer) {
    parser_* parser = calloc(1,sizeof(*parser));

    parser->lexer = lexer;
    parser->current_token_info = get_next_token(lexer);
    parser->last_token_info = parser->current_token_info;

    return parser;
}