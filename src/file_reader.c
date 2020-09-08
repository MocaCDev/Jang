#include "file_reader.h"
#define UNIVERSAL
#include "easy_access.h"
#include <stdio.h>
#include "parser.h"

/* THIS FILE WILL SUPPORT ALL FILE HANDLING WITHIN THE Jang PROGRAMMING LANGUAGE */

static file_buffer* buffer_file_size(size_t size) {
    file_buffer* f_b = calloc(1,sizeof(*f_b));

    f_b->buffer_size = size*8;
    f_b->ammount_of_character = size;

    return f_b;
}

void* gather_main_jang_file() {
    system("python3 src/gather_main.py");

    FILE* find_main = fopen("main.txt","rb");
    char* a = calloc(500,sizeof(char));

    if(!find_main) raise_error("\nNo main file found to execute\n\n");

    fgets(a, 500, find_main);

    if(strcmp(a,"err")==0) raise_error("\nNo main.jang file found to run your Jang application\nPlease create a main.jang file to run your project\n\n");
    fclose(find_main);
    
    return a;
}

/*
    If the extension is not in file_name, we will add it the continue.
*/
char* file_check_extension(char* file_name, char* extension) {

    int index = 0;

    char* curr_extension = calloc(1,sizeof(char));

    for(; index < strlen(file_name); index++) {
        if(file_name[index] == '.') {
            do {
                index++;

                char* new_val = universal_convert_to_string(file_name[index]);

                curr_extension = realloc(
                    curr_extension,
                    strlen(new_val)*sizeof(char*)
                );
                strcat(curr_extension,new_val);
                
            } while(file_name[index] != '\0');
        }
    }

    if(!(strlen(curr_extension) > 1)) {
        strcat(file_name,extension);
    }

    return file_name;
}

char* read_file(const char* file_to_read) {
    char* buffer = 0;
    long length;

    FILE* file = fopen(file_to_read,"rb");

    if(!(file)) {
        printf("\n\nErr reading file: %s\n\n",file_to_read);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    file_buffer* f_b = buffer_file_size(length);
    fseek(file, 0, SEEK_SET);

    if(ASSERT_FILE_BUFFER_SIZE(f_b->ammount_of_character, f_b->buffer_size)==0) {
        buffer = calloc(f_b->ammount_of_character, f_b->buffer_size);
        if(buffer) {
            fread(buffer,1,length,file);
        }
    } else {
        fprintf(stderr,"\nError matching memory size of file.\nTry again.\n\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    free(f_b);

    return buffer;
}