#include "../kv_store/dictionary.h"
#include "../request/request.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  char dummy_request_without_body[1024] =
      "GET /index.html HTTP/1.1\r\n"
      "Host: www.example.com\r\n"
      "Connection: keep-alive\r\n"
      "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
      "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.82 "
      "Safari/537.36\r\n"
      "Accept: "
      "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/"
      "apng,*/*;q=0.8\r\n"
      "Accept-Language: en-US,en;q=0.9\r\n"
      "Accept-Encoding: gzip, deflate, br\r\n"
      "\r\n";

  Request *req = init_request(dummy_request_without_body, 1024);
  printf("Request url: %s\n", req->url);
  printf("Request type: %u\n", req->type);
  printf("Request content: %s\n", req->content);
  printf("Request http-version: %s\n", req->http_version);
  printf("Request content-length: %u\n", req->content_length);
  repr(req->req_data);
  free_request(req);

  char dummy_request_with_body[1024] =
      "POST /submit-form HTTP/1.1\r\n"
      "Host: www.example.com\r\n"
      "Connection: keep-alive\r\n"
      "Content-Type: application/x-www-form-urlencoded\r\n"
      "Content-Length: 32\r\n"
      "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
      "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.82 "
      "Safari/537.36\r\n"
      "Accept: */*\r\n"
      "Accept-Language: en-US,en;q=0.9\r\n"
      "Accept-Encoding: gzip, deflate, br\r\n"
      "\r\n"
      "name=John+Doe&age=30&country=USA";

  req = init_request(dummy_request_with_body, 1024);

  printf("Request url: %s\n", req->url);
  printf("Request type: %u\n", req->type);
  printf("Request content: %s\n", req->content);
  printf("Request http-version: %s\n", req->http_version);
  printf("Request content-length: %u\n", req->content_length);

  repr(req->req_data);
  free_request(req);

  return EXIT_SUCCESS;
}
