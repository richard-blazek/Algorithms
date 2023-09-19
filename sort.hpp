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
void merge_sort(T* begin, T* end)
{
    size_t length = end - begin;
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
void heap_sort(T* begin, T* end)
{
    size_t length = end - begin;
    build_heap(begin, length);
    
    for (size_t i = length; i > 0; --i)
    {
        shift_heap_root(begin, i);
    }
}

#endif