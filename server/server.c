#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../queue/queue.h"
#include "client_handler.h"
#include "server.h"

#define PORT "8080"
#define BACKLOG 10
#define NUM_THREADS 20

typedef struct Work {
  Queue *q;
  pthread_mutex_t *mutex;
  pthread_cond_t *cond_var;
} Work;

void *get_in_addr(struct sockaddr *);
pthread_t **init_thread_pool(size_t, Work *);
void *thread_work(void *);

void repr_sock(void *data) {
  if (data)
    printf("%d, ", (*(int *)data));
}

void run_server() {

  int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr; // connector's address information
  socklen_t sin_size;
  int yes = 1;
  char s[INET6_ADDRSTRLEN];
  int rv;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use my IP

  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    exit(EXIT_FAILURE);
  }

  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      perror("setsockopt");
      exit(1);
    }

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("server: bind");
      continue;
    }

    break;
  }

  freeaddrinfo(servinfo); // all done with this structure

  if (p == NULL) {
    fprintf(stderr, "server: failed to bind\n");
    exit(EXIT_FAILURE);
  }

  if (listen(sockfd, BACKLOG) == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  Queue *q = new_queue(10);
  if (q == NULL) {
    perror(">>>> work buffer failed; closing socket...");
    close(sockfd);
    exit(EXIT_FAILURE);
  }
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;

  Work w = {q, &mutex, &cond_var};

  pthread_t **t = init_thread_pool(2, &w);
  if (t == NULL) {
    close(sockfd);
    free_queue(q);
    perror(">>>> thread pool failure; closing socket...");
    exit(EXIT_FAILURE);
  }

  printf(">>>> server: listening...\n");

  while (1) {
    sin_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if (new_fd == -1) {
      perror(">>>> server: accept connection failed");
      continue;
    }

    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr),
              s, sizeof s);
    printf(">>>> server: client connection from %s\n", s);

    pthread_mutex_lock(&mutex);
    int *sock_m = (int *)malloc(sizeof(int));
    *sock_m = new_fd;
    if (enqueue(q, sock_m)) {
      pthread_cond_signal(&cond_var);
    }
    pthread_mutex_unlock(&mutex);
  }
}

// get sockaddr, IPv4 or IPv6
void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

pthread_t **init_thread_pool(size_t num_threads, Work *w) {
  pthread_t **threads = (pthread_t **)malloc(sizeof(pthread_t *) * num_threads);
  if (threads == NULL) {
    perror(">>>> thread pool creation error");
    return NULL;
  } else {
    for (int i = 0; i < num_threads; i++) {
      threads[i] = (pthread_t *)malloc(sizeof(pthread_t));
      if (threads[i] == NULL) {
        perror(">>>> thread creation error");
        for (int j = 0; j < i; j++) {
          free(threads[j]);
        }
        free(threads);
        return NULL;
      }
      if (pthread_create(threads[i], NULL, thread_work, (void *)w) != 0) {
        perror(">>>> pthread_create error");
        for (int j = 0; j < i; j++) {
          free(threads[j]);
        }
        free(threads[i]);
        free(threads);
        return NULL;
      }
    }
    return threads;
  }
}

void *thread_work(void *args) {
  Work *w = (Work *)args;
  while (true) {
    pthread_mutex_lock(w->mutex);
    if (!is_empty(w->q)) {
      client_handler(dequeue(w->q));
    } else {
      pthread_cond_wait(w->cond_var, w->mutex);
    }
    pthread_mutex_unlock(w->mutex);
  }
  free(w);
  return NULL;
}
