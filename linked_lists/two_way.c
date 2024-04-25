#include "two_way.h"
#include <stdlib.h>

struct two_way
{
    float value;
    two_way *prev, *next;
};

two_way *two_way_init()
{
    two_way *sentinel = malloc(sizeof(two_way));
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    return sentinel;
}

float two_way_value(two_way *node)
{
    return node->value;
}

two_way *two_way_next(two_way *node)
{
    return node->next;
}

two_way *two_way_previous(two_way *node)
{
    return node->prev;
}

two_way *two_way_insert_after(two_way *node, float new_value)
{
    two_way *new_node = malloc(sizeof(two_way));
    new_node->next = node->next;
    node->next->prev = new_node;
    new_node->prev = node;
    node->next = new_node;
    new_node->value = new_value;
    return new_node;
}

float two_way_erase(two_way *node)
{
    if (node->next != node)
    {
        node->next->prev = node->prev;
        node->prev->next = node->next;
        float old_value = node->value;
        free(node);
        return old_value;
    }
    return 0.0f;
}

void two_way_free(two_way *sentinel)
{
    two_way *node = sentinel, *next;
    do
    {
        next = node->next;
        free(node);
        node = next;
    } while (node != sentinel);
}
