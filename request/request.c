#include "request.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *find_start_of_body(char *header);

Request *init_request(char *buffer, unsigned int length) {
  Request *request = (Request *)malloc(sizeof(Request));
  if (!request)
    return NULL;

  request->req_data = init_dictionary();
  if (!request->req_data) {
    free_request(request);
    return NULL;
  }

  char *req_body_start = find_start_of_body(buffer);

  char *split_req_line_save_ptr;
  char *split_line_save_ptr;
  char *split_param_save_ptr;

  char *request_line = strtok_r(buffer, "\r\n", &split_line_save_ptr);
  if (!request_line) {
    free_request(request);
    return NULL;
  }

  char *line;
  while ((line = strtok_r(NULL, "\r\n", &split_line_save_ptr)) &&
         strlen(line) > 0) {
    char *key = strtok_r(line, ":", &split_param_save_ptr);
    char *value = strtok_r(NULL, ":", &split_param_save_ptr);

    if (key && value) {
      while (isspace((unsigned char)*value))
        value++;
      insert_kv(request->req_data, key, value);
    }
  }

  char *req_line_save_ptr;
  char *method = strtok_r(request_line, " ", &req_line_save_ptr);
  char *url = strtok_r(NULL, " ", &req_line_save_ptr);
  char *version = strtok_r(NULL, " ", &req_line_save_ptr);

  request->type = (strcmp(method, "GET") == 0)      ? REQUEST_GET
                  : (strcmp(method, "POST") == 0)   ? REQUEST_POST
                  : (strcmp(method, "DELETE") == 0) ? REQUEST_DELETE
                  : (strcmp(method, "PATCH") == 0)  ? REQUEST_PATCH
                                                    : -1;
  request->url = strdup(url);
  request->http_version = strdup(version);

  if (!req_body_start) {
    request->content = NULL;
    request->content_length = 0;
  } else {
    const char *content_length_str =
        search_dict(request->req_data, "Content-Length");
    if (content_length_str) {
      request->content_length = strtoul(content_length_str, NULL, 10);
    } else {
      request->content_length = 0;
    }

    request->content = strndup(req_body_start, request->content_length);
  }

  return request;
}

char *find_start_of_body(char *buffer) {
  char *start;

  if ((start = strstr(buffer, "\r\n\r\n")) != NULL) {
    return start + 4;
  } else if ((start = strstr(buffer, "\n\n")) != NULL) {
    return start + 2;
  } else if ((start = strstr(buffer, "\r\r")) != NULL) {
    return start + 2;
  } else {
    return NULL;
  }
}

void free_request(Request *request) {
  if (request) {
    free(request->url);
    free(request->http_version);
    free(request->content);
    free_dict(request->req_data);
    free(request);
  }
  request = NULL;
}
