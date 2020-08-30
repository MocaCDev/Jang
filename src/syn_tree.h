#ifndef SYNT_TREE
#define SYNT_TREE
#include "parser.h"
#include <stdlib.h>

typedef struct SYN_TREE {
    enum {
        TREE_PKG,
        TREE_EXPORTS,
        TREE_EOF
    } TREE_TOKEN_TYPE;

    /* PKG INFORMATION */
    /* IMPORTED PKG NAME INFORMATION. THIS HAS NOTHING TO DO WITH IMPORTED FILE NAMES */
    size_t amount_of_imported_pkg_names;
    char** imported_pkg_names;
    parser_** parser_pkg_information; /*
        This parser struct array will be used to check each pkg name
    */

    /* MAIN PKG NAME. THIS HAS NOTHING TO DO WITH ANYTHING RELATED TO IMPORTS */
    char* main_pkg_name;

    /* MAIN PKG IMPORTS */
    size_t amount_of_imports;
    char** import_names;
    parser_** parser_import_information; /*
        This parser struct array will be used to check each import name
        against each ideal in the import_names array
    */

    /* IMPORTED PKG IMPORTS. THIS IS ALL IMPORT INFORMATION TAKEN PLACE IN IMPORTED FILES */
    size_t amount_of_secondary_imports;
    char** secondary_imports;
    parser_** parser_2_imports; /*
        This parser struct array will be used to check each import name within the parser struct array
        and the secondary_imports array
    */
} SYN_TREE_;

SYN_TREE_* init_syntax_tree(int SYNTAX_TREE_TOKEN_ID);

#endif