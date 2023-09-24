#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sort.h"
#include "link1_list.h"
#include "link2_list.h"

#define ARRAY_SIZE 55

void fill_with_random(char* output, size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        output[i] = rand();
    }
}

void test_sort_float(void (*sort) (float*, size_t), const char* prompt)
{
    float array[ARRAY_SIZE];
    fill_with_random((char*)array, ARRAY_SIZE * sizeof(float));
    sort(array, ARRAY_SIZE);

    puts(prompt);
    for (size_t i = 0; i < ARRAY_SIZE; ++i)
    {
        printf("%f\n", array[i]);
    }
    putchar('\n');
}

void test_sort_uint8_t(void (*sort) (uint8_t*, size_t), const char* prompt)
{
    uint8_t array[ARRAY_SIZE];
    fill_with_random((char*)array, ARRAY_SIZE);
    sort(array, ARRAY_SIZE);

    puts(prompt);
    for (size_t i = 0; i < ARRAY_SIZE; ++i)
    {
        printf("%u\n", array[i]);
    }
    putchar('\n');
}


void print_link1_list(link1_node* node)
{
    node = node->next;
    while (node)
    {
        printf("%.2f ", node->value);
        node = node->next;
    }
    printf("\n");
}

void print_link2_list(link2_node* sentinel)
{
    link2_node* current = sentinel->next;
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
    link1_node* list = link1_list_init();
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
    link1_node* it = list->next->next->next;
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
    puts("Playing with the singly linked list:");
    link2_node* list = link2_list_init();
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
    link2_node* it = list->next->next;
    it = link2_list_insert_after(it, 666);
    it = it->next;
    link2_list_erase(it);
    print_link2_list(list);
    link2_list_free(list);
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
    return 0;
}