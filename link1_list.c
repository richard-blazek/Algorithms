#include "link1_list.h"
#include <stdlib.h>

link1_node* link1_list_init()
{
    return calloc(1, sizeof(link1_node));
}

link1_node* link1_list_insert_after(link1_node* node, float new_value)
{
    link1_node* new_node = malloc(sizeof(link1_node));
    new_node->next = node->next;
    new_node->value = new_value;
    node->next = new_node;
    return new_node;
}

float link1_list_erase_after(link1_node* node)
{
    if (node->next)
    {
        link1_node* old_node = node->next;
        float old_value = old_node->value;
        node->next = old_node->next;
        free(old_node);
        return old_value;
    }
}

void link1_list_free(link1_node* list)
{
    link1_node* next = 0;
    while (list)
    {
        next = list->next;
        free(list);
        list = next;
    }
}
