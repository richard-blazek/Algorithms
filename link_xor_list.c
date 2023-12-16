#include "link_xor_list.h"

#include <stdint.h>
#include <stdlib.h>

#define XOR(a,b) ((link_xor_node*)((uintptr_t)(a) ^ (uintptr_t)(b)))
#define REPLACE(value,old,replacement) XOR(XOR(value, old), replacement)

struct link_xor_node
{
    float value;
    link_xor_node *link;
};

link_xor_node *link_xor_list_init()
{
    return calloc(1, sizeof(link_xor_node));
}

float link_xor_list_value(link_xor_node *node)
{
    return node->value;
}

link_xor_node *link_xor_list_next(link_xor_node *prev, link_xor_node *current)
{
    return XOR(prev, current->link);
}

link_xor_node *link_xor_list_insert_after(link_xor_node *prev, link_xor_node *node, float new_value)
{
    link_xor_node *next = XOR(prev, node->link);
    link_xor_node *new = malloc(sizeof(link_xor_node));
    new->value = new_value;
    new->link = XOR(node, next);
    node->link = XOR(prev, new);
    if (next)
    {
        next->link = REPLACE(next->link, node, new);
    }
    return new;
}

float link_xor_list_erase(link_xor_node *prev, link_xor_node *node)
{
    link_xor_node *next = XOR(node->link, prev);
    prev->link = REPLACE(prev->link, node, next);
    next->link = REPLACE(next->link, node, prev);
    float value = node->value;
    free(node);
    return value;
}

void link_xor_list_free(link_xor_node *head)
{
    link_xor_node *prev = head, *current = head->link;
    while (current)
    {
        link_xor_node *next = XOR(prev, current->link);
        free(prev);
        prev = current;
        current = next;
    }
    free(prev);
}
