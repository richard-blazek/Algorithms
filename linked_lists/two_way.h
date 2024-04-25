#ifndef __TWO_WAY__
#define __TWO_WAY__

typedef struct two_way two_way;

two_way *two_way_init();
float two_way_value(two_way *node);
two_way *two_way_next(two_way *node);
two_way *two_way_previous(two_way *node);
two_way *two_way_insert_after(two_way *node, float new_value);
float two_way_erase(two_way *node);
void two_way_free(two_way *sentinel);

#endif
