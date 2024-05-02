#ifndef __XOR_WAY__
#define __XOR_WAY__

typedef struct xorptr_linked_list xorptr_linked_list;

xorptr_linked_list *xorptr_linked_list_init();
float xorptr_linked_list_value(xorptr_linked_list *node);
xorptr_linked_list *xorptr_linked_list_next(xorptr_linked_list *other_side, xorptr_linked_list *node);
xorptr_linked_list *xorptr_linked_list_insert_after(xorptr_linked_list *prev, xorptr_linked_list *node, float new_value);
float xorptr_linked_list_erase(xorptr_linked_list *prev, xorptr_linked_list *node);
void xorptr_linked_list_free(xorptr_linked_list *head);

#endif
