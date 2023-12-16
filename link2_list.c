#include "link2_list.h"
#include <stdlib.h>

link2_node *link2_list_init()
{
    link2_node *sentinel = malloc(sizeof(link2_node));
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    return sentinel;
}

link2_node *link2_list_insert_after(link2_node *node, float new_value)
{
    link2_node *new_node = malloc(sizeof(link2_node));
    new_node->next = node->next;
    node->next->prev = new_node;
    new_node->prev = node;
    node->next = new_node;
    new_node->value = new_value;
    return new_node;
}

float link2_list_erase(link2_node *node)
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

void link2_list_free(link2_node *sentinel)
{
    link2_node *node = sentinel, *next;
    do
    {
        next = node->next;
        free(node);
        node = next;
    } while (node != sentinel);
}
