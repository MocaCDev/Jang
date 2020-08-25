#ifndef LEXER
#define LEXER
#include "tokens.h"

typedef struct lexer {
    const char* contents;
    unsigned int current_line;
    char current_char;
    int index;
    char* var_name;
    Tokens_* tokens;
} lexer_;

lexer_* init_lexer();
Tokens_* get_next_token(lexer_* lexer);

#endif