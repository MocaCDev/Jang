#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"
#include "easy_access.h"

parser_* init_parser(lexer_* lexer) {
    parser_* parser = calloc(1,sizeof(*parser));
    parser->PKG_INFO = calloc(1,sizeof(parser->PKG_INFO));

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

            if(strcmp(parser->current_token_info->token_value,"NAME")==0) {
                gather_next_token(parser, TOKEN_ID);
                
                if(parser->current_token_info->token_id==TOKEN_EQUALS) gather_next_token(parser, TOKEN_EQUALS);
                else if(parser->current_token_info->token_id==TOKEN_COLON) gather_next_token(parser, TOKEN_COLON);
                else raise_error("\nMissing ':' or '=' for PKG:NAME\n\n");
                
                if(parser->current_token_info->token_value && !(parser->current_token_info->token_value[0]=='"')) {
                    parser->PKG_INFO->PKG_NAME = parser->current_token_info->token_value;
                    gather_next_token(parser, TOKEN_ID);

                    if(parser->current_token_info->token_id==TOKEN_COMMA) gather_next_token(parser, TOKEN_COMMA);
                } else raise_error("\nExpected PKG:NAME, got NULL on line %d(%d characters in)\n\n",parser->lexer->current_line,(parser->lexer->character_number-1));
            }
            if(strcmp(parser->current_token_info->token_value,"VERSION")==0) {
                gather_next_token(parser, TOKEN_ID);

                if(parser->current_token_info->token_id==TOKEN_EQUALS) gather_next_token(parser, TOKEN_EQUALS);
                else if(parser->current_token_info->token_id == TOKEN_COLON) gather_next_token(parser, TOKEN_COLON);
                else raise_error("\nMissing ':' or '=' for PKG:VERSION\n\n");

                if(parser->current_token_info->token_value && !(parser->current_token_info->token_value[0]=='"')) {
                    parser->PKG_INFO->PKG_VERSION = parser->current_token_info->token_value;
                    gather_next_token(parser, TOKEN_ID);

                    gather_next_token(parser, TOKEN_RIGHT_CURL);
                }
            }
            
            if(parser->current_token_info->token_id==TOKEN_SEMI) gather_next_token(parser, TOKEN_SEMI);
            else raise_error("\nExpecting ';' at end of PKG declaration\n\n");
        } else {
            raise_error("\nError: Expecting '{' on line %d, found %s\n\n",parser->lexer->current_line,parser->current_token_info->token_value);
            //CLOSE(EXIT_FAILURE);
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