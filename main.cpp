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
        std::cout << array[i] << "\n";
    }
}

template<typename T, size_t S>
constexpr size_t array_size(T (&)[S])
{
    return S;
}


int main()
{
    srand(time(0));

    int32_t items[55];

    std::cout << "Quick sort:\n";
    randomize_array(items);
    quick_sort(items, array_size(items));
    print_array(items);

    std::cout << "\nMerge sort:\n";
    randomize_array(items);
    merge_sort(items, array_size(items));
    print_array(items);

    std::cout << "\nHeap sort:\n";
    randomize_array(items);
    heap_sort(items, array_size(items));
    print_array(items);
    return 0;
}