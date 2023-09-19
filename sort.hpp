#ifndef SORT_H_INCLUDED
#define SORT_H_INCLUDED

#include <cstdlib>
#include <utility>

using std::size_t;

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
        if (*ptr < *pivot)
        {
            T tmp = std::move(*pivot);
            *pivot = std::move(*ptr);
            *ptr = std::move(pivot[1]);
            pivot[1] = std::move(tmp);
            ++pivot;
        }
    }
    // Tail recurse with the greater partition
    if (pivot - begin > end - pivot - 1)
    {
        quick_sort(pivot + 1, end);
        quick_sort(begin, pivot);
    }
    else
    {
        quick_sort(begin, pivot);
        quick_sort(pivot + 1, end);
    }
}

#endif