#include "queue.h"
#include "doubly_ll.h"
#include <stdlib.h>

struct queue
{
    doubly_ll *list;
};

queue *queue_init()
{
    queue *q = malloc(sizeof(queue));
    q->list = doubly_ll_init();
    return q;
}

void queue_push(queue *q, float value)
{
    doubly_ll_insert_after(q->list, value);
}

bool queue_empty(queue *q)
{
    return doubly_ll_next(q->list) == q->list;
}

float queue_top(queue *q)
{
    return doubly_ll_value(doubly_ll_previous(q->list));
}

float queue_pop(queue *q)
{
    return doubly_ll_erase(doubly_ll_previous(q->list));
}

void queue_free(queue *q)
{
    doubly_ll_free(q->list);
    free(q);
}
