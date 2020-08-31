#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"
#include "easy_access.h"
#include "file_reader.h"
#include "syn_tree.h"

parser_* init_parser(lexer_* lexer, char* active_file) {
    parser_* parser = calloc(1,sizeof(*parser));
    parser->PKG_INFO = calloc(1,sizeof(*parser->PKG_INFO));
    parser->active_file = active_file;

    parser->lexer = lexer;
    parser->current_token_info = get_next_token(lexer);
    parser->last_token_info = parser->current_token_info;
    parser->PKG_INFO->amount_of_imports = 0;
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

static inline void check_operation(parser_* parser, char* PKG_) {
    if(parser->current_token_info->token_id==TOKEN_EQUALS) gather_next_token(parser, TOKEN_EQUALS);
    else if(parser->current_token_info->token_id == TOKEN_COLON) gather_next_token(parser, TOKEN_COLON);
    else raise_error("\nMissing ':' or '=' for %s\n\n",PKG_);
}

SYN_TREE_* PKG_SETUP(parser_* parser) {
    if(parser->current_token_info->token_id==PKG_KEYWORD) {
        parser->lexer->pkg_found = 0;
        SYN_TREE_* syntax_tree = init_syntax_tree(TREE_PKG);
        parser = gather_next_token(parser, PKG_KEYWORD);

        if(parser->current_token_info->token_id==TOKEN_LEFT_CURL) {
            parser->lexer->is_special = 0;
            static int amount;
            gather_next_token(parser, TOKEN_LEFT_CURL);

            BEGINNING:
            if(strcmp(parser->current_token_info->token_value,"NAME")==0) {
                gather_next_token(parser, TOKEN_ID);
                
                check_operation(parser,"PKG:NAME");
                
                if(parser->current_token_info->token_value && !(parser->current_token_info->token_value[0]=='"')) {
                    parser->PKG_INFO->PKG_NAME = parser->current_token_info->token_value;

                    gather_next_token(parser, TOKEN_ID);

                    if(parser->current_token_info->token_id==TOKEN_COMMA) gather_next_token(parser, TOKEN_COMMA);
                    else goto END;
                    goto BEGINNING;
                }// else raise_error("\nExpected PKG:NAME, got NULL on line %d(%d characters in)\n\n",parser->lexer->current_line,(parser->lexer->character_number-1));
            }
            else if(strcmp(parser->current_token_info->token_value,"VERSION")==0) {
                gather_next_token(parser, TOKEN_ID);

                check_operation(parser,"PKG:VERSION");

                if(parser->current_token_info->token_value && !(parser->current_token_info->token_value[0]=='"')) {
                    parser->PKG_INFO->PKG_VERSION = parser->current_token_info->token_value;
                    gather_next_token(parser, TOKEN_ID);

                    if(parser->current_token_info->token_id == TOKEN_COMMA) gather_next_token(parser, TOKEN_COMMA);
                    else goto END;
                    goto BEGINNING;
                }

            }
            else if(strcmp(parser->current_token_info->token_value,"IMPORT")==0) {
                gather_next_token(parser, TOKEN_ID);

                check_operation(parser,"PKG:IMPORT");

                if(parser->current_token_info->token_value  && !(parser->current_token_info->token_value[0] == '"')) {
                    static int imports;
                    char* file_to_open = strcat(parser->current_token_info->token_value,".jang");

                    Tokens_* tokens = calloc(1,sizeof(*tokens));
                    lexer_* lexer = init_lexer(
                        read_file(file_to_open),
                        tokens
                    );
                    parser_* parser_2 = init_parser(lexer,file_to_open);
                    /*
                        There would be an if statement checking import names, but it seems as if the C compiler covers it for us!
                        If the user imports the 'active file' inside the imported file, the terminal will print 'killed'.
                        If the user imports the currnt 'active file' that was imported in the 'active file', the terminal will
                        print 'killed'
                    */
                    parse(parser_2);

                    if(!(strcmp(parser->current_token_info->token_value,parser->active_file)==0)) {

                        parser->PKG_INFO->current_import_name = realloc(
                            parser->PKG_INFO->current_import_name,
                            strlen(parser->current_token_info->token_value)*sizeof(char*)
                        );

                        parser->PKG_INFO->current_import_name = parser->current_token_info->token_value;
                    }

                    if(parser_2->PKG_INFO->PKG_NAME) {
                        parser->PKG_INFO->amount_of_imports = ++imports;
                        /*parser->PKG_INFO->imports = realloc(
                            parser->PKG_INFO->imports,
                            parser->PKG_INFO->amount_of_imports*sizeof(parser->PKG_INFO->imports)
                        );
                        parser->PKG_INFO->imports[parser->PKG_INFO->amount_of_imports-1] = file_to_open;*/

                        /* Assigning the second parser to the primary parser */
                        parser_2->PKG_INFO = parser->PKG_INFO;
                    }
                }

                gather_next_token(parser, TOKEN_ID);
                if(parser->current_token_info->token_id == TOKEN_COMMA) gather_next_token(parser, TOKEN_COMMA);
                else goto END;
                goto BEGINNING;
            }
            
            END:
            //syntax_tree->amount_of_imported_pkg_names = amount;
            if(parser->current_token_info->token_id == TOKEN_RIGHT_CURL) {
                gather_next_token(parser, TOKEN_RIGHT_CURL);
                if(
                    parser->current_token_info->token_id == EXPORTS_KEYWORD && 
                    strcmp(parser->active_file,"main.jang")==0
                ) {
                    raise_error("\nError: Cannot export any default variables/methods in main Jang file\n\n");
                } else if(
                    parser->current_token_info->token_id == EXPORTS_KEYWORD &&
                    !(
                        strcmp(parser->active_file,"main.jang")==0
                    )
                ) {
                    gather_next_token(parser, EXPORTS_KEYWORD);
                }
            }
            else raise_error("\nExpecting closing '}' on line %d\n\n",parser->lexer->current_line);
            if(parser->current_token_info->token_id==TOKEN_SEMI) gather_next_token(parser, TOKEN_SEMI);
            else raise_error("\nExpecting ';' at end of PKG declaration\n\n");
            parser->lexer->is_special = 1;
            parser->lexer->tokens = assign_current_parser(parser->lexer->tokens, parser);
        } else {
            raise_error("\nError: Expecting '{' on line %d, found %s\n\n",parser->lexer->current_line,parser->current_token_info->token_value);
            //CLOSE(EXIT_FAILURE);
        }
        return syntax_tree;
    } else {
        raise_error("\nError: Expecting \"PKG\" keyword, got token %d\n\n",parser->current_token_info->token_id);
        CLOSE(EXIT_FAILURE);
    }
}
static inline void* parse_id(parser_* parser) {

    return parser;
}

SYN_TREE_* parse(parser_* parser) {
    switch(parser->current_token_info->token_id) {
        case PKG_KEYWORD: return PKG_SETUP(parser);
        case TOKEN_ID: return parse_id(parser);
        default: break;
    }
    return init_syntax_tree(TREE_EOF);
}