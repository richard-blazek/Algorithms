#ifndef DOUBLY_LL_H_INCLUDED
#define DOUBLY_LL_H_INCLUDED

typedef struct doubly_ll doubly_ll;

doubly_ll *doubly_ll_init();
float doubly_ll_value(doubly_ll *node);
doubly_ll *doubly_ll_next(doubly_ll *node);
doubly_ll *doubly_ll_previous(doubly_ll *node);
doubly_ll *doubly_ll_insert_after(doubly_ll *node, float new_value);
float doubly_ll_erase(doubly_ll *node);
void doubly_ll_free(doubly_ll *sentinel);

#endif
