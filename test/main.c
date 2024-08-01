#include "../file_ops/file_ops.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  printf("<===================File OPS==========================>\n");
  FileBuffer *fb = read_file("../conf.json");
  printf("File mime-type: %s\n", fb->mime_type);
  free_buffer(fb);
  printf("<===================File OPS==========================>\n");
  fb = read_file("../public/index.html");
  printf("File mime-type: %s\n", fb->mime_type);
  free_buffer(fb);
  return EXIT_SUCCESS;
}
