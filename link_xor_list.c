#include "link_xor_list.h"
#include <stdlib.h>

#define XOR(a,b) ((size_t)(a) ^ (size_t)(b))

struct link_xor_node
{
    float value;
    size_t link;
};

link_xor_node *link_xor_list_init()
{
    return calloc(1, sizeof(link_xor_node));
}

link_xor_node *link_xor_list_next(link_xor_node *prev, link_xor_node *current)
{
    return XOR(prev, current->link);
}

link_xor_node *link_xor_list_insert_after(link_xor_node *prev, link_xor_node *node, float new_value)
{
    link_xor_node *next = XOR(prev, node->link);
    link_xor_node *next_next = XOR(node, next->link);
    link_xor_node *new = malloc(sizeof(link_xor_node));
    new->value = new_value;
    node->link = XOR(prev, new);
    new->link = XOR(node, next);
    next->link = XOR(new, next_next);
    return new;
}

float link_xor_list_erase(link_xor_node *prev, link_xor_node *node)
{
    link_xor_node *next = XOR(node->link, prev);
    prev->link = XOR(XOR(prev->link, node), next);
    next->link = XOR(XOR(next->link, node), prev);
    
    float value = node->value;
    free(node);
    return value;
}

void link_xor_list_free(link_xor_node *head)
{
    link_xor_node *prev = head, *currrent = head->link;
    while (currrent)
    {
        link_xor_node *next = XOR(prev, currrent->link);
        free(prev);
        prev = currrent;
        currrent = next;
    }
    free(prev);
}
