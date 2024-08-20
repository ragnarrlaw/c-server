#include <stdio.h>
#include <stdlib.h>

#include "../queue/queue.h"

void repr_data(void *);

typedef struct Data {
  int data;
} Data;

int main(int argc, char *argv[]) {
  Queue *q = new_queue(4);
  iter(q, repr_data);

  for (int i = 0; i < 4; i++) {
    Data *d = (Data *)malloc(sizeof(Data));
    d->data = i;
    enqueue(q, d);
  }

  iter(q, repr_data);
  printf(">>>> Front: %d\n", q->front);
  printf(">>>> Rear: %d\n", q->rear);

  free(dequeue(q));
  printf(">>>> Front: %d\n", q->front);
  printf(">>>> Rear: %d\n", q->rear);
  iter(q, repr_data);

  free(dequeue(q));
  printf(">>>> Front: %d\n", q->front);
  printf(">>>> Rear: %d\n", q->rear);
  iter(q, repr_data);

  Data *d = (Data *)malloc(sizeof(Data));
  d->data = 10;

  enqueue(q, d);
  printf(">>>> Front: %d\n", q->front);
  printf(">>>> Rear: %d\n", q->rear);
  iter(q, repr_data);

  d = (Data *)malloc(sizeof(Data));
  d->data = 11;

  enqueue(q, d);
  printf(">>>> Front: %d\n", q->front);
  printf(">>>> Rear: %d\n", q->rear);
  iter(q, repr_data);

  d = (Data *)malloc(sizeof(Data));
  d->data = 12;

  enqueue(q, d);
  printf(">>>> Front: %d\n", q->front);
  printf(">>>> Rear: %d\n", q->rear);
  iter(q, repr_data);

  while (!is_empty(q))
    free(dequeue(q));
  printf(">>>> Front: %d\n", q->front);
  printf(">>>> Rear: %d\n", q->rear);
  iter(q, repr_data);

  free_queue(q);
  return EXIT_SUCCESS;
}

void repr_data(void *data) {
  if (data)
    printf("Data {data: %d}, ", (*(Data *)data).data);
  else
    printf("NULL, ");
}
