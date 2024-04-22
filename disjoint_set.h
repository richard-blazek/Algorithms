#ifndef DISJOINT_SET_H_INCLUDED
#define DISJOINT_SET_H_INCLUDED

#include <stdlib.h>

typedef struct disjoint_set disjoint_set;
disjoint_set *disjoint_set_init(size_t capacity);
size_t disjoint_set_find(disjoint_set *set, size_t vertex);
void disjoint_set_union(disjoint_set *set, size_t v1, size_t v2);
void disjoint_set_free(disjoint_set *set);

#endif