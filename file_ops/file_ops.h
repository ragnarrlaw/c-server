#ifndef _FILE_OPS_H
#define _FILE_OPS_H

#include <stdlib.h>

typedef struct FileBuffer {
    char *file_content;
    const char *mime_type;
    size_t size;
} FileBuffer;

FileBuffer* read_file(char* file_path);
void free_buffer(FileBuffer* file_buff);

#endif /* _FILE_OPS_H */
