#include "file_ops.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

/**
 * Read regular files as binary and return
 * a file_buffer object on failure returns NULL
*/
file_buffer *read_file(char *file_path) {
  char *buf, *tmp_buf;
  struct stat s;
  size_t file_size, bytes_read, bytes_remaining, total_read = 0;

  if (stat(file_path, &s) == -1) { return NULL; }
  if (!(s.st_mode & S_IFREG)) { return NULL; }

  FILE *fd = fopen(file_path, "rb"); // read as binary
  if (fd == NULL) { return NULL; }

  bytes_remaining = file_size = s.st_size;
  buf = tmp_buf = (char*)malloc(file_size);
  if (buf == NULL) { 
    fclose(fd);
    return NULL;
  }

  while (
    bytes_read = fread(tmp_buf, 1, bytes_remaining, fd), 
    bytes_read != 0 && bytes_remaining > 0
  ) {
    
    if (bytes_read == -1) {
      free(buf);
      fclose(fd);
      return NULL;
    }

    bytes_remaining -= bytes_read;
    total_read +=bytes_read; // incase if the nunmber of bytes read is not equal to the size of the bytes
    tmp_buf += bytes_read;
    
    file_buffer *fb = (file_buffer*)malloc(sizeof(file_buffer));
    fb->size = total_read;
    fb->file_content = buf;
    fb->mime_type = "";

    fclose(fd);
    return fb;
  }
  return NULL;
}

/**
 * Free a given file_buffer object
*/
void free_buffer(file_buffer *file_buff) {
  if (file_buff) {
    free(file_buff->file_content);
    if (file_buff->mime_type) free(file_buff->mime_type);
    free(file_buff);
  }
}
