#ifndef LINK2_LIST_H_INCLUDED
#define LINK2_LIST_H_INCLUDED

typedef struct link2_node
{
    float value;
    struct link2_node* prev;
    struct link2_node* next;
} link2_node;

link2_node* link2_list_init();
link2_node* link2_list_insert_after(link2_node* node, float new_value);
link2_node* link2_list_insert_before(link2_node* node, float new_value);
float link2_list_erase(link2_node* node);
float link2_list_erase_after(link2_node* node);
float link2_list_erase_before(link2_node* node);
void link2_list_free(link2_node* sentinel);

#endif
