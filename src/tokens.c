#include "tokens.h"
#include "easy_access.h"
#include "parser.h"

static inline void* assign(Tokens_* tokens, int id, char* value) {
    tokens->token_id = id;
    tokens->token_value = value;

    return tokens;
}

/* This function keeps up to date with 2 lexer struct ideals: current_line, character_number*/
Tokens_* update_token_info(int lines, int char_num, Tokens_* tokens) {
    tokens->current_line = lines;
    tokens->current_char = char_num;
    
    return tokens;
}
/* This function keeps up to date with the parser for the Tokens_ struct */
Tokens_* assign_current_parser(Tokens_* tokens,void* parser) {
    tokens->parser = ((parser_*)parser);
    return tokens;
}

Tokens_* init_token(int token_id, char* value, Tokens_* tokens) {
    if(!tokens) {
        tokens = calloc(1,sizeof(*tokens));
    }
    tokens->token_value = (char*) malloc(sizeof(tokens->token_value));

    switch(token_id) {
        case TOKEN_ID: {
            return assign(tokens, token_id, value);
        }
        case TOKEN_EQUALS: {
            if(strcmp(value,"=")==0) return assign(tokens,token_id,value);
        }
        case TOKEN_COLON: {
            if(strcmp(value,":")==0) return assign(tokens,token_id,value);
        }
        case TOKEN_COMMA: {
            if(strcmp(value,",")==0) return assign(tokens,token_id,value);
        }
        case TOKEN_SEMI: {
            if(strcmp(value,";")==0) return assign(tokens,token_id,value);
        }
        case TOKEN_LEFT_P: {
            if(strcmp(value,"(")==0) return assign(tokens,token_id,value);
        }
        case TOKEN_RIGHT_P: {
            if(strcmp(value,")")==0) return assign(tokens,token_id,value);
        }
        case TOKEN_LEFT_CURL: {
            if(strcmp(value,"{")==0) return assign(tokens,token_id,value);
        }
        case TOKEN_RIGHT_CURL: {
            if(strcmp(value, "}")==0) return assign(tokens,token_id,value);
        }
        case TOKEN_EOF: {
            if(strcmp(value,"\0")==0) return assign(tokens,token_id,value);
        }
        case EXPORTS_KEYWORD: {
            if(strcmp(value,"_EXPORTS_")==0) return assign(tokens,token_id,value);
        }
        case IMPORTS_KEYWORD: {
            if(strcmp(value,"IMPORT")==0) return assign(tokens, token_id, value);
        }
        default: {
            raise_error("\nValue does not match with token id(%s L:%d C:%d)\n\n",value,tokens->current_line,tokens->current_char-1);
            CLOSE(-1);
        }
    }
}