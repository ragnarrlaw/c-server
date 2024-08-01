#include "mime.h"
#include <stdlib.h>
#include <string.h>

const char *get_file_mime_type(const char *buffer, size_t buffer_length) {
  if (buffer_length >= MAGIC_SIZE && memcmp(buffer, "\xFF\xD8\xFF", 3) == 0) {
    return "image/jpeg";
  }
  if (buffer_length >= MAGIC_SIZE &&
      memcmp(buffer, "\x89PNG\r\n\x1A\n", 8) == 0) {
    return "image/png";
  }
  if (buffer_length >= MAGIC_SIZE && memcmp(buffer, "%PDF-", 5) == 0) {
    return "application/pdf";
  }
  if (buffer_length >= 5 && memcmp(buffer, "{\n", 2) == 0) {
    return "application/json";
  }
  if (buffer_length >= 4 && memcmp(buffer, "<!DOCTYPE", 9) == 0) {
    return "text/html";
  }
  return "application/octet-stream";
}
