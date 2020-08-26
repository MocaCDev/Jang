#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tokens.h"
#include "easy_access.h"

static inline void* assign(Tokens_* tokens, int id, char* value) {
    tokens->token_id = id;
    tokens->token_value = value;

    return tokens;
}

Tokens_* init_token(int token_id, char* value) {
    Tokens_* tokens = calloc(1,sizeof(*tokens));
    tokens->token_value = (char*) malloc(sizeof(tokens->token_value));

    switch(token_id) {
        case TOKEN_ID: {
            return assign(tokens, token_id, value);
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
        case PKG_KEYWORD: {
            if(
                strcmp(value,"PKG")==0 ||
                strcmp(value,"pkg")==0
            ) return assign(tokens, token_id, value);
        }
        default: {
            raise_error("\nValue does not match with token id\n\n");
            CLOSE(-1);
        }
    }
}