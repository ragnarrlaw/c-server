#include <stdlib.h>
#include <string.h>

#include "mime.h"

const char *get_file_extension(const char *filename) {
  const char *dot = strrchr(filename, '.');
  if (!dot || dot == filename)
    return "";
  return dot + 1;
}

const char *get_file_mime_type(const char *buffer, size_t buffer_length) {

  if (buffer_length >= 3 && memcmp(buffer, "\xFF\xD8\xFF", 3) == 0) {
    return "image/jpeg";
  }
  if (buffer_length >= 8 && memcmp(buffer, "\x89PNG\r\n\x1A\n", 8) == 0) {
    return "image/png";
  }
  if (buffer_length >= 5 && memcmp(buffer, "%PDF-", 5) == 0) {
    return "application/pdf";
  }
  if (buffer_length >= 2 && memcmp(buffer, "{\n", 2) == 0) {
    return "application/json";
  }
  if (buffer_length >= 9 && memcmp(buffer, "<!DOCTYPE", 9) == 0) {
    return "text/html";
  }

  if (buffer_length >= 10 &&
      (memcmp(buffer, "function", 8) == 0 || memcmp(buffer, "var ", 4) == 0 ||
       memcmp(buffer, "let ", 4) == 0 || memcmp(buffer, "const ", 6) == 0 ||
       memcmp(buffer, "(function", 9) == 0)) {
    return "text/javascript";
  }

  if (buffer_length >= 6 &&
      (memcmp(buffer, "body {", 6) == 0 || memcmp(buffer, ".class {", 8) == 0 ||
       memcmp(buffer, "#id {", 5) == 0 || memcmp(buffer, "@import", 7) == 0 ||
       memcmp(buffer, "@media", 6) == 0)) {
    return "text/css";
  }

  return "application/octet-stream";
}

const char *get_file_mime_type_filename(const char *filename,
                                        const char *buffer,
                                        size_t buffer_length) {
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
  if (strcasecmp(extension, "js") == 0) {
    return "application/javascript";
  }

  if (buffer_length >= 3 && memcmp(buffer, "\xFF\xD8\xFF", 3) == 0) {
    return "image/jpeg";
  }
  if (buffer_length >= 8 && memcmp(buffer, "\x89PNG\r\n\x1A\n", 8) == 0) {
    return "image/png";
  }
  if (buffer_length >= 5 && memcmp(buffer, "%PDF-", 5) == 0) {
    return "application/pdf";
  }
  if (buffer_length >= 2 && memcmp(buffer, "{\n", 2) == 0) {
    return "application/json";
  }
  if (buffer_length >= 9 && memcmp(buffer, "<!DOCTYPE", 9) == 0) {
    return "text/html";
  }

  if (buffer_length >= 10 &&
      (memcmp(buffer, "function", 8) == 0 || memcmp(buffer, "var ", 4) == 0 ||
       memcmp(buffer, "let ", 4) == 0 || memcmp(buffer, "const ", 6) == 0 ||
       memcmp(buffer, "(function", 9) == 0)) {
    return "application/javascript";
  }

  if (buffer_length >= 6 &&
      (memcmp(buffer, "body {", 6) == 0 || memcmp(buffer, ".class {", 8) == 0 ||
       memcmp(buffer, "#id {", 5) == 0 || memcmp(buffer, "@import", 7) == 0 ||
       memcmp(buffer, "@media", 6) == 0)) {
    return "text/css";
  }

  return "application/octet-stream";
}
