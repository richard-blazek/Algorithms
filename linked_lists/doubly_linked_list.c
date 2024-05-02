#include "doubly_linked_list.h"
#include <stdlib.h>

struct doubly_linked_list
{
    float value;
    doubly_linked_list *prev, *next;
};

doubly_linked_list *doubly_linked_list_init()
{
    doubly_linked_list *sentinel = malloc(sizeof(doubly_linked_list));
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    return sentinel;
}

float doubly_linked_list_value(doubly_linked_list *node)
{
    return node->value;
}

doubly_linked_list *doubly_linked_list_next(doubly_linked_list *node)
{
    return node->next;
}

doubly_linked_list *doubly_linked_list_previous(doubly_linked_list *node)
{
    return node->prev;
}

doubly_linked_list *doubly_linked_list_insert_after(doubly_linked_list *node, float new_value)
{
    doubly_linked_list *new_node = malloc(sizeof(doubly_linked_list));
    new_node->next = node->next;
    node->next->prev = new_node;
    new_node->prev = node;
    node->next = new_node;
    new_node->value = new_value;
    return new_node;
}

float doubly_linked_list_erase(doubly_linked_list *node)
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

void doubly_linked_list_free(doubly_linked_list *sentinel)
{
    doubly_linked_list *node = sentinel, *next;
    do
    {
        next = node->next;
        free(node);
        node = next;
    } while (node != sentinel);
}
