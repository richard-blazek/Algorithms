#include "doubly_ll.h"
#include <stdlib.h>

doubly_ll *doubly_ll_init()
{
    doubly_ll *sentinel = malloc(sizeof(doubly_ll));
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    return sentinel;
}

doubly_ll *doubly_ll_insert_after(doubly_ll *node, float new_value)
{
    doubly_ll *new_node = malloc(sizeof(doubly_ll));
    new_node->next = node->next;
    node->next->prev = new_node;
    new_node->prev = node;
    node->next = new_node;
    new_node->value = new_value;
    return new_node;
}

float doubly_ll_erase(doubly_ll *node)
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

void doubly_ll_free(doubly_ll *sentinel)
{
    doubly_ll *node = sentinel, *next;
    do
    {
        next = node->next;
        free(node);
        node = next;
    } while (node != sentinel);
}
