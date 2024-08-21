#ifndef _RESPONSE_H
#define _RESPONSE_H

#include <unistd.h>

#define MAX_BUFFER_SIZE 3072 // 3KBs
#define RESPONSE_BUFFER_SIZE 4096 // 4KBs
#define CHUNK_SIZE 1024 // 1KB

typedef enum  ResponseCode {
    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NO_CONTENT = 204,
    MOVED_PERMANENTLY = 301,
    FOUND = 302,
    SEE_OTHER = 303,
    NOT_MODIFIED = 304,
    TEMPORARY_REDIRECT = 307,
    PERMANENT_REDIRECT = 308,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    REQUEST_TIMEOUT = 408,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    HTTP_VERSION_NOT_SUPPORTED = 505,
    SERVICE_UNAVAILABLE = 503,
    GATEWAY_TIMEOUT = 504,
} ResponseCode;

void send_file_response(int cli_sock, const char *file_name, const char *headers);

void send_response(int cli_sock, ResponseCode code, const char *content_type, const char *response_body, size_t response_body_length, const char *headers);

#endif // !_RESPONSE_H
