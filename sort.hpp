#ifndef SORT_H_INCLUDED
#define SORT_H_INCLUDED

#include <cstdlib>
#include <utility>

using std::size_t;


template<typename T>
T* quick_sort_partition(T* begin, T* end)
{
    T* pivot = begin + rand() % (end - begin);
    for (T* ptr = begin; ptr < end; ++ptr)
    {
        if (*ptr < *pivot && ptr > pivot)
        {
            T tmp = std::move(*pivot);
            *pivot = std::move(*ptr);
            *ptr = std::move(pivot[1]);
            pivot[1] = std::move(tmp);
            ++pivot;
        }
    }
    for (T* ptr = end - 1; ptr >= begin; --ptr)
    {
        if (*ptr > *pivot && ptr < pivot)
        {
            T tmp = std::move(*pivot);
            *pivot = std::move(*ptr);
            *ptr = std::move(pivot[-1]);
            pivot[-1] = std::move(tmp);
            --pivot;
        }
    }
    return pivot;
}

template<typename T>
void quick_sort(T* begin, T* end)
{
    if (end <= begin + 1)
    {
        return;
    }
    T* pivot = quick_sort_partition(begin, end);
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


template<typename T>
void merge(T* begin1, T* end1, T* begin2, T* end2, T* dest)
{
    while (begin1 != end1 && begin2 != end2)
    {
        if (*begin1 <= *begin2)
        {
            *dest = std::move(*begin1);
            ++begin1;
        }
        else
        {
            *dest = std::move(*begin2);
            ++begin2;
        }
        ++dest;
    }
    while (begin1 != end1)
    {
        *dest = std::move(*begin1);
        ++dest;
        ++begin1;
    }
    while (begin2 != end2)
    {
        *dest = std::move(*begin2);
        ++dest;
        ++begin2;
    }
}

template<typename T>
void merge_sort(T* begin, size_t length)
{
    T* buffer = new T[length];
    T* original = begin;
    for (size_t partition = 1; partition < length; partition <<= 1)
    {
        for (size_t i = 0; i < length; i += partition << 1)
        {
            size_t mid_i = std::min(i + partition, length);
            size_t end_i = std::min(mid_i + partition, length);
            merge(begin + i, begin + mid_i, begin + mid_i, begin + end_i, buffer + i);
        }

        std::swap(buffer, begin);
    }

    if (original == buffer)
    {
        std::copy(begin, begin + length, buffer);
        delete[] begin;
    }
}

#endif