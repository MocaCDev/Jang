#ifndef E_A
#define E_A
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef INCLUDE_ALL_HEADERS

#include "file_reader.h"
#include "lexer.h"
#include "parser.h"
#include "runtime.h"
#include "syn_tree.h"
#include "tokens.h"

#endif

#define CLOSE(exit_error) exit(exit_error)
#define raise_error(errmsg, ...) fprintf(stderr,errmsg, ##__VA_ARGS__), CLOSE(EXIT_FAILURE)

#ifdef DECLARE_KEYWORDS

/* All exporting/importing ideals */
#define _EXPORTS_KEYWORD_ "_EXPORTS_"
#define _IMPORTS_KEYWORD_ "IMPORT"
/* Packaging ideals */
#define _PKG_KEYWORD_ "PKG"
#define _pkg_KEYWORD_ "pkg"

#undef FOR_LEXER
#endif

#ifdef UNIVERSAL

char* universal_convert_to_string(char __char);

#endif

#endif