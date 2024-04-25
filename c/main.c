#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "singly_ll.h"
#include "doubly_ll.h"
#include "xored_ll.h"

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
    doubly_ll *current = doubly_ll_next(sentinel);
    while (current != sentinel)
    {
        printf("%.2f ", doubly_ll_value(current));
        current = doubly_ll_next(current);
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
    doubly_ll_insert_after(doubly_ll_previous(list), 3);
    doubly_ll_insert_after(doubly_ll_previous(list), 4);
    doubly_ll_insert_after(doubly_ll_previous(list), 5);
    doubly_ll_insert_after(list, 6);
    doubly_ll_insert_after(list, 7);
    doubly_ll_insert_after(list, 8);
    doubly_ll_insert_after(list, 9);
    doubly_ll_insert_after(list, 10);

    doubly_ll_erase(doubly_ll_previous(list));
    doubly_ll *it = doubly_ll_next(doubly_ll_next(list));
    it = doubly_ll_insert_after(it, 666);
    it = doubly_ll_next(it);
    doubly_ll_erase(it);
    print_doubly_ll(list);
    doubly_ll_free(list);
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
    test_singly_ll();
    test_doubly_ll();
    test_xor_linked_list();
    return 0;
}
