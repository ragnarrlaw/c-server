#ifndef _REQUEST_H_
#define _REQUEST_H_

/**
 * WARNING: THE FOLLOWING REQUEST CLASS DOENS'T HANDLE THE CHUNKED
 * TRANSFER CASES WHEN THE "Transfer-encoding: chunked" HEADER is
 * PRESENT
 * */

#include "../kv_store/dictionary.h"

typedef enum RequestType {
    REQUEST_GET,    // GET request
    REQUEST_POST,   // POST request
    REQUEST_DELETE, // DELETE request
    REQUEST_PATCH  // PATCH request
} RequestType;

typedef struct Request {
    RequestType type;
    char *url;
    char *http_version;
    char *content;
    unsigned int content_length;
    Dictionary *req_data;
} Request;

Request* init_request(char* buffer, unsigned int length);

void free_request(Request *request);

#endif // !_REQUEST_H_
