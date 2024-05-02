#include "singly_linked_list.h"
#include <stdlib.h>

struct singly_linked_list
{
    float value;
    singly_linked_list *next;
};

singly_linked_list *singly_linked_list_init()
{
    return calloc(1, sizeof(singly_linked_list));
}

float singly_linked_list_value(singly_linked_list *node)
{
    return node->value;
}

singly_linked_list *singly_linked_list_next(singly_linked_list *node)
{
    return node->next;
}

singly_linked_list *singly_linked_list_insert_after(singly_linked_list *node, float new_value)
{
    singly_linked_list *new_node = malloc(sizeof(singly_linked_list));
    new_node->next = node->next;
    new_node->value = new_value;
    node->next = new_node;
    return new_node;
}

float singly_linked_list_erase_after(singly_linked_list *node)
{
    if (node->next)
    {
        singly_linked_list *old_node = node->next;
        float old_value = old_node->value;
        node->next = old_node->next;
        free(old_node);
        return old_value;
    }
    return 0.0f;
}

void singly_linked_list_free(singly_linked_list *list)
{
    singly_linked_list *next = 0;
    while (list)
    {
        next = list->next;
        free(list);
        list = next;
    }
}
