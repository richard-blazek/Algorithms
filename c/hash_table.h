#ifndef HASH_TABLE_H_INCLUDED
#define HASH_TABLE_H_INCLUDED

#include <stdbool.h>
#include <stdlib.h>

typedef struct hash_table hash_table;

hash_table *hash_table_init();
void hash_table_put(hash_table *h, const char *key, float value);
bool hash_table_has(hash_table *h, const char *key);
float hash_table_get(hash_table *h, const char *key);
bool hash_table_delete(hash_table *h, const char *key);
void hash_table_free(hash_table *h);

#endif