#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"
#include "easy_access.h"
#include "file_reader.h"
#include "syn_tree.h"

static int amount;

parser_* init_parser(lexer_* lexer, char* active_file) {
    parser_* parser = calloc(1,sizeof(*parser));
    parser->PKG_INFO = calloc(1,sizeof(*parser->PKG_INFO));
    parser->active_file = active_file;

    parser->lexer = lexer;
    parser->current_token_info = get_next_token(lexer);
    parser->last_token_info = parser->current_token_info;
    //parser->PKG_INFO->amount_of_imports = 0;
    parser->PKG_INFO->current_import_name = calloc(1,sizeof(char));
    //parser->PKG_INFO->imports = calloc(1,sizeof(parser->PKG_INFO->imports));

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

/*
    Simple way to check basic operations:
    '(', ')', ':', ';', '{', '}'
*/
static inline void* check_operation(parser_* parser, char* PKG_, int throw_err) {

    if(parser->current_token_info->token_id==TOKEN_EQUALS) {
        gather_next_token(parser, TOKEN_EQUALS);
    }
    else if(parser->current_token_info->token_id == TOKEN_COLON) {
        gather_next_token(parser, TOKEN_COLON);
    }
    else {
        if(throw_err == 0) {
            raise_error("\nMissing key token ideal for %s\n\n",PKG_);
        }
    }
    return parser;
}

SYN_TREE_* PKG_SETUP(parser_* parser, SYN_TREE_* syntax_tree) {
    if(parser->current_token_info->token_id==IMPORTS_KEYWORD) {
        gather_next_token(parser, IMPORTS_KEYWORD);

        check_operation(parser, "IMPORT", 1);

        if(parser->current_token_info->token_id == TOKEN_LEFT_P) {
            gather_next_token(parser, TOKEN_LEFT_P);
            
            if(parser->current_token_info->token_id == TOKEN_RIGHT_P) {
                gather_next_token(parser, TOKEN_RIGHT_P);

                if(!(parser->current_token_info->token_id == TOKEN_SEMI)) {
                    raise_error("\nExpecting ';' at end of IMPORT expression(s)\n\n");
                } else gather_next_token(parser, TOKEN_SEMI);
            }
        } else {
            /* SINGILAR IMPORTS */
        }
    } else {
        raise_error("\nError: Expecting \"PKG_IMPORTS\" keyword, got token %d\n\n",parser->current_token_info->token_id);
        CLOSE(EXIT_FAILURE);
    }
}
static inline void* parse_id(parser_* parser) {

    return parser;
}

SYN_TREE_* parse(parser_* parser) {
    switch(parser->current_token_info->token_id) {
        case IMPORTS_KEYWORD: return PKG_SETUP(parser, init_syntax_tree(TREE_PKG));
        case TOKEN_ID: return parse_id(parser);
        default: break;
    }
    return init_syntax_tree(TREE_EOF);
}