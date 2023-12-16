#include "singly_ll.h"
#include <stdlib.h>

singly_ll *singly_ll_init()
{
    return calloc(1, sizeof(singly_ll));
}

singly_ll *singly_ll_insert_after(singly_ll *node, float new_value)
{
    singly_ll *new_node = malloc(sizeof(singly_ll));
    new_node->next = node->next;
    new_node->value = new_value;
    node->next = new_node;
    return new_node;
}

float singly_ll_erase_after(singly_ll *node)
{
    if (node->next)
    {
        singly_ll *old_node = node->next;
        float old_value = old_node->value;
        node->next = old_node->next;
        free(old_node);
        return old_value;
    }
    return 0.0f;
}

void singly_ll_free(singly_ll *list)
{
    singly_ll *next = 0;
    while (list)
    {
        next = list->next;
        free(list);
        list = next;
    }
}
