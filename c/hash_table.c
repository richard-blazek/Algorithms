#include "hash_table.h"
#include <string.h>

typedef struct element
{
    const char *key;
    float value;
} element;

struct hash_table
{
    element *data;
    size_t size, capacity;
};

static size_t hash(const char *str, size_t range)
{
    size_t hash = 5381;
    for (char c = *str; c; ++str, c = *str)
    {
        hash = ((hash << 5) + hash) ^ c;
    }
    return hash % range;
}

static hash_table *hash_table_make(size_t capacity)
{
    hash_table *h = malloc(sizeof(hash_table));
    h->size = 0;
    h->capacity = capacity;
    h->data = calloc(capacity, sizeof(element));
    return h;
}

static void hash_table_realloc(hash_table *h, size_t capacity)
{
    hash_table *tmp = hash_table_make(capacity);
    for (size_t i = 0; i < h->capacity; ++i)
    {
        if (h->data[i].key != 0)
        {
            hash_table_put(tmp, h->data[i].key, h->data[i].value);
        }
    }
    free(h->data);
    h->data = tmp->data;
    h->capacity = capacity;
    free(tmp);
}

static size_t hash_table_lookup(hash_table *h, const char *key)
{
    size_t i = hash(key, h->capacity);
    while (h->data[i].key != 0 && strcmp(h->data[i].key, key) != 0)
    {
        i = (i + 1) % h->capacity;
    }
    return i;
}

hash_table *hash_table_init()
{
    return hash_table_make(8);
}

void hash_table_put(hash_table *h, const char *key, float value)
{
    if (h->size > h->capacity / 2)
    {
        hash_table_realloc(h, h->capacity * 2);
    }
    size_t i = hash_table_lookup(h, key);
    if (h->data[i].key == 0)
    {
        h->data[i].key = key;
        ++h->size;
    }
    h->data[i].value = value;
}

bool hash_table_has(hash_table *h, const char *key)
{
    return h->data[hash_table_lookup(h, key)].key != 0;
}

float hash_table_get(hash_table *h, const char *key)
{
    size_t i = hash_table_lookup(h, key);
    return h->data[i].key != 0 ? h->data[i].value : 0;
}

static element *remove_block(hash_table *h, size_t begin, size_t *count)
{
    size_t end = begin;
    while (h->data[end % h->capacity].key != 0)
    {
        ++end;
    }
    if (end == begin)
    {
        *count = 0;
        return 0;
    }
    if (end == begin + 1)
    {
        h->data[begin].key = 0;
        --h->size;
        *count = 0;
        return 0;
    }
    *count = end - begin - 1;
    size_t prewrap = begin + *count < h->capacity ? *count : h->capacity - begin;
    size_t wrapped = *count - prewrap;

    element *removed = malloc(sizeof(element) * (*count));
    memcpy(removed, h->data + begin + 1, prewrap * sizeof(element));
    memset(h->data + begin, 0, prewrap * sizeof(element));
    memcpy(removed + prewrap, h->data, wrapped * sizeof(element));
    memset(h->data, 0, wrapped * sizeof(element));

    h->size -= *count;
    return removed;
}

bool hash_table_delete(hash_table *h, const char *key)
{
    if (h->size < h->capacity / 8 && h->capacity >= 16)
    {
        hash_table_realloc(h, h->capacity / 2);
    }
    size_t removed_count = 0;
    size_t removal_begin = hash_table_lookup(h, key);
    element *removed = remove_block(h, removal_begin, &removed_count);

    for (size_t i = 1; i < removed_count; ++i)
    {
        hash_table_put(h, removed[i].key, removed[i].value);
    }
    free(removed);
}

void hash_table_free(hash_table *h)
{
    free(h->data);
    free(h);
}
