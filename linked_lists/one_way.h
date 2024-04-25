#ifndef __ONE_WAY__
#define __ONE_WAY__

typedef struct one_way one_way;

one_way *one_way_init();
float one_way_value(one_way *node);
one_way *one_way_next(one_way *node);
one_way *one_way_insert_after(one_way *node, float new_value);
float one_way_erase_after(one_way *node);
void one_way_free(one_way *list);

#endif
