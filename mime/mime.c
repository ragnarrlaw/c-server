#include "mime.h"
#include <string.h>

const char *get_file_extension(const char *filename);

const char *get_file_mime_type(const char *filename) {
  const char *extension = get_file_extension(filename);

  if (strcasecmp(extension, "jpg") == 0 || strcasecmp(extension, "jpeg") == 0) {
    return "image/jpeg";
  }
  if (strcasecmp(extension, "png") == 0) {
    return "image/png";
  }
  if (strcasecmp(extension, "pdf") == 0) {
    return "application/pdf";
  }
  if (strcasecmp(extension, "json") == 0) {
    return "application/json";
  }
  if (strcasecmp(extension, "html") == 0 || strcasecmp(extension, "htm") == 0) {
    return "text/html";
  }
  if (strcasecmp(extension, "css") == 0) {
    return "text/css";
  }
  if (strcasecmp(extension, "js") == 0 || strcasecmp(extension, "mjs") == 0) {
    return "text/javascript";
  }
  if (strcasecmp(extension, "ico") == 0) {
    return "image/vnd";
  }
  return "application/octet-stream";
}

const char *get_file_extension(const char *filename) {
  const char *dot = strrchr(filename, '.');
  if (!dot || dot == filename)
    return "";
  return dot + 1;
}
