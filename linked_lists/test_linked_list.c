#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "singly_linked_list.h"
#include "doubly_linked_list.h"
#include "xorptr_linked_list.h"

void print_doubly_linked_list(doubly_linked_list *sentinel)
{
    doubly_linked_list *current = sentinel;
    while ((current = doubly_linked_list_next(current)) != sentinel)
    {
        printf("%.2f ", doubly_linked_list_value(current));
    }
    printf("\n");
}

void test_singly_linked_list()
{
    singly_linked_list *list = singly_linked_list_init();
    singly_linked_list_insert_after(list, 1);
    singly_linked_list_insert_after(list, 2);
    singly_linked_list_insert_after(list, 3);
    singly_linked_list_insert_after(list, 4);
    singly_linked_list_insert_after(list, 5);
    singly_linked_list_insert_after(list, 6);
    singly_linked_list_insert_after(list, 7);
    singly_linked_list_insert_after(list, 8);
    singly_linked_list_insert_after(list, 9);
    singly_linked_list_insert_after(list, 10);
    singly_linked_list *it = singly_linked_list_next(singly_linked_list_next(singly_linked_list_next(list)));
    singly_linked_list_erase_after(it);
    it = singly_linked_list_insert_after(it, 666);
    it = singly_linked_list_next(it);
    singly_linked_list_insert_after(it, 1001);
    it = singly_linked_list_next(singly_linked_list_next(it));
    singly_linked_list_erase_after(it);

    it = list;
    assert(singly_linked_list_value(it = singly_linked_list_next(it)) == 10.0);
    assert(singly_linked_list_value(it = singly_linked_list_next(it)) == 9.0);
    assert(singly_linked_list_value(it = singly_linked_list_next(it)) == 8.0);
    assert(singly_linked_list_value(it = singly_linked_list_next(it)) == 666.0);
    assert(singly_linked_list_value(it = singly_linked_list_next(it)) == 6.0);
    assert(singly_linked_list_value(it = singly_linked_list_next(it)) == 1001.0);
    assert(singly_linked_list_value(it = singly_linked_list_next(it)) == 5.0);
    assert(singly_linked_list_value(it = singly_linked_list_next(it)) == 3.0);
    assert(singly_linked_list_value(it = singly_linked_list_next(it)) == 2.0);
    assert(singly_linked_list_value(it = singly_linked_list_next(it)) == 1.0);
    assert((it = singly_linked_list_next(it)) == 0);

    singly_linked_list_free(list);
    puts("test_singly_linked_list: OK");
}

void test_doubly_linked_list()
{
    doubly_linked_list *list = doubly_linked_list_init();
    doubly_linked_list_insert_after(list, 1);
    doubly_linked_list_insert_after(list, 2);
    doubly_linked_list_insert_after(doubly_linked_list_previous(list), 3);
    doubly_linked_list_insert_after(doubly_linked_list_previous(list), 4);
    doubly_linked_list_insert_after(doubly_linked_list_previous(list), 5);
    doubly_linked_list_insert_after(list, 6);
    doubly_linked_list_insert_after(list, 7);
    doubly_linked_list_insert_after(list, 8);
    doubly_linked_list_insert_after(list, 9);
    doubly_linked_list_insert_after(list, 10);

    doubly_linked_list_erase(doubly_linked_list_previous(list));
    doubly_linked_list *it = doubly_linked_list_next(doubly_linked_list_next(list));
    it = doubly_linked_list_insert_after(it, 666);
    it = doubly_linked_list_next(it);
    doubly_linked_list_erase(it);

    it = list;
    assert((it = doubly_linked_list_next(it)) != list);
    assert(doubly_linked_list_value(it) == 10.0);
    assert((it = doubly_linked_list_next(it)) != list);
    assert(doubly_linked_list_value(it) == 9.0);
    assert((it = doubly_linked_list_next(it)) != list);
    assert(doubly_linked_list_value(it) == 666.0);
    assert((it = doubly_linked_list_next(it)) != list);
    assert(doubly_linked_list_value(it) == 7.0);
    assert((it = doubly_linked_list_next(it)) != list);
    assert(doubly_linked_list_value(it) == 6.0);
    assert((it = doubly_linked_list_next(it)) != list);
    assert(doubly_linked_list_value(it) == 2.0);
    assert((it = doubly_linked_list_next(it)) != list);
    assert(doubly_linked_list_value(it) == 1.0);
    assert((it = doubly_linked_list_next(it)) != list);
    assert(doubly_linked_list_value(it) == 3.0);
    assert((it = doubly_linked_list_next(it)) != list);
    assert(doubly_linked_list_value(it) == 4.0);
    assert((it = doubly_linked_list_next(it)) == list);

    doubly_linked_list_free(list);
    puts("test_doubly_linked_list: OK");
}

void test_xor_linked_list()
{
    xorptr_linked_list *list = xorptr_linked_list_init();
    xorptr_linked_list_insert_after(0, list, 2);
    xorptr_linked_list *four = xorptr_linked_list_insert_after(0, list, 4);
    xorptr_linked_list *five = xorptr_linked_list_insert_after(0, list, 5);
    xorptr_linked_list_insert_after(0, list, 6);
    xorptr_linked_list *eight = xorptr_linked_list_insert_after(0, list, 8);
    
    xorptr_linked_list_insert_after(list, eight, 7);
    xorptr_linked_list_erase(four, five);

    xorptr_linked_list *current = xorptr_linked_list_next(0, list), *prev = list, *next;

    assert(xorptr_linked_list_value(current) == 8.0);
    next = xorptr_linked_list_next(prev, current);
    prev = current;
    current = next;

    assert(xorptr_linked_list_value(current) == 7.0);
    next = xorptr_linked_list_next(prev, current);
    prev = current;
    current = next;

    assert(xorptr_linked_list_value(current) == 6.0);
    next = xorptr_linked_list_next(prev, current);
    prev = current;
    current = next;

    assert(xorptr_linked_list_value(current) == 4.0);
    next = xorptr_linked_list_next(prev, current);
    prev = current;
    current = next;

    assert(xorptr_linked_list_value(current) == 2.0);
    next = xorptr_linked_list_next(prev, current);
    prev = current;
    current = next;

    assert(current == 0);
    xorptr_linked_list_free(list);

    puts("test_xor_linked_list: OK");
}

int main()
{
    srand(time(0));
    test_singly_linked_list();
    test_doubly_linked_list();
    test_xor_linked_list();
    return 0;
}
