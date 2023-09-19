#ifndef SORT_H_INCLUDED
#define SORT_H_INCLUDED

#include <cstdlib>
#include <cstdint>
#include <utility>

using std::size_t;
using std::uint8_t;


template<typename T>
T* partition_by_random_pivot(T* begin, T* end)
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
void quick_sort_between(T* begin, T* end)
{
    if (end <= begin + 1)
    {
        return;
    }
    T* pivot = partition_by_random_pivot(begin, end);
    // Tail recurse with the greater partition
    if (pivot - begin > end - pivot - 1)
    {
        quick_sort_between(pivot + 1, end);
        quick_sort_between(begin, pivot);
    }
    else
    {
        quick_sort_between(begin, pivot);
        quick_sort_between(pivot + 1, end);
    }
}

template<typename T>
void quick_sort(T* begin, size_t length)
{
    quick_sort_between(begin, begin + length);
}


template<typename T>
void merge(T* begin1, T* end1, T* begin2, T* end2, T* dest)
{
    while (begin1 != end1 && begin2 != end2)
    {
        *dest++ = std::move(*begin1 <= *begin2 ? *begin1++ : *begin2++);
    }
    while (begin1 != end1)
    {
        *dest++ = std::move(*begin1++);
    }
    while (begin2 != end2)
    {
        *dest++ = std::move(*begin2++);
    }
}

template<typename T>
void merge_sort(T* array, size_t length)
{
    T* buffer = new T[length];
    T* original = array;
    for (size_t partition = 1; partition < length; partition <<= 1)
    {
        for (size_t i = 0; i < length; i += partition << 1)
        {
            size_t mid_i = std::min(i + partition, length);
            size_t end_i = std::min(mid_i + partition, length);
            merge(array + i, array + mid_i, array + mid_i, array + end_i, buffer + i);
        }

        std::swap(buffer, array);
    }

    if (original == buffer)
    {
        std::copy(array, array + length, buffer);
        delete[] array;
    }
}


template<typename T>
void insert_to_heap(T* heap, size_t old_size, T element)
{
    heap[old_size] = std::move(element);
    for (size_t i = old_size, parent = (i - 1) >> 1; i > 0 && heap[parent] < heap[i]; i = parent, parent = (i - 1) >> 1)
    {
        std::swap(heap[i], heap[parent]);
    }
}

template<typename T>
void build_heap(T* heap, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        insert_to_heap(heap, i, heap[i]);
    }
}

template<typename T>
void shift_heap_root(T* heap, size_t size)
{
    std::swap(heap[0], heap[size - 1]);

    size_t i = 0;
    size -= 1;
    for (;;)
    {
        size_t left = i << 1 | 1, right = (i << 1) + 2;
        if (left >= size)
        {
            break;
        }
        size_t child = right < size && heap[right] > heap[left] ? right : left;
        if (heap[child] <= heap[i])
        {
            break;
        }

        std::swap(heap[i], heap[child]);
        i = child;
    }
}

template<typename T>
void heap_sort(T* array, size_t length)
{
    build_heap(array, length);
    
    for (size_t i = length; i > 0; --i)
    {
        shift_heap_root(array, i);
    }
}

#endif