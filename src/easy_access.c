#define INCLUDE_ALL_HEADERS
#include "easy_access.h"

#ifdef UNIVERSAL

char* universal_convert_to_string(char __char) {
    char* item = calloc(1,sizeof(char));
    item[0] = __char;
    item[1] = '\0';

    return item;
}

#endif