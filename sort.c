#include "sort.h"
#include <string.h>

static float* partition_by_random_pivot(float* begin, float* end)
{
    float* pivot = begin + rand() % (end - begin);
    for (float* ptr = begin; ptr < end; ++ptr)
    {
        if (*ptr < *pivot && ptr > pivot)
        {
            float tmp = *pivot;
            *pivot = *ptr;
            *ptr = pivot[1];
            pivot[1] = tmp;
            ++pivot;
        }
    }
    for (float* ptr = end - 1; ptr >= begin; --ptr)
    {
        if (*ptr > *pivot && ptr < pivot)
        {
            float tmp = *pivot;
            *pivot = *ptr;
            *ptr = pivot[-1];
            pivot[-1] = tmp;
            --pivot;
        }
    }
    return pivot;
}

static void quick_sort_between(float* begin, float* end)
{
    if (end <= begin + 1)
    {
        return;
    }
    float* pivot = partition_by_random_pivot(begin, end);
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

void quick_sort(float* begin, size_t length)
{
    quick_sort_between(begin, begin + length);
}


static void merge(float* begin1, float* end1, float* begin2, float* end2, float* dest)
{
    while (begin1 != end1 && begin2 != end2)
    {
        *dest++ = *begin1 <= *begin2 ? *begin1++ : *begin2++;
    }
    while (begin1 != end1)
    {
        *dest++ = *begin1++;
    }
    while (begin2 != end2)
    {
        *dest++ = *begin2++;
    }
}

void merge_sort(float* array, size_t length)
{
    float* buffer = malloc(sizeof(float) * length);
    float* original = array;
    for (size_t partition = 1; partition < length; partition <<= 1)
    {
        for (size_t i = 0; i < length; i += partition << 1)
        {
            size_t mid_i = i + partition >= length ? length : i + partition;
            size_t end_i = mid_i + partition >= length ? length : mid_i + partition;
            merge(array + i, array + mid_i, array + mid_i, array + end_i, buffer + i);
        }
        float* tmp = buffer;
        buffer = array;
        array = tmp;
    }

    if (original == buffer)
    {
        memcpy(buffer, array, sizeof(float) * length);
        free(array);
    }
}


static void insert_to_heap(float* heap, size_t old_size, float element)
{
    heap[old_size] = element;
    float tmp;
    for (size_t i = old_size, parent = (i - 1) >> 1; i > 0 && heap[parent] < heap[i]; i = parent, parent = (i - 1) >> 1)
    {
        tmp = heap[i];
        heap[i] = heap[parent];
        heap[parent] = tmp;
    }
}

static void build_heap(float* heap, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        insert_to_heap(heap, i, heap[i]);
    }
}

static void unshift_heap(float* heap, size_t size)
{
    float tmp = heap[0];
    heap[0] = heap[size - 1];
    heap[size - 1] = tmp;

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

        tmp = heap[i];
        heap[i] = heap[child];
        heap[child] = tmp;
        i = child;
    }
}

void heap_sort(float* array, size_t length)
{
    build_heap(array, length);
    for (size_t i = length; i > 0; --i)
    {
        unshift_heap(array, i);
    }
}

void counting_sort(uint8_t* array, size_t length)
{
    size_t counts[256] = {0};
    for (size_t i = 0; i < length; ++i)
    {
        ++counts[array[i]];
    }
    for (size_t i = 0, dest = 0; i < 256; ++i)
    {
        for (size_t count = counts[i]; count > 0; --count, ++dest)
        {
            array[dest] = i;
        }
    }
}
#include <stdio.h>

void radix_sort_generic(void* input, void* output, size_t length, size_t unit_size, uint8_t (*map) (void*))
{
    size_t counts[256] = {0};
    size_t starts[256] = {0};
    size_t byte_count = length * unit_size;

    for (size_t i = 0; i < byte_count; i += unit_size)
    {
        counts[map(input + i)] += unit_size;
    }
    for (size_t i = 1; i < 256; ++i)
    {
        starts[i] = starts[i - 1] + counts[i - 1];
    }

    for (size_t i = 0; i < byte_count; i += unit_size)
    {
        size_t* start = starts + map(input + i);
        memcpy(output + *start, input + i, unit_size);
        *start += unit_size;
    }
}

static uint32_t convert_float_to_int(float* value)
{
    uint32_t i = *(uint32_t*)value;
    uint32_t sign = i >> 31;
    return (((sign ^ 1) - 1) ^ (i | 0x80000000)) + sign;
}

static uint8_t get_float_byte0(void* value)
{
    return convert_float_to_int(value);
}
static uint8_t get_float_byte1(void* value)
{
    return convert_float_to_int(value) >> 8;
}
static uint8_t get_float_byte2(void* value)
{
    return convert_float_to_int(value) >> 16;
}
static uint8_t get_float_byte3(void* value)
{
    return convert_float_to_int(value) >> 24;
}

void radix_sort(float* array, size_t length)
{
    float* buffer = malloc(sizeof(float) * length);
    radix_sort_generic(array, buffer, length, sizeof(float), get_float_byte0);
    radix_sort_generic(buffer, array, length, sizeof(float), get_float_byte1);
    radix_sort_generic(array, buffer, length, sizeof(float), get_float_byte2);
    radix_sort_generic(buffer, array, length, sizeof(float), get_float_byte3);
    free(buffer);
}