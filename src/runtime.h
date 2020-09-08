#ifndef RUNTIME
#define RUNTIME
#include "lexer.h"
#include "syn_tree.h"
#include "parser.h"
#include "easy_access.h"

/* This will be a struct pointer in all typedef structs that need to store the lexer and parser */
struct Basic_Lexer_Parser {
    lexer_* lexer;
    parser_* parser;
} BASIC_LEXER_PARSER;

typedef struct Basic_Lexer_Parser l_a_p;

/* FOR IMPORTS */
typedef struct RUNTIME_IMPORTS {
    SYN_TREE_** import_information;
    size_t amount_of_imports;
    l_a_p* lexer_and_parser; /*
        Both will be init of each struct.
    */
    char* curent_file_name;
} RUNTIME_IMPORTS_;

typedef struct RUNTIME_PKG {
    SYN_TREE_** package_information;
    size_t amount_of_packages; /*
        This may not be needed.
        But, I may add in a feature where a file does not need a package declaration.
        But for now, it is a must.
    */
    l_a_p* lexer_and_parser;
} RUNTIME_PKG_;

typedef struct RUNTIME_EXPORTS {
    SYN_TREE_** export_information; /*
        This should store all export variable names, functions etc..
    */
    size_t amount_of_exports;
    l_a_p* lexer_and_parser;
} RUNTIME_EXPORTS_;

/* RUNTIME_IMPORTS */
RUNTIME_IMPORTS_* init_import_runtime(SYN_TREE_* current_node, char* import_file_name, parser_* parser, char* main_file);

/* RUNTIME_PKG */
RUNTIME_PKG_* init_package_runtime(parser_* parser, lexer_* lexer);

/* RUNTIME_EXPORTS  */
RUNTIME_EXPORTS_* init_export_runtime(parser_* parser, lexer_* lexer);

SYN_TREE_* check_tree_type(SYN_TREE_* current_node, parser_* parser, lexer_* lexer, char* main_file_path);

#endif