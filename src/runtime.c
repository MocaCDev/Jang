#include "runtime.h"

RUNTIME_IMPORTS_* init_import_runtime(parser_* parser, lexer_* lexer) {
    RUNTIME_IMPORTS_* runtime_imports = calloc(1,sizeof(*runtime_imports));

    runtime_imports->amount_of_imports = 0;
    runtime_imports->lexer_and_parser = calloc(1,sizeof(*runtime_imports->lexer_and_parser));
    runtime_imports->lexer_and_parser->lexer = lexer;
    runtime_imports->lexer_and_parser->parser = parser;
    runtime_imports->import_information = (void*)0;

    return runtime_imports;
}

RUNTIME_EXPORTS_* init_export_runtiem(parser_* parser, lexer_* lexer) {
    RUNTIME_EXPORTS_* runtime_exports = calloc(1,sizeof(*runtime_exports));

    runtime_exports->amount_of_exports = 0;
    runtime_exports->lexer_and_parser->lexer = lexer;
    runtime_exports->lexer_and_parser->parser = parser;
    runtime_exports->export_information = (void*)0;

    return runtime_exports;
}

RUNTIME_PKG_* init_package_runtime(parser_* parser, lexer_* lexer) {
    RUNTIME_PKG_* runtime_package = calloc(1,sizeof(*runtime_package));

    runtime_package->amount_of_packages = 0;
    runtime_package->lexer_and_parser = calloc(2,sizeof(*lexer)*sizeof(*parser));
    runtime_package->lexer_and_parser->lexer = lexer;
    runtime_package->lexer_and_parser->parser = parser;
    runtime_package->package_information = (void*)0;
    return runtime_package;
}

static inline SYN_TREE_* runtime_look_at_package(RUNTIME_PKG_* runtime_package, SYN_TREE_* syntax_tree) {
    printf("HERE!\n");

    return syntax_tree;
}
static inline SYN_TREE_* runtime_look_at_import(RUNTIME_IMPORTS_* runtime_imports, SYN_TREE_* syntax_tree) {

    printf("HERE!!\n");
    return syntax_tree;
}
static inline SYN_TREE_* runtime_look_at_statements(SYN_TREE_* syntax_tree,parser_* parser, lexer_* lexer) {

    for(int i = 0; i < syntax_tree->amount_of_statements; i++) {
        check_tree_type(syntax_tree->syntax_tree_values[i], parser,lexer);
    }

    return syntax_tree;
}

SYN_TREE_* check_tree_type(SYN_TREE_* current_node, parser_* parser, lexer_* lexer) {
    switch(current_node->TREE_TOKEN_TYPE) {
        case TREE_PKG: return runtime_look_at_package(init_package_runtime(parser,lexer),current_node);
        case TREE_IMPORTS: return runtime_look_at_import(init_import_runtime(parser,lexer),current_node);
        case TREE_DEF: return runtime_look_at_statements(current_node,parser,lexer);
        case TREE_EXPORTS: printf("YES\n");break;
        default: break;
    }

    return init_syntax_tree(TREE_EOF);
}