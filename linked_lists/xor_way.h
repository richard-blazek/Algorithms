#ifndef __XOR_WAY__
#define __XOR_WAY__

typedef struct xor_way xor_way;

xor_way *xor_way_init();
float xor_way_value(xor_way *node);
xor_way *xor_way_next(xor_way *other_side, xor_way *node);
xor_way *xor_way_insert_after(xor_way *prev, xor_way *node, float new_value);
float xor_way_erase(xor_way *prev, xor_way *node);
void xor_way_free(xor_way *head);

#endif
