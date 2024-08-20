#ifndef _CLIENT_HANDLER
#define _CLIENT_HANDLER

#include <sys/socket.h>
#include "../request/request.h"

typedef struct thread_args_t {
    int client_sock;
    Request *request;
} thread_args_t;

void *handler_client(void *);

#endif // !_CLIENT_HANDLER
