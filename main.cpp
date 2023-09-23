#include <iostream>

#include "sort.hpp"
#include "utils.hpp"
#include "single_list.hpp"


template<typename T, size_t S, void (*Sort) (T*, size_t)>
void test_sort(std::string prompt)
{
    T items[S];
    fill_with_random(items);
    Sort(items, std::size(items));

    std::cout << prompt;
    print_many(items);
    std::cout << "\n";
}


int main()
{
    srand(time(0));

    test_sort<int32_t, 55, quick_sort>("Quick sort:\n");
    test_sort<int32_t, 55, merge_sort>("Merge sort:\n");
    test_sort<int32_t, 55, heap_sort>("Heap sort:\n");
    test_sort<uint8_t, 55, counting_sort>("Counting sort - unsigned:\n");
    test_sort<int8_t, 55, counting_sort>("Counting sort - signed:\n");
    test_sort<int32_t, 55, radix_sort>("Radix sort - integers:\n");
    test_sort<float, 55, radix_sort>("Radix sort - floats:\n");

    std::cout << "Just playing with single-linked list:\n";
    single_list<int> list;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    list.push_front(4);
    list.push_front(5);
    list.push_front(6);
    list.push_front(7);
    list.push_front(8);
    list.push_front(9);
    list.push_front(10);
    auto backup = list;
    list.pop_front();
    auto it = list.begin();
    ++it;
    ++it;
    ++it;
    it.erase();
    it.insert(666);
    ++it;
    it.insert(1001);
    ++it;
    ++it;
    it.erase();
    print_many(list);
    std::cout << "\n";
    print_many(backup);
    return 0;
}