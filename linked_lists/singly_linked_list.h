#ifndef __ONE_WAY__
#define __ONE_WAY__

typedef struct singly_linked_list singly_linked_list;

singly_linked_list *singly_linked_list_init();
float singly_linked_list_value(singly_linked_list *node);
singly_linked_list *singly_linked_list_next(singly_linked_list *node);
singly_linked_list *singly_linked_list_insert_after(singly_linked_list *node, float new_value);
float singly_linked_list_erase_after(singly_linked_list *node);
void singly_linked_list_free(singly_linked_list *list);

#endif
