#include "xorptr_linked_list.h"

#include <stdint.h>
#include <stdlib.h>

#define XOR(a,b) ((xorptr_linked_list*)((uintptr_t)(a) ^ (uintptr_t)(b)))
#define REPLACE(value,old,replacement) XOR(XOR(value, old), replacement)

struct xorptr_linked_list
{
    float value;
    xorptr_linked_list *link;
};

xorptr_linked_list *xorptr_linked_list_init()
{
    return calloc(1, sizeof(xorptr_linked_list));
}

float xorptr_linked_list_value(xorptr_linked_list *node)
{
    return node->value;
}

xorptr_linked_list *xorptr_linked_list_next(xorptr_linked_list *other_side, xorptr_linked_list *current)
{
    return XOR(other_side, current->link);
}

xorptr_linked_list *xorptr_linked_list_insert_after(xorptr_linked_list *prev, xorptr_linked_list *node, float new_value)
{
    xorptr_linked_list *next = XOR(prev, node->link);
    xorptr_linked_list *new = malloc(sizeof(xorptr_linked_list));
    new->value = new_value;
    new->link = XOR(node, next);
    node->link = XOR(prev, new);
    if (next)
    {
        next->link = REPLACE(next->link, node, new);
    }
    return new;
}

float xorptr_linked_list_erase(xorptr_linked_list *prev, xorptr_linked_list *node)
{
    xorptr_linked_list *next = XOR(node->link, prev);
    prev->link = REPLACE(prev->link, node, next);
    next->link = REPLACE(next->link, node, prev);
    float value = node->value;
    free(node);
    return value;
}

void xorptr_linked_list_free(xorptr_linked_list *head)
{
    xorptr_linked_list *prev = head, *current = head->link;
    while (current)
    {
        xorptr_linked_list *next = XOR(prev, current->link);
        free(prev);
        prev = current;
        current = next;
    }
    free(prev);
}
