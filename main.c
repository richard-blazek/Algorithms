#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sort.h"
#include "singly_ll.h"
#include "doubly_ll.h"
#include "stack.h"
#include "queue.h"
#include "deque.h"
#include "priority_queue.h"
#include "hash_table.h"
#include "xored_ll.h"

#define ARRAY_SIZE 55

void fill_with_random(char *output, size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        output[i] = rand();
    }
}

void test_sort_float(void (*sort)(float *, size_t), const char *prompt)
{
    float array[ARRAY_SIZE];
    fill_with_random((char *)array, ARRAY_SIZE * sizeof(float));
    sort(array, ARRAY_SIZE);

    puts(prompt);
    for (size_t i = 0; i < ARRAY_SIZE; ++i)
    {
        printf("%f\n", array[i]);
    }
    putchar('\n');
}

void test_sort_uint8_t(void (*sort)(uint8_t *, size_t), const char *prompt)
{
    uint8_t array[ARRAY_SIZE];
    fill_with_random((char *)array, ARRAY_SIZE);
    sort(array, ARRAY_SIZE);

    puts(prompt);
    for (size_t i = 0; i < ARRAY_SIZE; ++i)
    {
        printf("%u\n", array[i]);
    }
    putchar('\n');
}

void print_singly_ll(singly_ll *node)
{
    node = singly_ll_next(node);
    while (node)
    {
        printf("%.2f ", singly_ll_value(node));
        node = singly_ll_next(node);
    }
    printf("\n");
}

void print_doubly_ll(doubly_ll *sentinel)
{
    doubly_ll *current = sentinel->next;
    while (current != sentinel)
    {
        printf("%.2f ", current->value);
        current = current->next;
    }
    printf("\n");
}

void test_singly_ll()
{
    puts("Playing with the singly linked list:");
    singly_ll *list = singly_ll_init();
    singly_ll_insert_after(list, 1);
    singly_ll_insert_after(list, 2);
    singly_ll_insert_after(list, 3);
    singly_ll_insert_after(list, 4);
    singly_ll_insert_after(list, 5);
    singly_ll_insert_after(list, 6);
    singly_ll_insert_after(list, 7);
    singly_ll_insert_after(list, 8);
    singly_ll_insert_after(list, 9);
    singly_ll_insert_after(list, 10);
    singly_ll *it = singly_ll_next(singly_ll_next(singly_ll_next(list)));
    singly_ll_erase_after(it);
    it = singly_ll_insert_after(it, 666);
    it = singly_ll_next(it);
    singly_ll_insert_after(it, 1001);
    it = singly_ll_next(singly_ll_next(it));
    singly_ll_erase_after(it);
    print_singly_ll(list);
    singly_ll_free(list);
}

void test_doubly_ll()
{
    puts("Playing with the doubly linked list:");
    doubly_ll *list = doubly_ll_init();
    doubly_ll_insert_after(list, 1);
    doubly_ll_insert_after(list, 2);
    doubly_ll_insert_after(list->prev, 3);
    doubly_ll_insert_after(list->prev, 4);
    doubly_ll_insert_after(list->prev, 5);
    doubly_ll_insert_after(list, 6);
    doubly_ll_insert_after(list, 7);
    doubly_ll_insert_after(list, 8);
    doubly_ll_insert_after(list, 9);
    doubly_ll_insert_after(list, 10);

    doubly_ll_erase(list->prev);
    doubly_ll *it = list->next->next;
    it = doubly_ll_insert_after(it, 666);
    it = it->next;
    doubly_ll_erase(it);
    print_doubly_ll(list);
    doubly_ll_free(list);
}

void test_stack()
{
    puts("Playing with the stack:");
    stack *st = stack_init();
    stack_push(st, 1);
    stack_push(st, 1);
    stack_push(st, 2);
    stack_push(st, 3);
    stack_push(st, 5);
    stack_push(st, 8);
    stack_push(st, 13);
    stack_push(st, 21);
    while (!stack_empty(st))
    {
        printf("%.1f ", stack_pop(st));
    }
    putchar('\n');
    stack_free(st);
}

void test_queue()
{
    puts("Playing with the queue:");
    queue *q = queue_init();
    queue_push(q, 1);
    queue_push(q, 1);
    queue_push(q, 2);
    queue_push(q, 3);
    queue_push(q, 5);
    queue_push(q, 8);
    queue_push(q, 13);
    queue_push(q, 21);
    while (!queue_empty(q))
    {
        printf("%.1f ", queue_pop(q));
    }
    putchar('\n');
    queue_free(q);
}

void test_deque()
{
    puts("Playing with the deque:");
    deque *dq = deque_init(3);
    printf("%ld %ld\n", deque_length(dq), deque_capacity(dq));
    deque_push_back(dq, 1);
    deque_push_back(dq, 2);
    deque_push_front(dq, 0.5);
    deque_push_back(dq, 3);
    deque_push_front(dq, 0.1);
    printf("%ld %ld\n", deque_length(dq), deque_capacity(dq));
    deque_reserve(dq, 10);
    deque_push_back(dq, 4);
    printf("%ld %ld\n", deque_length(dq), deque_capacity(dq));
    deque_reserve(dq, 33);
    printf("%ld %ld\n", deque_length(dq), deque_capacity(dq));
    deque_resize(dq, 7);
    deque_shrink(dq);
    printf("%ld %ld\n", deque_length(dq), deque_capacity(dq));

    for (size_t i = 0, len = deque_length(dq); i < len; ++i)
    {
        printf("%.1f ", deque_get(dq, i));
    }
    puts("\n");
    deque_free(dq);
}

void test_priority_queue()
{
    puts("Playing with the priority queue:");
    priority_queue *priorityq = priority_queue_init();
    priority_queue_push(priorityq, 1.0 / 4, 4);
    priority_queue_push(priorityq, 1.0 / 8, 8);
    priority_queue_push(priorityq, 1.0 / 1, 1);
    priority_queue_push(priorityq, 1.0 / 5, 5);
    priority_queue_push(priorityq, 1.0 / 2, 2);
    priority_queue_push(priorityq, 1.0 / 4, 4);
    priority_queue_push(priorityq, 1.0 / 10, 10);
    priority_queue_push(priorityq, 1.0 / 4, 6);
    priority_queue_push(priorityq, 1.0 / 4, 3);
    while (!priority_queue_empty(priorityq))
    {
        printf("(%d ", priority_queue_top_priority(priorityq));
        printf("%.3f) ", priority_queue_pop(priorityq));
    }
    putchar('\n');
    priority_queue_free(priorityq);
}

void test_hash_table()
{
    puts("Playing with the hash table:");
    hash_table *ht = hash_table_init();

    const char *str = "I don't want to live in the hearts of my countrymen; I want to live in my apartment.";

    for (size_t i = 0; i < 66; ++i)
    {
        hash_table_put(ht, str + i, i * 10);
    }
    for (size_t i = 0; i < 66; i += 2)
    {
        hash_table_delete(ht, str + i);
    }
    for (size_t i = 1; i < 66; i += 4)
    {
        hash_table_delete(ht, str + i);
    }
    for (size_t i = 0; i < 66; i += 3)
    {
        hash_table_delete(ht, str + i);
    }
    for (size_t i = 0; i < 66; ++i)
    {
        printf("i: %lu | has: %d | value: %.0f\n", i, hash_table_has(ht, str + i), hash_table_get(ht, str + i));
    }
    hash_table_free(ht);
}

void test_xor_linked_list()
{
    puts("Playing with the XOR linked list:");

    xored_ll *list = xored_ll_init();
    xored_ll_insert_after(0, list, 2);
    xored_ll *four = xored_ll_insert_after(0, list, 4);
    xored_ll *five = xored_ll_insert_after(0, list, 5);
    xored_ll_insert_after(0, list, 6);
    xored_ll *eight = xored_ll_insert_after(0, list, 8);
    
    xored_ll_insert_after(list, eight, 7);
    xored_ll_erase(four, five);

    xored_ll *current = xored_ll_next(0, list), *prev = list;
    while (current)
    {
        printf("%.0f ", xored_ll_value(current));
        xored_ll *next = xored_ll_next(prev, current);
        prev = current;
        current = next;
    }
    putchar('\n');
    xored_ll_free(list);
}

int main()
{
    srand(time(0));

    test_sort_float(quick_sort, "Quick sort:");
    test_sort_float(merge_sort, "Merge sort:");
    test_sort_float(heap_sort, "Heap sort:");
    test_sort_float(radix_sort, "Radix sort:");
    test_sort_uint8_t(counting_sort, "Counting sort:");

    test_singly_ll();
    test_doubly_ll();
    test_stack();
    test_queue();
    test_deque();
    test_priority_queue();
    test_hash_table();
    test_xor_linked_list();
    return 0;
}