#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"
#include "easy_access.h"
#include "file_reader.h"
#include "syn_tree.h"
#include "tokens.h"

parser_* init_parser(lexer_* lexer, char* active_file) {
    parser_* parser = calloc(1,sizeof(*parser));
    parser->PKG_INFO = calloc(1,sizeof(*parser->PKG_INFO));
    parser->active_file = active_file;

    parser->lexer = lexer;
    parser->current_token_info = get_next_token(lexer);
    parser->last_token_info = parser->current_token_info;
    parser->PKG_INFO->all_imports = calloc(1,sizeof(*parser->PKG_INFO->all_imports));
    //parser->PKG_INFO->amount_of_imports = 0;
    parser->PKG_INFO->current_import_name = calloc(1,sizeof(char));
    //parser->PKG_INFO->export_values = calloc(1,sizeof(char*));
    //parser->PKG_INFO->imports = calloc(1,sizeof(parser->PKG_INFO->imports));

    return parser;
}

static inline parser_* gather_next_token(parser_* parser, int current_token_id) {
    if(parser->current_token_info->token_id==current_token_id) {
        parser->last_token_info = parser->current_token_info;
        parser->current_token_info = get_next_token(parser->lexer);
    } else {
        raise_error("\nTokens do not match(%s)\n\n",parser->current_token_info->token_value);
    }

    return parser;
}

/*
    Simple way to check basic operations:
    ':' and '='
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

/*
    Imports in Jang is a very strict concept:
        When you import another .jang file, that jang file will be ran..fully..as if it is the main file
*/
static int import_amount;
char** arr;
void** parser_current_state;
SYN_TREE_* IMPORT(parser_* parser, SYN_TREE_* syntax_tree) {
    if(parser->current_token_info->token_id==IMPORTS_KEYWORD) {
        if(!(arr)) {
            arr = calloc(1,sizeof(char*));
        }
        if(!(parser_current_state)) {
            parser_current_state = calloc(1,sizeof(*parser_current_state));
        }
        parser->lexer->is_import = 0;
        /* TODO: Finsish This. This is a very sloppy version */
        gather_next_token(parser, IMPORTS_KEYWORD);
        Tokens_* tokens = calloc(1,sizeof(*tokens));
        lexer_* lexer = init_lexer(read_file(strcat(parser->current_token_info->token_value,".jang")),tokens);

        /* STORING IMPORTED FILE NAME */
        parser->PKG_INFO->current_import_name = parser->current_token_info->token_value;

        parser_* parser__ = init_parser(lexer, parser->current_token_info->token_value);
        parse(parser__);
        arr = realloc(
            arr,
            (import_amount+1)*sizeof(*arr)
        );
        arr[import_amount] = parser->PKG_INFO->current_import_name;

        parser_current_state = realloc(
            parser_current_state,
            (import_amount+1)*sizeof(struct parser*)
        );
        parser_current_state[import_amount] = parser;
        import_amount++;
    } else {
        raise_error("\nError: Expecting \"PKG_IMPORTS\" keyword, got token %d\n\n",parser->current_token_info->token_id);
        CLOSE(EXIT_FAILURE);
    }
    parser->PKG_INFO->amount_of_imports = import_amount;
    parser->PKG_INFO->all_imports = realloc(
        parser->PKG_INFO->all_imports,
        (import_amount+1)*sizeof(*parser->PKG_INFO->all_imports)
    );
    parser->PKG_INFO->all_imports = arr;

    /* Setting up syntax tree */
    syntax_tree->import_names = parser->PKG_INFO->all_imports;
    syntax_tree->amount_of_imports = parser->PKG_INFO->amount_of_imports;
    syntax_tree->parser_import_information = parser_current_state;
    return syntax_tree;
}
static inline void* parse_id(parser_* parser) {

    return parser;
}

SYN_TREE_* parse(parser_* parser) {
    switch(parser->current_token_info->token_id) {
        case IMPORTS_KEYWORD: return IMPORT(parser, init_syntax_tree(TREE_PKG));
        case TOKEN_ID: return parse_id(parser);
        default: break;
    }
    return init_syntax_tree(TREE_EOF);
}
