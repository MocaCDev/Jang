#ifndef TOKENS
#define TOKENS

typedef struct Tokens {
    enum {
        /* KEYWORD IDEALS */
        TOKEN_ID,
        TOKEN_LEFT_CURL,
        TOKEN_RIGHT_CURL,
        PKG_KEYWORD,
        /*END OF KEYWORD IDEALS*/

        /* PUNCTUATION */
        TOKEN_EQUALS,
        TOKEN_COLON,
        TOKEN_COMMA,
        TOKEN_SEMI,
        TOKEN_LEFT_P,
        TOKEN_RIGHT_P,
        /*END OF PUNCTUATION */
        TOKEN_EOF
    } token_id;
    char* token_value;
} Tokens_;

Tokens_* init_token(int token_id, char* value);

#endif