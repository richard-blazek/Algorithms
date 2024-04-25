#include "one_way.h"
#include <stdlib.h>

struct one_way
{
    float value;
    one_way *next;
};

one_way *one_way_init()
{
    return calloc(1, sizeof(one_way));
}

float one_way_value(one_way *node)
{
    return node->value;
}

one_way *one_way_next(one_way *node)
{
    return node->next;
}

one_way *one_way_insert_after(one_way *node, float new_value)
{
    one_way *new_node = malloc(sizeof(one_way));
    new_node->next = node->next;
    new_node->value = new_value;
    node->next = new_node;
    return new_node;
}

float one_way_erase_after(one_way *node)
{
    if (node->next)
    {
        one_way *old_node = node->next;
        float old_value = old_node->value;
        node->next = old_node->next;
        free(old_node);
        return old_value;
    }
    return 0.0f;
}

void one_way_free(one_way *list)
{
    one_way *next = 0;
    while (list)
    {
        next = list->next;
        free(list);
        list = next;
    }
}
