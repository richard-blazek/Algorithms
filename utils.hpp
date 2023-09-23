#ifndef ARRAY_UTILS_H_INCLUDED
#define ARRAY_UTILS_H_INCLUDED

#include <cstdlib>
#include <cstdint>
#include <utility>
#include <type_traits>

using std::size_t;
using std::int8_t;
using std::uint8_t;

template<typename T, size_t S>
void fill_with_random(T (&array)[S])
{
    for (size_t i = 0; i < S; ++i)
    {
        array[i] = rand() - RAND_MAX / 2;
    }
}

template<size_t S>
void fill_with_random(float (&array)[S])
{
    for (size_t i = 0; i < S; ++i)
    {
        array[i] = rand() / float(RAND_MAX) * 2 - 1;
    }
}

template<typename U, typename C>
void print_many_as(C& container)
{
    for (auto it = std::begin(container), end = std::end(container); it != end; ++it)
    {
        std::cout << U(*it) << "\n";
    }
}

template<typename C>
void print_many(std::enable_if_t<std::is_same_v<decltype(*std::begin(*(C*)0)), uint8_t>, C>& container)
{
    print_many_as<int>(container);
}

template<typename C>
void print_many(std::enable_if_t<std::is_same_v<decltype(*std::begin(*(C*)0)), int8_t>, C>& container)
{
    print_many_as<int>(container);
}

template<typename C>
void print_many(C& container)
{
    print_many_as<decltype(*std::begin(*(C*)0))>(container);
}

#endif