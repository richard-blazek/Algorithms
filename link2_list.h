#ifndef LINK2_LIST_H_INCLUDED
#define LINK2_LIST_H_INCLUDED

typedef struct link2_node link2_node;

struct link2_node
{
    float value;
    link2_node *prev, *next;
};

link2_node *link2_list_init();
link2_node *link2_list_insert_after(link2_node *node, float new_value);
float link2_list_erase(link2_node *node);
void link2_list_free(link2_node *sentinel);

#endif
