#include "sort.h"
#include <string.h>

static size_t partition(float *array, size_t length)
{
    size_t pivot = 0;
    for (size_t i = 1; i < length; ++i)
    {
        if (array[i] < array[pivot])
        {
            float tmp = array[pivot];
            array[pivot] = array[i];
            array[i] = array[pivot + 1];
            array[pivot + 1] = tmp;
            ++pivot;
        }
    }
    return pivot;
}

void quick_sort(float *array, size_t length)
{
    size_t pivot = partition(array, length);
    if (pivot > 1)
    {
        quick_sort(array, pivot);
    }
    if (length - pivot - 1 > 1)
    {
        quick_sort(array + pivot + 1, length - pivot - 1);
    }
}

static void merge(float *dst, float *src1, float *src2, float *end1, float *end2)
{
    while (src1 < end1 && src2 < end2)
    {
        *dst++ = *src1 < *src2 ? *src1++ : *src2++;
    }
    while (src1 < end1)
    {
        *dst++ = *src1++;
    }
    while (src2 < end2)
    {
        *dst++ = *src2++;
    }
}

static size_t min(size_t i, size_t j)
{
    return i < j ? i : j;
}

void merge_sort(float *array, size_t length)
{
    float *tmp = malloc(sizeof(float) * length);
    int in_tmp = 0;
    for (size_t sz = 1; sz < length; sz *= 2)
    {
        float *src = in_tmp ? tmp : array, *dst = in_tmp ? array : tmp;
        for (size_t i = 0; i < length; i += 2 * sz)
        {
            merge(dst + i, src + i, src + i + sz, src + min(i + sz, length), src + min(i + 2 * sz, length));
        }
        in_tmp = !in_tmp;
    }
    if (in_tmp)
    {
        memcpy(array, tmp, sizeof(float) * length);
    }
    free(tmp);
}

static void swap(float *x, float *y)
{
    float tmp = *x;
    *x = *y;
    *y = tmp;
}

static void heap_insert(float *heap, size_t length, float value)
{
    heap[length] = value;
    for (size_t i = length; i > 0 && heap[i] > heap[(i - 1) / 2]; i = (i - 1) / 2)
    {
        swap(&heap[i], &heap[(i - 1) / 2]);
    }
}

static float heap_remove(float *heap, size_t length)
{
    --length;
    float result = heap[0];
    heap[0] = heap[length];

    for (size_t i = 0;;)
    {
        size_t left = i * 2 + 1, right = i * 2 + 2;
        size_t next = left < length && right < length && heap[right] > heap[left] ? right : left;
        if (next >= length || heap[i] >= heap[next])
        {
            break;
        }
        swap(&heap[i], &heap[next]);
        i = next;
    }
    return result;
}

void heap_sort(float *array, size_t length)
{
    for (size_t i = 0; i < length; ++i)
    {
        heap_insert(array, i, array[i]);
    }
    for (size_t i = 0; i < length; ++i)
    {
        array[length - i - 1] = heap_remove(array, length - i);
    }
}

void counting_sort(uint8_t *array, size_t length)
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

void radix_sort_generic(void *input, void *output, size_t length, size_t unit_size, uint8_t (*map)(void *))
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
        size_t *start = starts + map(input + i);
        memcpy(output + *start, input + i, unit_size);
        *start += unit_size;
    }
}

static uint32_t convert_float_to_int(float *value)
{
    uint32_t i = *(uint32_t *)value;
    uint32_t sign = i >> 31;
    return (((sign ^ 1) - 1) ^ (i | 0x80000000)) + sign;
}

static uint8_t get_float_byte0(void *value)
{
    return convert_float_to_int(value);
}
static uint8_t get_float_byte1(void *value)
{
    return convert_float_to_int(value) >> 8;
}
static uint8_t get_float_byte2(void *value)
{
    return convert_float_to_int(value) >> 16;
}
static uint8_t get_float_byte3(void *value)
{
    return convert_float_to_int(value) >> 24;
}

void radix_sort(float *array, size_t length)
{
    float *buffer = malloc(sizeof(float) * length);
    radix_sort_generic(array, buffer, length, sizeof(float), get_float_byte0);
    radix_sort_generic(buffer, array, length, sizeof(float), get_float_byte1);
    radix_sort_generic(array, buffer, length, sizeof(float), get_float_byte2);
    radix_sort_generic(buffer, array, length, sizeof(float), get_float_byte3);
    free(buffer);
}