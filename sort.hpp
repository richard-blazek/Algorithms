#ifndef SORT_H_INCLUDED
#define SORT_H_INCLUDED

#include <cstdlib>
#include <utility>

using std::size_t;

template<typename T>
void dbg_print(T* begin, T* end)
{
    std::cout<<"[";
    while (begin < end)
    {
        std::cout<<*begin<<" ";
        ++begin;
    }
    std::cout<<"]\n";
}

template<typename T>
void quick_sort(T* begin, T* end)
{
    if (end <= begin + 1)
    {
        return;
    }
    T* pivot = begin;// + rand() % (end - begin);
    for (T* ptr = begin; ptr < end; ++ptr)
    {
        if ((*ptr < *pivot && ptr > pivot) || (*ptr > *pivot && ptr < pivot))
        {
            std::swap(*ptr, *pivot);
            pivot = ptr;
        }
    }
    quick_sort(begin, pivot);
    quick_sort(pivot + 1, end);
}

#endif