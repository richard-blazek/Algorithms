#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdbool.h>

typedef struct stack stack;

stack* stack_init();
void stack_push(stack* st, float value);
bool stack_empty(stack* st);
float stack_top(stack* st);
float stack_pop(stack* st);
void stack_free(stack* st);

#endif