#ifndef DEQUE_H_INCLUDED
#define DEQUE_H_INCLUDED

#include <stdlib.h>

typedef struct deque deque;

deque* deque_init(size_t capacity);
void deque_free(deque* dq);

size_t deque_length(deque* dq);
size_t deque_capacity(deque* dq);

float deque_get(deque* dq, size_t i);
void deque_set(deque* dq, size_t i, float value);

void deque_resize(deque* dq, size_t length);
void deque_reserve(deque* dq, size_t capacity);
void deque_shrink(deque* dq);

void deque_push_back(deque* dq, float value);
void deque_push_front(deque* dq, float value);
float deque_pop_back(deque* dq);
float deque_pop_front(deque* dq);

#endif