#ifndef PRIORITY_QUEUE_H_INCLUDED
#define PRIORITY_QUEUE_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

typedef struct priority_queue priority_queue;

priority_queue* priority_queue_init();
void priority_queue_push(priority_queue* q, float value, int32_t priority);
bool priority_queue_empty(priority_queue* q);
float priority_queue_top(priority_queue* q);
int32_t priority_queue_top_priority(priority_queue* q);
float priority_queue_pop(priority_queue* q);
void priority_queue_free(priority_queue* q);

#endif