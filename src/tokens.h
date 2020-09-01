#ifndef TOKENS
#define TOKENS
#include "easy_access.h"

typedef struct Tokens {
    enum {
        /* KEYWORD IDEALS */
        TOKEN_ID,
        TOKEN_LEFT_CURL,
        TOKEN_RIGHT_CURL,
        IMPORTS_KEYWORD, // multiple imports
        _PKG_KEYWORD, // Packages the current .jang file
        _pkg_KEYWORD, // sub-package of a file
        EXPORTS_KEYWORD,
        /*END OF KEYWORD IDEALS*/

        /* PUNCTUATION */
        TOKEN_EQUALS,
        TOKEN_COLON,
        TOKEN_COMMA,
        TOKEN_SEMI,
        TOKEN_LEFT_P,
        TOKEN_RIGHT_P,
        /*END OF PUNCTUATION */

        /* END OF FILE. NOT A TOKEN, JUST INITIALIZED AS ONE */
        TOKEN_EOF
        /* END OF END OF FILE */
    } token_id;
    int current_line;
    int current_char;
    char* token_value;
    void* parser;
} Tokens_;

Tokens_* init_token(int token_id, char* value, Tokens_* tokens);
Tokens_* update_token_info(int lines, int char_num, Tokens_* tokens);
Tokens_* assign_current_parser(Tokens_* tokens,void* parser);

#endif