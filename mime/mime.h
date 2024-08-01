#ifndef _MIME_H
#define _MIME_H

#include <stdlib.h>
#define MAGIC_SIZE 8

const char *get_file_mime_type(const char* buffer, size_t len);

#endif /* ifndef _MIME_H */
