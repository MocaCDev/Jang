#include "easy_access.h"

#ifndef UNIVERSAL

char* universal_convert_to_string(char __char) {
    char* item = calloc(1,sizeof(char));
    item[0] = __char;
    item[1] = '\0';

    return item;
}

#endif