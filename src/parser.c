#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"
#include "easy_access.h"
#include "file_reader.h"

parser_* init_parser(lexer_* lexer) {
    parser_* parser = calloc(1,sizeof(*parser));
    parser->PKG_INFO = calloc(1,sizeof(parser->PKG_INFO));

    parser->lexer = lexer;
    parser->current_token_info = get_next_token(lexer);
    parser->last_token_info = parser->current_token_info;
    parser->PKG_INFO->amount_of_imports = 0;
    parser->PKG_INFO->imports = calloc(parser->PKG_INFO->amount_of_imports+1,sizeof(*parser->PKG_INFO->imports));

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

static inline void check_operation(parser_* parser, char* PKG_) {
    if(parser->current_token_info->token_id==TOKEN_EQUALS) gather_next_token(parser, TOKEN_EQUALS);
    else if(parser->current_token_info->token_id == TOKEN_COLON) gather_next_token(parser, TOKEN_COLON);
    else raise_error("\nMissing ':' or '=' for %s\n\n",PKG_);
}

static inline void* PKG_SETUP(parser_* parser) {
    if(parser->current_token_info->token_id==PKG_KEYWORD) {
        parser->lexer->pkg_found = 0;
        parser = gather_next_token(parser, PKG_KEYWORD);

        if(parser->current_token_info->token_id==TOKEN_LEFT_CURL) {
            gather_next_token(parser, TOKEN_LEFT_CURL);

            if(strcmp(parser->current_token_info->token_value,"NAME")==0) {
                gather_next_token(parser, TOKEN_ID);
                
                check_operation(parser,"PKG:NAME");
                
                if(parser->current_token_info->token_value && !(parser->current_token_info->token_value[0]=='"')) {
                    parser->PKG_INFO->PKG_NAME = parser->current_token_info->token_value;
                    gather_next_token(parser, TOKEN_ID);

                    if(parser->current_token_info->token_id==TOKEN_COMMA) gather_next_token(parser, TOKEN_COMMA);
                }// else raise_error("\nExpected PKG:NAME, got NULL on line %d(%d characters in)\n\n",parser->lexer->current_line,(parser->lexer->character_number-1));
            }
            if(strcmp(parser->current_token_info->token_value,"VERSION")==0) {
                gather_next_token(parser, TOKEN_ID);

                check_operation(parser,"PKG:VERSION");

                if(parser->current_token_info->token_value && !(parser->current_token_info->token_value[0]=='"')) {
                    parser->PKG_INFO->PKG_VERSION = parser->current_token_info->token_value;
                    gather_next_token(parser, TOKEN_ID);

                    if(parser->current_token_info->token_id == TOKEN_COMMA) gather_next_token(parser, TOKEN_COMMA);
                    else goto END;
                }

            }
            if(strcmp(parser->current_token_info->token_value,"IMPORT")==0) {
                gather_next_token(parser, TOKEN_ID);

                check_operation(parser,"PKG:IMPORT");

                if(parser->current_token_info->token_value  && !(parser->current_token_info->token_value[0] == '"')) {
                    parser->PKG_INFO->amount_of_imports++;
                    parser->PKG_INFO->imports = realloc(
                        parser->PKG_INFO->imports,
                        parser->PKG_INFO->amount_of_imports*sizeof(parser->PKG_INFO->imports)
                    );
                    strcat(parser->current_token_info->token_value,".jang");

                    parser->PKG_INFO->imports[parser->PKG_INFO->amount_of_imports-1] = (char*)parser->current_token_info->token_value;

                    /* READING THE FILE */
                    Tokens_* tokens = calloc(1,sizeof(*tokens));
                    lexer_* lexer = init_lexer(
                        read_file((char*)parser->PKG_INFO->imports[parser->PKG_INFO->amount_of_imports-1]),
                        tokens
                    );
                    parser_* parser_2 = init_parser(lexer);
                    parse(parser_2);
                }

                gather_next_token(parser, TOKEN_ID);
            }
            
            END:
            if(parser->current_token_info->token_id == TOKEN_RIGHT_CURL) gather_next_token(parser, TOKEN_RIGHT_CURL);
            else raise_error("\nExpecting closing '}' on line %d\n\n",parser->lexer->current_line);
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