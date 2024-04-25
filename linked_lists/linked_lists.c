#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "one_way.h"
#include "two_way.h"
#include "xor_way.h"

void print_one_way(one_way *node)
{
    node = one_way_next(node);
    while (node)
    {
        printf("%.2f ", one_way_value(node));
        node = one_way_next(node);
    }
    printf("\n");
}

void print_two_way(two_way *sentinel)
{
    two_way *current = two_way_next(sentinel);
    while (current != sentinel)
    {
        printf("%.2f ", two_way_value(current));
        current = two_way_next(current);
    }
    printf("\n");
}

void test_one_way()
{
    puts("Playing with the singly linked list:");
    one_way *list = one_way_init();
    one_way_insert_after(list, 1);
    one_way_insert_after(list, 2);
    one_way_insert_after(list, 3);
    one_way_insert_after(list, 4);
    one_way_insert_after(list, 5);
    one_way_insert_after(list, 6);
    one_way_insert_after(list, 7);
    one_way_insert_after(list, 8);
    one_way_insert_after(list, 9);
    one_way_insert_after(list, 10);
    one_way *it = one_way_next(one_way_next(one_way_next(list)));
    one_way_erase_after(it);
    it = one_way_insert_after(it, 666);
    it = one_way_next(it);
    one_way_insert_after(it, 1001);
    it = one_way_next(one_way_next(it));
    one_way_erase_after(it);
    print_one_way(list);
    one_way_free(list);
}

void test_two_way()
{
    puts("Playing with the doubly linked list:");
    two_way *list = two_way_init();
    two_way_insert_after(list, 1);
    two_way_insert_after(list, 2);
    two_way_insert_after(two_way_previous(list), 3);
    two_way_insert_after(two_way_previous(list), 4);
    two_way_insert_after(two_way_previous(list), 5);
    two_way_insert_after(list, 6);
    two_way_insert_after(list, 7);
    two_way_insert_after(list, 8);
    two_way_insert_after(list, 9);
    two_way_insert_after(list, 10);

    two_way_erase(two_way_previous(list));
    two_way *it = two_way_next(two_way_next(list));
    it = two_way_insert_after(it, 666);
    it = two_way_next(it);
    two_way_erase(it);
    print_two_way(list);
    two_way_free(list);
}

void test_xor_linked_list()
{
    puts("Playing with the XOR linked list:");

    xor_way *list = xor_way_init();
    xor_way_insert_after(0, list, 2);
    xor_way *four = xor_way_insert_after(0, list, 4);
    xor_way *five = xor_way_insert_after(0, list, 5);
    xor_way_insert_after(0, list, 6);
    xor_way *eight = xor_way_insert_after(0, list, 8);
    
    xor_way_insert_after(list, eight, 7);
    xor_way_erase(four, five);

    xor_way *current = xor_way_next(0, list), *prev = list;
    while (current)
    {
        printf("%.0f ", xor_way_value(current));
        xor_way *next = xor_way_next(prev, current);
        prev = current;
        current = next;
    }
    putchar('\n');
    xor_way_free(list);
}

int main()
{
    srand(time(0));
    test_one_way();
    test_two_way();
    test_xor_linked_list();
    return 0;
}
