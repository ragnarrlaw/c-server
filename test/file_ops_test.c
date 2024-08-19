#include "../file_ops/file_ops.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  printf("<===================File OPS==========================>\n");
  FileBuffer *fb = read_file("/home/bobby/playground/c/server/test/files/conf.json");
  if (fb == NULL) return EXIT_FAILURE;
  printf("File mime-type: %s\n", fb->mime_type);
  free_buffer(fb);
  if (fb == NULL) return EXIT_FAILURE;
  printf("<===================File OPS==========================>\n");
  fb = read_file("/home/bobby/playground/c/server/test/files/index.html");
  printf("File mime-type: %s\n", fb->mime_type);
  free_buffer(fb);
  return EXIT_SUCCESS;
}
