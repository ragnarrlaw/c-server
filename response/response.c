#include "response.h"
#include "../mime/mime.h"
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define INDEX_FILE_NAME "index.html"

const char *get_status_message(int code);
const char *get_file_extension(const char *filename);
const char *get_file_mime_type(const char *filename);

char not_found[] = "<!DOCTYPE html><html lang=\"en\"><head>"
                  "<meta charset = \"UTF-8\"/>"
                  "<meta name = \"viewport\" content = \"width=device-width, initial-scale=1.0\"/>"
                  "<title> Page Not Found </title> </head><body><p><b>"
                  "404</b> </p><hr/><p><b>Page Not Found</b></p>"
                  "</body></html>";

char internal_server_error[] =
    "<!DOCTYPE html><html lang = \"en\"><head>"
    "<meta charset = \"UTF-8\" />"
    "<meta name = \"viewport\" content = "
    "\"width=device-width, initial-scale=1.0\" />"
    "<title> Internal Server Error</title></head>"
    "<body><p><b> 500 </b> </p><hr /><p> <b> "
    "Internal Server Error</b></p></body></html>";

char bad_request[] = "<!DOCTYPE html><html lang = \"en\"><head>"
                    "<meta charset = \"UTF-8\"/>"
                    "<meta name = \"viewport\" content = \"width=device-width, initial-scale=1.0\" />"
                    "<title>Bad Request</title></head>"
                    "<body><p><b>400</b></p><hr/><p><b> "
                    "Bad Request</b></p></body></html>";

void send_file_response(int cli_sock, const char *file_path, const char *headers) {
  char *buf, *tmp_buf;
  struct stat s;
  size_t file_size, bytes_read, bytes_remaining, total_read = 0;

  // Check if the file exists
  if (stat(file_path, &s) == -1) {
    perror(">>>> stat error during file info retrieval");
    send_response(cli_sock, NOT_FOUND, "text/html", not_found, strlen(not_found), NULL);
    return;
  }

  // Check if the file is a directory
  if (S_ISDIR(s.st_mode)) {
    char index_path[PATH_MAX];
    snprintf(index_path, sizeof(index_path), "%s/%s", file_path, INDEX_FILE_NAME);
    if (stat(index_path, &s) == -1) {
      send_response(cli_sock, BAD_REQUEST, "text/html", bad_request, strlen(bad_request), NULL);
      return;
    }
    file_path = index_path;
  }

  if (S_ISLNK(s.st_mode)) {
    char real_path[PATH_MAX];
    if (realpath(file_path, real_path) == NULL) {
      perror(">>>> realpath error");
      send_response(cli_sock, BAD_REQUEST, "text/html", bad_request, strlen(bad_request), NULL);
      return;
    }
    if (stat(real_path, &s) == -1) {
      perror(">>>> stat error during symlink resolution");
      send_response(cli_sock, NOT_FOUND, "text/html", not_found, strlen(not_found), NULL);
      return;
    }
    if (S_ISDIR(s.st_mode)) {
      char index_path[PATH_MAX];
      snprintf(index_path, sizeof(index_path), "%s/%s", real_path, INDEX_FILE_NAME);
      if (stat(index_path, &s) == -1) {
        perror(">>>> index.html not found in symlinked directory");
        send_response(cli_sock, BAD_REQUEST, "text/html", bad_request, strlen(bad_request), NULL);
        return;
      }
      file_path = index_path;
    } else {
      file_path = real_path;
    }
  }

  if (!S_ISREG(s.st_mode)) {
    perror(">>>> not a regular file");
    send_response(cli_sock, BAD_REQUEST, "text/html", bad_request, strlen(bad_request), NULL);
    return;
  }

  if (s.st_size == 0) {
    send_response(cli_sock, 204, "application/x-empty", NULL, 0, NULL);
    return;
  }

  FILE *fd = fopen(file_path, "rb"); // read as binary
  if (fd == NULL) {
    perror(">>>> file open error");
    send_response(cli_sock, INTERNAL_SERVER_ERROR, "text/html", internal_server_error, strlen(internal_server_error), NULL);
    return;
  }

  if (s.st_size > MAX_BUFFER_SIZE) {
    char *buf = (char *)malloc(CHUNK_SIZE);
    if (buf == NULL) {
      fclose(fd);
      perror(">>>> file buffer allocation error");
      send_response(cli_sock, INTERNAL_SERVER_ERROR, "text/html", internal_server_error, strlen(internal_server_error), NULL);
      return;
    }

    char initial_header[102]; // don't need the Content-Length header for
                              // chunked encoding
    snprintf(initial_header, sizeof(initial_header),
             "HTTP/1.1 200 OK\r\n"
             "Transfer-Encoding: chunked\r\n"
             "Content-Type: %s\r\n"
             "\r\n",
             get_file_mime_type(file_path));
    send(cli_sock, initial_header, strlen(initial_header), 0);

    size_t bytes_read;
    while ((bytes_read = fread(buf, 1, CHUNK_SIZE, fd)) > 0) {
      if (ferror(fd)) {
        free(buf);
        fclose(fd);
        perror(">>>> file read error");
        send(cli_sock, "0\r\n\r\n", 5, 0); // end chunked transfer encoding
        send_response(cli_sock, INTERNAL_SERVER_ERROR, "text/html", internal_server_error, strlen(internal_server_error), NULL); // send error response
        return;
      }

      char chunk_header[16];
      snprintf(chunk_header, sizeof(chunk_header), "%X\r\n", (unsigned int)bytes_read);

      // Calculate total size for the concatenated buffer
      size_t total_size = strlen(chunk_header) + bytes_read + 2; // 2 for "\r\n"
      char *send_buf = (char *)malloc(total_size);
      if (send_buf == NULL) {
        free(buf);
        fclose(fd);
        perror(">>>> send buffer allocation error");
        send(cli_sock, "0\r\n\r\n", 5, 0); // end chunked transfer encoding
        send_response(cli_sock, INTERNAL_SERVER_ERROR, "text/html", internal_server_error, strlen(internal_server_error), NULL); // send error response
        return;
      }

      // copy chunk header, chunk data, and chunk terminator into send_buf - used to avoid multiple sends
      memcpy(send_buf, chunk_header, strlen(chunk_header));
      memcpy(send_buf + strlen(chunk_header), buf, bytes_read);
      memcpy(send_buf + strlen(chunk_header) + bytes_read, "\r\n", 2);

      send(cli_sock, send_buf, total_size, 0);

      free(send_buf);
    }

    send(cli_sock, "0\r\n\r\n", 5, 0); // end chunked transfer encoding

    free(buf);
    fclose(fd);
  } else {
    bytes_remaining = file_size = s.st_size;
    buf = tmp_buf = (char *)malloc(file_size);
    if (buf == NULL) {
      fclose(fd);
      perror(">>>> file buffer allocation error");
      send_response(cli_sock, INTERNAL_SERVER_ERROR, "text/html",
                    internal_server_error, strlen(internal_server_error), NULL);
      return;
    } else {
      while ((bytes_read = fread(tmp_buf, 1, bytes_remaining, fd)) != 0 &&
             bytes_remaining > 0) {
        if (ferror(fd)) {
          free(buf);
          fclose(fd);
          perror(">>>> file read error");
          send_response(cli_sock, INTERNAL_SERVER_ERROR, "text/html",
                        internal_server_error, strlen(internal_server_error),
                        NULL);
          return;
        }
        bytes_remaining -= bytes_read;
        total_read += bytes_read;
        tmp_buf += bytes_read;
      }
      send_response(cli_sock, OK, get_file_mime_type(file_path), buf,
                    total_read, NULL);
      free(buf);
      fclose(fd);
      return;
    }
  }
}

void send_response(int cli_sock, ResponseCode code, const char *mime_type,
                   const char *body, size_t body_len, const char *headers) {
  char response[RESPONSE_BUFFER_SIZE];
  const char *status_message = get_status_message(code);
  int offset = snprintf(response, RESPONSE_BUFFER_SIZE, "HTTP/1.1 %d %s\r\n",
                        code, status_message);

  if (mime_type) {
    offset += snprintf(response + offset, RESPONSE_BUFFER_SIZE - offset,
                       "Content-Type: %s\r\n", mime_type);
  }
  if (body_len > 0) {
    offset += snprintf(response + offset, RESPONSE_BUFFER_SIZE - offset,
                       "Content-Length: %zu\r\n", body_len);
  }
  if (headers) {
    offset += snprintf(response + offset, RESPONSE_BUFFER_SIZE - offset, "%s",
                       headers);
  }

  if (body) {
    snprintf(response + offset, RESPONSE_BUFFER_SIZE - offset, "\r\n%s", body);
  } else {
    snprintf(response + offset, RESPONSE_BUFFER_SIZE - offset, "\r\n");
  }

  send(cli_sock, response, strlen(response), 0);
}

const char *get_status_message(int code) {
  switch (code) {
  case 200:
    return "OK";
  case 201:
    return "Created";
  case 202:
    return "Accepted";
  case 204:
    return "No Content";
  case 301:
    return "Moved Permanently";
  case 302:
    return "Found";
  case 303:
    return "See Other";
  case 304:
    return "Not Modified";
  case 307:
    return "Temporary Redirect";
  case 308:
    return "Permanent Redirect";
  case 400:
    return "Bad Request";
  case 401:
    return "Unauthorized";
  case 403:
    return "Forbidden";
  case 404:
    return "Not Found";
  case 405:
    return "Method Not Allowed";
  case 408:
    return "Request Timeout";
  case 500:
    return "Internal Server Error";
  case 501:
    return "Not Implemented";
  case 503:
    return "Service Unavailable";
  case 504:
    return "Gateway Timeout";
  case 505:
    return "HTTP Version Not Supported";
  default:
    return "Unknown Status";
  }
}
