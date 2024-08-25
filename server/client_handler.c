#include "client_handler.h"
#include "../request/request.h"
#include "../response/response.h"
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#define DOCUMENT_ROOT                                                          \
  "./public" // bit of a pickle here, the file path is taken from the executing
             // dir not the code containing dir, because that is how realpath()
             // works - currently it is set to start from the test dir, THIS ONLY HAPPENS IF THE RELATIVE PATH IS BEING USED

void client_handler(void *args) {

  int cli_sock = *((int *)args);
  free(args);
  char buffer[BUFFER_SIZE];
  size_t recv_bytes;

  recv_bytes = recv(cli_sock, buffer, BUFFER_SIZE - 1, 0);

  if (recv_bytes < 0) {
    perror(">>>> recv error");
    // send error responses
  } else {
    buffer[BUFFER_SIZE - 1] = '\0'; // null terminate the buffer
    Request *request = init_request(buffer, BUFFER_SIZE); // create the request
    printf(">>>> client requested - %s\n", request->url);

    char req_path[PATH_MAX];
    snprintf(req_path, PATH_MAX, "%s%s", DOCUMENT_ROOT, (strcmp(request->url, "/") == 0) ? "" : request->url);

    send_file_response(cli_sock, req_path, NULL);

    free_request(request);
  }
  close(cli_sock);
  return;
}
