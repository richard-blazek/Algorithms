#include <iostream>
#include "sort.hpp"


template<typename T, size_t S>
void randomize_array(T (&array)[S])
{
    for (size_t i = 0; i < S; ++i)
    {
        array[i] = rand();
    }
}

template<typename T, size_t S>
void print_array(T (&array)[S])
{
    for (size_t i = 0; i < S; ++i)
    {
        std::cout << int64_t(array[i]) << "\n";
    }
}

template<typename T, size_t S>
constexpr size_t array_size(T (&)[S])
{
    return S;
}


template<typename T, size_t S, void (*Fn) (T*, size_t)>
void test_sort(std::string prompt)
{
    T items[S];
    randomize_array(items);
    Fn(items, array_size(items));

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
    return 0;
}