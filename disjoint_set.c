#include "disjoint_set.h"

struct disjoint_set
{
    size_t *parents;
    size_t *ranks;
};

disjoint_set *disjoint_set_init(size_t capacity)
{
    disjoint_set *uf = malloc(sizeof(disjoint_set));
    uf->parents = malloc(sizeof(size_t) * capacity);
    uf->ranks = malloc(sizeof(size_t) * capacity);
    for (size_t i = 0; i < capacity; ++i)
    {
        uf->parents[i] = i;
        uf->ranks[i] = 1;
    }
    return uf;
}

size_t disjoint_set_find(disjoint_set *set, size_t vertex)
{
    if (set->parents[vertex] != vertex)
    {
        set->parents[vertex] = disjoint_set_find(set, set->parents[vertex]);
    }
    return set->parents[vertex];
}

void disjoint_set_union(disjoint_set *set, size_t v1, size_t v2)
{
    size_t c1 = disjoint_set_find(set, v1);
    size_t c2 = disjoint_set_find(set, v2);
    if (c1 != c2)
    {
        if (set->ranks[c1] < set->ranks[c2])
        {
            set->ranks[c2] += set->ranks[c1];
            set->parents[c1] = c2;
        }
        else
        {
            set->ranks[c1] += set->ranks[c2];
            set->parents[c2] = c1;
        }
    }
}

void disjoint_set_free(disjoint_set *uf)
{
    free(uf->parents);
    free(uf->ranks);
    free(uf);
}
