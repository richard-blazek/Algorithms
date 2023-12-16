#include "stack.h"
#include "link1_list.h"
#include <stdlib.h>

struct stack
{
    link1_node *list;
};

stack *stack_init()
{
    stack *st = malloc(sizeof(stack));
    st->list = link1_list_init();
    return st;
}

void stack_push(stack *st, float value)
{
    link1_list_insert_after(st->list, value);
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
    return link1_list_erase_after(st->list);
}

void stack_free(stack *st)
{
    link1_list_free(st->list);
    free(st);
}
