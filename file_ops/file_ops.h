#ifndef _FILE_OPS_H
#define _FILE_OPS_H

#include <stdlib.h>

typedef struct file_buffer {
    char *file_content;
    char *mime_type;
    size_t size;
} file_buffer;

file_buffer* read_file(char*);
void free_buffer(file_buffer*);

#endif /* _FILE_OPS_H */
