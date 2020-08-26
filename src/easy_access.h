#ifndef E_A
#define E_A

#define raise_error(errmsg, ...) fprintf(stderr,errmsg, ##__VA_ARGS__)
#define CLOSE(exit_error) exit(exit_error)

#endif