#include "deque.h"
#include <string.h>

struct deque
{
    float *data;
    size_t length, capacity, shift;
};

static size_t min(size_t a, size_t b)
{
    return a < b ? a : b;
}

deque *deque_init(size_t capacity)
{
    deque *dq = malloc(sizeof(deque));
    dq->data = malloc(sizeof(float) * capacity);
    dq->capacity = capacity;
    dq->length = dq->shift = 0;
    return dq;
}

void deque_free(deque *dq)
{
    free(dq->data);
    free(dq);
}

size_t deque_length(deque *dq)
{
    return dq->length;
}

size_t deque_capacity(deque *dq)
{
    return dq->capacity;
}

float deque_get(deque *dq, size_t i)
{
    return dq->data[(i + dq->shift) % dq->capacity];
}

void deque_set(deque *dq, size_t i, float value)
{
    dq->data[(i + dq->shift) % dq->capacity] = value;
}

static void deque_realloc(deque *dq, size_t length, size_t capacity)
{
    float *new_data = malloc(capacity * sizeof(float));

    size_t total_copied = min(length, dq->length);
    size_t before_wrap = min(total_copied, dq->capacity - dq->shift);

    memcpy(new_data, dq->data + dq->shift, before_wrap * sizeof(float));
    memcpy(new_data + before_wrap, dq->data, (total_copied - before_wrap) * sizeof(float));

    free(dq->data);
    dq->data = new_data;
    dq->capacity = capacity;
    dq->length = length;
    dq->shift = 0;
}

void deque_reserve(deque *dq, size_t capacity)
{
    if (capacity > dq->capacity)
    {
        if (capacity <= dq->capacity << 1)
        {
            deque_realloc(dq, dq->length, dq->capacity << 1);
        }
        else
        {
            deque_realloc(dq, dq->length, capacity);
        }
    }
}

void deque_resize(deque *dq, size_t length)
{
    if (length <= dq->capacity)
    {
        dq->length = length;
    }
    else
    {
        deque_reserve(dq, length);
        dq->length = length;
    }
}

void deque_shrink(deque *dq)
{
    if (dq->length != dq->capacity)
    {
        deque_realloc(dq, dq->length, dq->length);
    }
}

void deque_push_back(deque *dq, float value)
{
    deque_resize(dq, dq->length + 1);
    deque_set(dq, dq->length - 1, value);
}

void deque_push_front(deque *dq, float value)
{
    deque_resize(dq, dq->length + 1);
    deque_set(dq, dq->capacity - 1, value);
    dq->shift = (dq->shift - 1 + dq->capacity) % dq->capacity;
}

float deque_pop_back(deque *dq)
{
    float value = deque_get(dq, dq->length - 1);
    --dq->length;
    return value;
}

float deque_pop_front(deque *dq)
{
    float value = deque_get(dq, 0);
    dq->shift = (dq->shift + 1) % dq->capacity;
    --dq->length;
    return value;
}
