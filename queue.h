#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include <stdbool.h>

typedef struct queue queue;

queue *queue_init();
void queue_push(queue *q, float value);
bool queue_empty(queue *q);
float queue_top(queue *q);
float queue_pop(queue *q);
void queue_free(queue *q);

#endif