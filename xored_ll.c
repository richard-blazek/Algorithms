#include "xored_ll.h"

#include <stdint.h>
#include <stdlib.h>

#define XOR(a,b) ((xored_ll*)((uintptr_t)(a) ^ (uintptr_t)(b)))
#define REPLACE(value,old,replacement) XOR(XOR(value, old), replacement)

struct xored_ll
{
    float value;
    xored_ll *link;
};

xored_ll *xored_ll_init()
{
    return calloc(1, sizeof(xored_ll));
}

float xored_ll_value(xored_ll *node)
{
    return node->value;
}

xored_ll *xored_ll_next(xored_ll *prev, xored_ll *current)
{
    return XOR(prev, current->link);
}

xored_ll *xored_ll_insert_after(xored_ll *prev, xored_ll *node, float new_value)
{
    xored_ll *next = XOR(prev, node->link);
    xored_ll *new = malloc(sizeof(xored_ll));
    new->value = new_value;
    new->link = XOR(node, next);
    node->link = XOR(prev, new);
    if (next)
    {
        next->link = REPLACE(next->link, node, new);
    }
    return new;
}

float xored_ll_erase(xored_ll *prev, xored_ll *node)
{
    xored_ll *next = XOR(node->link, prev);
    prev->link = REPLACE(prev->link, node, next);
    next->link = REPLACE(next->link, node, prev);
    float value = node->value;
    free(node);
    return value;
}

void xored_ll_free(xored_ll *head)
{
    xored_ll *prev = head, *current = head->link;
    while (current)
    {
        xored_ll *next = XOR(prev, current->link);
        free(prev);
        prev = current;
        current = next;
    }
    free(prev);
}
