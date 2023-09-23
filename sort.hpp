#ifndef SORT_H_INCLUDED
#define SORT_H_INCLUDED

#include <cstdlib>
#include <cstdint>
#include <utility>

using std::size_t;
using std::uint8_t;
using std::int8_t;
using std::uint16_t;
using std::int16_t;
using std::uint32_t;
using std::int32_t;
using std::uint64_t;
using std::int64_t;


template <typename T>
T *partition_by_random_pivot(T *begin, T *end)
{
    T *pivot = begin + rand() % (end - begin);
    for (T *ptr = begin; ptr < end; ++ptr)
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
    for (T *ptr = end - 1; ptr >= begin; --ptr)
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

template <typename T>
void quick_sort_between(T *begin, T *end)
{
    if (end <= begin + 1)
    {
        return;
    }
    T *pivot = partition_by_random_pivot(begin, end);
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

template <typename T>
void quick_sort(T *begin, size_t length)
{
    quick_sort_between(begin, begin + length);
}


template <typename T>
void merge(T *begin1, T *end1, T *begin2, T *end2, T *dest)
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

template <typename T>
void merge_sort(T *array, size_t length)
{
    T *buffer = new T[length];
    T *original = array;
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


template <typename T>
void insert_to_heap(T *heap, size_t old_size, T element)
{
    heap[old_size] = std::move(element);
    for (size_t i = old_size, parent = (i - 1) >> 1; i > 0 && heap[parent] < heap[i]; i = parent, parent = (i - 1) >> 1)
    {
        std::swap(heap[i], heap[parent]);
    }
}

template <typename T>
void build_heap(T *heap, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        insert_to_heap(heap, i, heap[i]);
    }
}

template <typename T>
void unshift_heap(T *heap, size_t size)
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

template <typename T>
void heap_sort(T* array, size_t length)
{
    build_heap(array, length);

    for (size_t i = length; i > 0; --i)
    {
        unshift_heap(array, i);
    }
}


template<typename T, int offset>
void counting_sort_generic(T* array, size_t length)
{
    size_t counts[256] = {0};
    for (size_t i = 0; i < length; ++i)
    {
        ++counts[array[i] + offset];
    }
    for (size_t i = 0, dest = 0; i < 256; ++i)
    {
        for (size_t count = counts[i]; count > 0; --count, ++dest)
        {
            array[dest] = i - offset;
        }
    }
}

void counting_sort(uint8_t* array, size_t length)
{
    counting_sort_generic<uint8_t, 0>(array, length);
}

void counting_sort(int8_t* array, size_t length)
{
    counting_sort_generic<int8_t, 128>(array, length);
}


template<typename T, typename SmallInt, size_t Range, SmallInt (*GetValue) (T)>
void radix_sort_generic(T* input, size_t length, T* output)
{
    size_t counts[Range] = {0};
    size_t starts[Range] = {0};

    for (size_t i = 0; i < length; ++i)
    {
        ++counts[GetValue(input[i])];
    }
    for (size_t i = 1; i <= Range; ++i)
    {
        starts[i] = starts[i - 1] + counts[i - 1];
    }

    for (size_t i = 0; i < length; ++i)
    {
        size_t* start = starts + GetValue(input[i]);
        output[*start] = input[i];
        ++*start;
    }
}

template<int lsr, typename R>
constexpr R shift_and_convert_int32_t(int32_t value)
{
    return R(value + 0x8000'0000 >> lsr);
}

void radix_sort(int32_t* array, size_t length)
{
    int32_t* buffer = new int32_t[length];
    radix_sort_generic<int32_t, uint8_t, 256, shift_and_convert_int32_t< 0, uint8_t>>(array, length, buffer);
    radix_sort_generic<int32_t, uint8_t, 256, shift_and_convert_int32_t< 8, uint8_t>>(buffer, length, array);
    radix_sort_generic<int32_t, uint8_t, 256, shift_and_convert_int32_t<16, uint8_t>>(array, length, buffer);
    radix_sort_generic<int32_t, uint8_t, 256, shift_and_convert_int32_t<24, uint8_t>>(buffer, length, array);
    delete[] buffer;
}

constexpr uint32_t convert_float_to_int(float value)
{
    uint32_t i = *(uint32_t*)&value;
    uint32_t sign = i >> 31;
    return ((!sign - 1) ^ (i | 0x8000'0000)) + sign;
}

template<int lsr>
constexpr uint8_t shift_and_convert_float(float value)
{
    return convert_float_to_int(value) >> lsr;
}

void radix_sort(float* array, size_t length)
{
    float* buffer = new float[length];
    radix_sort_generic<float, uint8_t, 256, shift_and_convert_float<0>>(array, length, buffer);
    radix_sort_generic<float, uint8_t, 256, shift_and_convert_float<8>>(buffer, length, array);
    radix_sort_generic<float, uint8_t, 256, shift_and_convert_float<16>>(array, length, buffer);
    radix_sort_generic<float, uint8_t, 256, shift_and_convert_float<24>>(buffer, length, array);
    delete[] buffer;
}

#endif