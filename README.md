# Multi-threaded Web Server


This project is a basic multi-threaded web server designed to serve static files. It is implemented in C and uses POSIX threads for handling multiple client requests concurrently.

## Project Structure

```
.
├── bin/ # Compiled binaries
├── cmd/
│ └── server/
│ └── main.c # Entry point for the server
├── server/
│ ├── server.h
│ ├── server.c
│ ├── client_handler.h
│ └── client_handler.c
├── queue/
│ ├── queue.h
│ └── queue.c
├── kv_store/
│ ├── dictionary.h
│ └── dictionary.c
├── request/
│ ├── request.h
│ └── request.c
├── response/
│ ├── response.h
│ └── response.c
├── mime/
│ ├── mime.h
│ └── mime.c
├── file_ops/
│ ├── file_ops.h
│ └── file_ops.c
├── test/
│ ├── file_ops_test.c
│ ├── dictionary_test.c
│ ├── request_test.c
│ ├── queue_test.c
│ ├── server_test.c
│ └── real_path_test.c
└── Makefile # Makefile for building and testing
```

## Dependencies

-   GCC (GNU Compiler Collection)
-   POSIX Threads (pthread)

## Build and Run

### Building the Server

To compile the server, run the following command:

```bash
$ make run_server
```

## Known Issues

-   Concurrency Issues: There may be race conditions and other concurrency-related bugs due to the multi-threaded design.
-   Memory Management: Potential memory leaks or improper memory management practices.
-   Limited HTTP Support: The server only supports basic HTTP requests and responses.
-   Error Handling: Error handling is minimal and may not cover all edge cases.
-   Security: This server is not secure and should not be used in a production environment.

## Future Improvements

-   Robust Error Handling: Improve error handling to cover more scenarios.
-   Enhanced HTTP Support: Add support for more HTTP methods and headers.
-   Security Enhancements: Implement security features like input validation and secure memory handling.
-   Performance Optimization: Optimize thread management and reduce overhead.
-   Logging: Implement logging for better debugging and monitoring.
