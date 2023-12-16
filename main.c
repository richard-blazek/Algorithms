#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sort.h"
#include "link1_list.h"
#include "link2_list.h"
#include "stack.h"
#include "queue.h"
#include "deque.h"
#include "priority_queue.h"
#include "hash_table.h"

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

void print_link1_list(link1_node *node)
{
    node = node->next;
    while (node)
    {
        printf("%.2f ", node->value);
        node = node->next;
    }
    printf("\n");
}

void print_link2_list(link2_node *sentinel)
{
    link2_node *current = sentinel->next;
    while (current != sentinel)
    {
        printf("%.2f ", current->value);
        current = current->next;
    }
    printf("\n");
}

void test_link1_list()
{
    puts("Playing with the singly linked list:");
    link1_node *list = link1_list_init();
    link1_list_insert_after(list, 1);
    link1_list_insert_after(list, 2);
    link1_list_insert_after(list, 3);
    link1_list_insert_after(list, 4);
    link1_list_insert_after(list, 5);
    link1_list_insert_after(list, 6);
    link1_list_insert_after(list, 7);
    link1_list_insert_after(list, 8);
    link1_list_insert_after(list, 9);
    link1_list_insert_after(list, 10);
    link1_node *it = list->next->next->next;
    link1_list_erase_after(it);
    it = link1_list_insert_after(it, 666);
    it = it->next;
    link1_list_insert_after(it, 1001);
    it = it->next->next;
    link1_list_erase_after(it);
    print_link1_list(list);
    link1_list_free(list);
}

void test_link2_list()
{
    puts("Playing with the doubly linked list:");
    link2_node *list = link2_list_init();
    link2_list_insert_before(list, 1);
    link2_list_insert_before(list, 2);
    link2_list_insert_before(list, 3);
    link2_list_insert_before(list, 4);
    link2_list_insert_before(list, 5);
    link2_list_insert_after(list, 6);
    link2_list_insert_after(list, 7);
    link2_list_insert_after(list, 8);
    link2_list_insert_after(list, 9);
    link2_list_insert_after(list, 10);

    link2_list_erase_before(list);
    link2_node *it = list->next->next;
    it = link2_list_insert_after(it, 666);
    it = it->next;
    link2_list_erase(it);
    print_link2_list(list);
    link2_list_free(list);
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
}

int main()
{
    srand(time(0));

    test_sort_float(quick_sort, "Quick sort:");
    test_sort_float(merge_sort, "Merge sort:");
    test_sort_float(heap_sort, "Heap sort:");
    test_sort_float(radix_sort, "Radix sort:");
    test_sort_uint8_t(counting_sort, "Counting sort:");

    test_link1_list();
    test_link2_list();
    test_stack();
    test_queue();
    test_deque();
    test_priority_queue();
    test_hash_table();
    return 0;
}