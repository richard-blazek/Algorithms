#ifndef SORT_H_INCLUDED
#define SORT_H_INCLUDED

#include <stdlib.h>
#include <stdint.h>

void quick_sort(float *begin, size_t length);
void merge_sort(float *array, size_t length);
void heap_sort(float *array, size_t length);
void counting_sort(uint8_t *array, size_t length);

void radix_sort_generic(void *input, void *output, size_t length, size_t unit_size, uint8_t (*map)(void *));
void radix_sort(float *array, size_t length);

#endif