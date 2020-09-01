#ifndef LEXER
#define LEXER
#include "tokens.h"

typedef struct lexer {
    const char* contents;
    unsigned int current_line;
    char current_char;
    unsigned int character_number;
    int index;
    char* var_name;
    Tokens_* tokens;
    int pkg_found;
    int is_string_assignment;
    int imports;
    int is_special;
} lexer_;

lexer_* init_lexer();
Tokens_* get_next_token(lexer_* lexer);
void* gather_id_full(lexer_* lexer);
void skip_whitespace(lexer_* lexer);

#endif