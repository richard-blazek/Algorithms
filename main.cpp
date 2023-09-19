#include <iostream>
#include <type_traits>
#include "sort.hpp"


template<typename T, size_t S>
void randomize_array(T (&array)[S])
{
    for (size_t i = 0; i < S; ++i)
    {
        array[i] = rand() - RAND_MAX / 2;
    }
}

template<size_t S>
void randomize_array(float (&array)[S])
{
    for (size_t i = 0; i < S; ++i)
    {
        array[i] = rand() / float(RAND_MAX) * 2 - 1;
    }
}

template<typename T, size_t S>
void print_array(T (&array)[S])
{
    for (size_t i = 0; i < S; ++i)
    {
        std::cout << array[i] << "\n";
    }
}

template<size_t S>
void print_array(uint8_t (&array)[S])
{
    for (size_t i = 0; i < S; ++i)
    {
        std::cout << int64_t(array[i]) << "\n";
    }
}

template<size_t S>
void print_array(int8_t (&array)[S])
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