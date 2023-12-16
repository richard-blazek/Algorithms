#ifndef LINK_XOR_LIST_INCLUDED
#define LINK_XOR_LIST_INCLUDED

typedef struct link_xor_node link_xor_node;

link_xor_node *link_xor_list_init();
float link_xor_list_value(link_xor_node *node);
link_xor_node *link_xor_list_next(link_xor_node *prev, link_xor_node *node);
link_xor_node *link_xor_list_insert_after(link_xor_node *prev, link_xor_node *node, float new_value);
float link_xor_list_erase(link_xor_node *prev, link_xor_node *node);
void link_xor_list_free(link_xor_node *head);

#endif
