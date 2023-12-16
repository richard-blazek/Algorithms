#include "stack.h"
#include "singly_ll.h"
#include <stdlib.h>

struct stack
{
    singly_ll *list;
};

stack *stack_init()
{
    stack *st = malloc(sizeof(stack));
    st->list = singly_ll_init();
    return st;
}

void stack_push(stack *st, float value)
{
    singly_ll_insert_after(st->list, value);
}

bool stack_empty(stack *st)
{
    return !st->list->next;
}

float stack_top(stack *st)
{
    return st->list->next->value;
}

float stack_pop(stack *st)
{
    return singly_ll_erase_after(st->list);
}

void stack_free(stack *st)
{
    singly_ll_free(st->list);
    free(st);
}
