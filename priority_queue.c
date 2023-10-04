#include "priority_queue.h"
#include <stdlib.h>

struct priority_queue
{
    float* values;
    int32_t* priorities;
    size_t length, capacity;
};

priority_queue* priority_queue_init()
{
    priority_queue* q = malloc(sizeof(priority_queue));
    q->capacity = 8;
    q->length = 0;
    q->priorities = malloc(sizeof(int32_t) * 8);
    q->values = malloc(sizeof(float) * 8);
    return q;
}

static void set_capacity(priority_queue* q, size_t capacity)
{
    q->capacity = capacity;
    q->priorities = realloc(q->priorities, sizeof(int32_t) * capacity);
    q->values = realloc(q->values, sizeof(float) * capacity);
}

static void increment_length(priority_queue* q)
{
    if (q->length == q->capacity)
    {
        set_capacity(q, q->capacity << 1);
    }
    ++q->length;
}

static void decrement_length(priority_queue* q)
{
    --q->length;
    if (q->length < q->capacity >> 2 && q->capacity >= 16)
    {
        set_capacity(q, q->capacity >> 1);
    }
}

#include <stdio.h>
static void bubble_up(priority_queue* q, size_t i)
{
    size_t parent = (i - 1) >> 1;
    if (i == 0 || q->priorities[parent] >= q->priorities[i])
    {
        return;
    }

    // Swap values
    float tmpf = q->values[i];
    q->values[i] = q->values[parent];
    q->values[parent] = tmpf;
    // Swap priorities
    int32_t tmpi = q->priorities[i];
    q->priorities[i] = q->priorities[parent];
    q->priorities[parent] = tmpi;

    // Recursive call
    bubble_up(q, parent);
}

static void bubble_down(priority_queue* q, size_t i)
{
    size_t left = i << 1 | 1, right = (i << 1) + 2;
    size_t child = left < q->length && right < q->length && q->priorities[right] > q->priorities[left] ? right : left;
    if (child >= q->length || q->priorities[child] <= q->priorities[i])
    {
        return;
    }

    // Swap values
    int32_t tmpi = q->priorities[i];
    q->priorities[i] = q->priorities[child];
    q->priorities[child] = tmpi;
    // Swap priorities
    float tmpf = q->values[i];
    q->values[i] = q->values[child];
    q->values[child] = tmpf;

    bubble_down(q, child);
}

void priority_queue_push(priority_queue* q, float value, int32_t priority)
{
    increment_length(q);

    q->values[q->length - 1] = value;
    q->priorities[q->length - 1] = priority;

    bubble_up(q, q->length - 1);
}

float priority_queue_pop(priority_queue* q)
{
    float result = q->values[0];
    q->values[0] = q->values[q->length - 1];
    q->priorities[0] = q->priorities[q->length - 1];

    decrement_length(q);
    bubble_down(q, 0);

    return result;
}

bool priority_queue_empty(priority_queue* q)
{
    return q->length == 0;
}

float priority_queue_top(priority_queue* q)
{
    return q->values[0];
}

int32_t priority_queue_top_priority(priority_queue* q)
{
    return q->priorities[0];
}

void priority_queue_free(priority_queue* q)
{
    free(q->priorities);
    free(q->values);
    free(q);
}
