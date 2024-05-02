#ifndef __TWO_WAY__
#define __TWO_WAY__

typedef struct doubly_linked_list doubly_linked_list;

doubly_linked_list *doubly_linked_list_init();
float doubly_linked_list_value(doubly_linked_list *node);
doubly_linked_list *doubly_linked_list_next(doubly_linked_list *node);
doubly_linked_list *doubly_linked_list_previous(doubly_linked_list *node);
doubly_linked_list *doubly_linked_list_insert_after(doubly_linked_list *node, float new_value);
float doubly_linked_list_erase(doubly_linked_list *node);
void doubly_linked_list_free(doubly_linked_list *sentinel);

#endif
