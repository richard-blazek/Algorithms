#include "xor_way.h"

#include <stdint.h>
#include <stdlib.h>

#define XOR(a,b) ((xor_way*)((uintptr_t)(a) ^ (uintptr_t)(b)))
#define REPLACE(value,old,replacement) XOR(XOR(value, old), replacement)

struct xor_way
{
    float value;
    xor_way *link;
};

xor_way *xor_way_init()
{
    return calloc(1, sizeof(xor_way));
}

float xor_way_value(xor_way *node)
{
    return node->value;
}

xor_way *xor_way_next(xor_way *other_side, xor_way *current)
{
    return XOR(other_side, current->link);
}

xor_way *xor_way_insert_after(xor_way *prev, xor_way *node, float new_value)
{
    xor_way *next = XOR(prev, node->link);
    xor_way *new = malloc(sizeof(xor_way));
    new->value = new_value;
    new->link = XOR(node, next);
    node->link = XOR(prev, new);
    if (next)
    {
        next->link = REPLACE(next->link, node, new);
    }
    return new;
}

float xor_way_erase(xor_way *prev, xor_way *node)
{
    xor_way *next = XOR(node->link, prev);
    prev->link = REPLACE(prev->link, node, next);
    next->link = REPLACE(next->link, node, prev);
    float value = node->value;
    free(node);
    return value;
}

void xor_way_free(xor_way *head)
{
    xor_way *prev = head, *current = head->link;
    while (current)
    {
        xor_way *next = XOR(prev, current->link);
        free(prev);
        prev = current;
        current = next;
    }
    free(prev);
}
