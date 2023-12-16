#ifndef DOUBLY_LL_H_INCLUDED
#define DOUBLY_LL_H_INCLUDED

typedef struct doubly_ll doubly_ll;

struct doubly_ll
{
    float value;
    doubly_ll *prev, *next;
};

doubly_ll *doubly_ll_init();
doubly_ll *doubly_ll_insert_after(doubly_ll *node, float new_value);
float doubly_ll_erase(doubly_ll *node);
void doubly_ll_free(doubly_ll *sentinel);

#endif
