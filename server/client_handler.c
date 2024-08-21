#include "client_handler.h"
#include "../request/request.h"
#include <bits/types/struct_iovec.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void client_handler(void *args) {

  printf(">>>> from client handler: %d\n", *(int *)args);

  int cli_sock = *((int *)args);
  free(args);
  char buffer[BUFFER_SIZE];
  size_t recv_bytes;

  // TODO: HANDLE PARTIAL READS
  recv_bytes = recv(cli_sock, buffer, BUFFER_SIZE - 1, 0);

  if (recv_bytes < 0) {
    perror(">>>> recv error");
    // send error responses
  } else {
    buffer[BUFFER_SIZE - 1] = '\0'; // null terminate the buffer
    Request *request = init_request(buffer, BUFFER_SIZE); // create the request
    printf(">>>> client requested - %s", request->url);

    const char *response = "HTTP/1.1 200 OK\r\n"
                           "Content-Type: text/html\r\n"
                           "Content-Length: 13\r\n"
                           "\r\n"
                           "Hello, world!";
    if (send(cli_sock, response, strlen(response), 0) == -1)
      perror(">>>> send error");

    free_request(request);
  }
  close(cli_sock);
  return;
}
