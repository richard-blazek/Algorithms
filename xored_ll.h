#ifndef XORED_LL_INCLUDED
#define XORED_LL_INCLUDED

typedef struct xored_ll xored_ll;

xored_ll *xored_ll_init();
float xored_ll_value(xored_ll *node);
xored_ll *xored_ll_next(xored_ll *other_side, xored_ll *node);
xored_ll *xored_ll_insert_after(xored_ll *prev, xored_ll *node, float new_value);
float xored_ll_erase(xored_ll *prev, xored_ll *node);
void xored_ll_free(xored_ll *head);

#endif
