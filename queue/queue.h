
#ifndef _QUEUE_H
#define _QUEUE_H

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>

typedef struct Queue {
    void **data;
    int front;
    int rear;
    int buf_len;
} Queue;


Queue *new_queue(size_t);
void free_queue(Queue*);
bool enqueue(Queue*, void*);
void *dequeue(Queue*);
bool is_full(Queue *);
bool is_empty(Queue *);
void iter(Queue *, void (*fn)(void *));

#endif //!_QUEUE_H
