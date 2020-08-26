#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"
#include "easy_access.h"

parser_* init_parser(lexer_* lexer) {
    parser_* parser = calloc(1,sizeof(*parser));

    parser->lexer = lexer;
    parser->current_token_info = get_next_token(lexer);
    parser->last_token_info = parser->current_token_info;

    return parser;
}

static inline parser_* gather_next_token(parser_* parser, int current_token_id) {
    if(parser->current_token_info->token_id==current_token_id) {
        parser->last_token_info = parser->current_token_info;
        parser->current_token_info = get_next_token(parser->lexer);
    } else {
        raise_error("\nTokens do not match\n\n");
    }

    return parser;
}

static inline void* PKG_SETUP(parser_* parser) {
    if(parser->current_token_info->token_id==PKG_KEYWORD) {
        parser->lexer->pkg_found = 0;
        parser = gather_next_token(parser, PKG_KEYWORD);

        if(parser->current_token_info->token_id==TOKEN_LEFT_CURL) {
            gather_next_token(parser, TOKEN_LEFT_CURL);
            gather_next_token(parser, TOKEN_ID);
        } else {
            raise_error("\nError: Expecting '{' on line %d, found %s\n\n",parser->lexer->current_line,parser->current_token_info->token_value);
            CLOSE(EXIT_FAILURE);
        }
    } else {
        raise_error("\nError: Expecting \"PKG\" keyword, got token %d\n\n",parser->current_token_info->token_id);
        CLOSE(EXIT_FAILURE);
    }
    return parser;
}
static inline void* parse_id(parser_* parser) {

    return parser;
}

parser_* parse(parser_* parser) {
    switch(parser->current_token_info->token_id) {
        case PKG_KEYWORD: return PKG_SETUP(parser);
        case TOKEN_ID: return parse_id(parser);
        default: break;
    }
    return parser;
}