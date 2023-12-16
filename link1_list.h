#ifndef LINK1_LIST_H_INCLUDED
#define LINK1_LIST_H_INCLUDED

typedef struct link1_node link1_node;

struct link1_node
{
    float value;
    link1_node *next;
};

link1_node *link1_list_init();
link1_node *link1_list_insert_after(link1_node *node, float new_value);
float link1_list_erase_after(link1_node *node);
void link1_list_free(link1_node *list);

#endif
