#ifndef E_A
#define E_A
#include <stdio.h>

#define CLOSE(exit_error) exit(exit_error)
#define raise_error(errmsg, ...) fprintf(stderr,errmsg, ##__VA_ARGS__), CLOSE(EXIT_FAILURE)

#endif