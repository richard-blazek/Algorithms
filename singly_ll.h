#ifndef SINGLY_LL_H_INCLUDED
#define SINGLY_LL_H_INCLUDED

typedef struct singly_ll singly_ll;

struct singly_ll
{
    float value;
    singly_ll *next;
};

singly_ll *singly_ll_init();
singly_ll *singly_ll_insert_after(singly_ll *node, float new_value);
float singly_ll_erase_after(singly_ll *node);
void singly_ll_free(singly_ll *list);

#endif
