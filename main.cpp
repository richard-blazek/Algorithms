#include <iostream>

#include "sort.hpp"
#include "array_utils.hpp"


template<typename T, size_t S, void (*Sort) (T*, size_t)>
void test_sort(std::string prompt)
{
    T items[S];
    randomize_array(items);
    Sort(items, array_size(items));

    std::cout << prompt;
    print_array(items);
}


int main()
{
    srand(time(0));

    test_sort<int32_t, 55, quick_sort>("Quick sort:\n");
    test_sort<int32_t, 55, merge_sort>("\nMerge sort:\n");
    test_sort<int32_t, 55, heap_sort>("\nHeap sort:\n");
    test_sort<uint8_t, 55, counting_sort>("\nCounting sort - unsigned:\n");
    test_sort<int8_t, 55, counting_sort>("\nCounting sort - signed:\n");
    test_sort<int32_t, 55, radix_sort>("\nRadix sort - integers:\n");
    test_sort<float, 55, radix_sort>("\nRadix sort - floats:\n");
    return 0;
}