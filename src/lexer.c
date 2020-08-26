#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

lexer_* init_lexer(const char* file_contents, Tokens_* tokens) {
    lexer_* lexer = calloc(1,sizeof(*lexer));

    lexer->contents = file_contents;
    lexer->current_line = 1;
    lexer->var_name = (char*) malloc(sizeof(lexer->var_name));
    lexer->character_number = 1;
    lexer->index = 0;
    lexer->current_char = lexer->contents[lexer->index];
    lexer->tokens = tokens;
    lexer->pkg_found = 1;

    return lexer;
}

static inline void advance(lexer_* lexer) {
    if(!(lexer->index > strlen(lexer->contents) || lexer->current_char == '\0')) {
        lexer->index++;
        lexer->current_char = lexer->contents[lexer->index];
        lexer->character_number++;

        if(lexer->current_char == '\n') {
            lexer->current_line++;
            lexer->character_number = 1;
        }
        
        if(lexer->current_char == '_') {
            advance(lexer);
        }
    }
}

static inline void skip_whitespace(lexer_* lexer) {
    static int amount_of_whitespace;
    do {
        advance(lexer);
        ++amount_of_whitespace;
    } while(lexer->current_char == ' ' || lexer->current_char == 0xC);
    
    if(
        amount_of_whitespace > 2 && !(
            lexer->pkg_found == 0
        )
    ) {
        
        fprintf(stdout, "\nWarning: Indention of %d is not needed on line %d(starts %d characters in)\n\n",amount_of_whitespace,lexer->current_line,(lexer->character_number-(amount_of_whitespace)-1));
    }
}

static inline char* convert_to_string(lexer_* lexer) {
    char* value = calloc(2,sizeof(char));

    value[0] = lexer->current_char;
    value[1] = '\0';

    return value;
}

static inline void* gather_converted_string(lexer_* lexer, int is_variable_name) {
    char* string_value = calloc(1,sizeof(char));

    do {
        char* current = convert_to_string(lexer);

        string_value = realloc(
            string_value,
            strlen(current)*sizeof(char*)
        );
        strcat(string_value,current);
        //free(current);
        advance(lexer);
    }  while(isalnum(lexer->current_char));
    
    if(is_variable_name==0) {
        lexer->var_name = string_value;
        return lexer->var_name;
    }
    //printf("%s",string_value);
    if(is_variable_name==1)  lexer->tokens = init_token(TOKEN_ID, string_value);

    return lexer;
}

static inline Tokens_* gather_id(lexer_* lexer) {
    gather_converted_string(lexer,1);
    return lexer->tokens;
}

static inline Tokens_* advance_with_token(Tokens_* token, lexer_* lexer) {
    advance(lexer);
    lexer->tokens = token;
    return token;
}

Tokens_* get_next_token(lexer_* lexer) {
    do {

        if(lexer->current_char == ' ' || lexer->current_char == 0xC)
            skip_whitespace(lexer);
        
        if(lexer->current_char=='\n') advance(lexer);

        if(isalnum(lexer->current_char)) {
            gather_id(lexer);
            if(
                lexer->pkg_found!=0 &&
                (strcmp(lexer->tokens->token_value,"PKG")==0 ||
                strcmp(lexer->tokens->token_value,"pkg")==0)
            ) {
                lexer->tokens = init_token(PKG_KEYWORD, lexer->tokens->token_value);

                //lexer->pkg_found = 0;
                return lexer->tokens;
            }
            lexer->tokens = init_token(TOKEN_ID,lexer->tokens->token_value);
            return lexer->tokens;
        }
        
        switch(lexer->current_char) {
            case '{': return advance_with_token(init_token(TOKEN_LEFT_CURL,convert_to_string(lexer)), lexer);
            case '}': return advance_with_token(init_token(TOKEN_RIGHT_CURL,convert_to_string(lexer)),lexer);
            default: break;
        }
        
    } while(lexer->current_char != '\0' && lexer->index < strlen(lexer->contents));

    return init_token(TOKEN_EOF,"\0");
}