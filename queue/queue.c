#include "queue.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

Queue *new_queue(size_t buf_len) {
  Queue *queue = (Queue *)malloc(sizeof(Queue));
  if (queue == NULL) {
    return NULL;
  } else {
    queue->data = (void **)malloc(sizeof(void *) * buf_len);
    if (queue->data == NULL) {
      free(queue);
      return NULL;
    }
    queue->front = 0;
    queue->rear = 0;
    queue->buf_len = buf_len;
    return queue;
  }
}

void free_queue(Queue *ptr_queue) {
  if (ptr_queue != NULL) {
    free(ptr_queue->data);
    free(ptr_queue);
  }
}

bool enqueue(Queue *ptr_queue, void *data) {
  if (!is_full(ptr_queue)) {
    ptr_queue->data[ptr_queue->rear] = data;
    ptr_queue->rear = (ptr_queue->rear + 1) % ptr_queue->buf_len;
    return true;
  }
  return false;
}

void *dequeue(Queue *ptr_queue) {
  if (!is_empty(ptr_queue)) {
    void *data = ptr_queue->data[ptr_queue->front];
    ptr_queue->front = (ptr_queue->front + 1) % ptr_queue->buf_len;
    return data;
  }
  return NULL;
}

bool is_full(Queue *ptr_queue) {
  return (ptr_queue->rear + 1) % ptr_queue->buf_len == ptr_queue->front;
}

bool is_empty(Queue *ptr_queue) { return ptr_queue->front == ptr_queue->rear; }

void iter(Queue *queue, void (*fn)(void *)) {
  if (is_empty(queue)) {
    printf(">>>> Queue is empty\n");
  } else {
    printf("Queue: {");
    for (size_t i = queue->front; i != queue->rear;
         i = (i + 1) % queue->buf_len) {
      fn(queue->data[i]);
    }
    printf(" }\n");
  }
}
