#include "queue.h"
#include "link2_list.h"
#include <stdlib.h>

struct queue
{
    link2_node* list;
};

queue* queue_init()
{
    queue* q = malloc(sizeof(queue));
    q->list = link2_list_init();
    return q;
}

void queue_push(queue* q, float value)
{
    link2_list_insert_after(q->list, value);
}

bool queue_empty(queue* q)
{
    return q->list->next = q->list;
}

float queue_top(queue* q)
{
    return q->list->prev->value;
}

float queue_pop(queue* q)
{
    return link2_list_erase_before(q->list);
}

void queue_free(queue* q)
{
    link2_list_free(q->list);
    free(q);
}
