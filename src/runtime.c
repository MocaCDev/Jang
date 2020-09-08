#include "runtime.h"
#include "file_reader.h"
#include "tokens.h"
#include "lexer.h"

RUNTIME_IMPORTS_* init_import_runtime(char* imported_file_name, parser_* parser, char* main_file) {
    RUNTIME_IMPORTS_* runtime_imports = calloc(1,sizeof(*runtime_imports));

    runtime_imports->amount_of_imports = 0;
    runtime_imports->import_information = (void*)0;

    /* Lexer and parser setup */
    runtime_imports->lexer_and_parser = calloc(1,sizeof(*runtime_imports->lexer_and_parser));

    /* Lexer setup */
    Tokens_* tokens = calloc(1,sizeof(*tokens));
    file_check_extension(imported_file_name,".jang"); // checking for .jang as the extension
    
    /* Checking to see if the file we are importing is the same as the currently active file */
    if(strcmp(imported_file_name,parser->active_file)==0)raise_error("\nCannot import '%s' into its own file(line %d)\n\n",imported_file_name,parser->lexer->current_line);

    /* Checking to see if the file we are importing is the same as the main file */
    if(strcmp(imported_file_name,main_file)==0) raise_error("\nCannot import main file(%s) into '%s'(line %d)\n\n",main_file,parser->active_file,parser->lexer->current_line-1);

    runtime_imports->lexer_and_parser->lexer = init_lexer(read_file(imported_file_name),tokens);

    /* Parser setup */
    runtime_imports->lexer_and_parser->parser = init_parser(runtime_imports->lexer_and_parser->lexer, (char*) imported_file_name);

    return runtime_imports;
}

RUNTIME_EXPORTS_* init_export_runtime(parser_* parser, lexer_* lexer) {
    RUNTIME_EXPORTS_* runtime_exports = calloc(1,sizeof(*runtime_exports));

    runtime_exports->amount_of_exports = 0;
    runtime_exports->lexer_and_parser->lexer = lexer;
    runtime_exports->lexer_and_parser->parser = parser;
    runtime_exports->export_information = (void*)0;

    return runtime_exports;
}

RUNTIME_PKG_* init_package_runtime(parser_* parser, lexer_* lexer) {
    if(!(lexer->has_package == 0)) {
        RUNTIME_PKG_* runtime_package = calloc(1,sizeof(*runtime_package));
        lexer->has_package = 0;

        runtime_package->amount_of_packages = 0;
        runtime_package->lexer_and_parser = calloc(2,sizeof(*lexer)*sizeof(*parser));
        runtime_package->lexer_and_parser->lexer = lexer;
        runtime_package->lexer_and_parser->parser = parser;
        runtime_package->package_information = (void*)0;
        return runtime_package;
    } else {
        raise_error("\nThe file '%s' already has a package.\nRemove line %d to resolve this error\n\n",parser->active_file,lexer->current_line);
    }
}

static inline SYN_TREE_* runtime_look_at_package(RUNTIME_PKG_* runtime_package, SYN_TREE_* syntax_tree) {
    printf("Package!\n");

    return syntax_tree;
}

/* 
    This will open the imported file, read the source code, the continue.
    ToDo: Find out a way store the imported file code for later use in the file it was imported in.
        -> This is done by storing all ideal variable/function etc names and values/functionalities to refer to when being used in the file it was all imported in
*/
static inline SYN_TREE_* runtime_look_at_import(RUNTIME_IMPORTS_* runtime_imports, SYN_TREE_* syntax_tree, char* main_file_path) {

    printf("Import!!\n");

    SYN_TREE_* tree = parse(runtime_imports->lexer_and_parser->parser);
    check_tree_type(tree, runtime_imports->lexer_and_parser->parser, runtime_imports->lexer_and_parser->lexer,main_file_path);

    return syntax_tree;
}
static inline SYN_TREE_* runtime_look_at_statements(SYN_TREE_* syntax_tree,parser_* parser, lexer_* lexer,char* main_file_path) {

    for(int i = 0; i < syntax_tree->amount_of_statements; i++) {
        check_tree_type(syntax_tree->syntax_tree_values[i], parser,lexer,main_file_path);
    }

    return syntax_tree;
}

SYN_TREE_* check_tree_type(SYN_TREE_* current_node, parser_* parser, lexer_* lexer, char* main_file_path) {
    switch(current_node->TREE_TOKEN_TYPE) {
        case TREE_PKG: return runtime_look_at_package(init_package_runtime(parser,lexer),current_node);
        case TREE_IMPORTS: return runtime_look_at_import(init_import_runtime(current_node->import_names[current_node->amount_of_imports-1],parser,main_file_path),current_node, main_file_path);
        case TREE_DEF: return runtime_look_at_statements(current_node,parser,lexer,main_file_path);
        case TREE_EXPORTS: printf("Exports\n");break;
        default: break;
    }

    return init_syntax_tree(TREE_EOF);
}