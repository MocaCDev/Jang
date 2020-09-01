#ifndef E_A
#define E_A
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CLOSE(exit_error) exit(exit_error)
#define raise_error(errmsg, ...) fprintf(stderr,errmsg, ##__VA_ARGS__), CLOSE(EXIT_FAILURE)

#endif