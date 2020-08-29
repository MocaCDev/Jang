#ifndef file_reader
#define file_reader
#include <stdlib.h>

typedef struct FILE_BUFFER {
    size_t buffer_size;
    size_t ammount_of_character;
} file_buffer;

char* read_file(const char* file_to_read);
void* gather_main_jang_file();

#define ASSERT_FILE_BUFFER_SIZE(size, match) ( (((f_b->ammount_of_character|f_b->buffer_size)&f_b->ammount_of_character) == size) ? 0 : 1 )

#endif