#include <cstdlib>
#include <cstdint>

using std::size_t;
using std::int8_t;
using std::uint8_t;

template<typename T, size_t S>
void randomize_array(T (&array)[S])
{
    for (size_t i = 0; i < S; ++i)
    {
        array[i] = rand() - RAND_MAX / 2;
    }
}

template<size_t S>
void randomize_array(float (&array)[S])
{
    for (size_t i = 0; i < S; ++i)
    {
        array[i] = rand() / float(RAND_MAX) * 2 - 1;
    }
}

template<typename T, size_t S>
void print_array(T (&array)[S])
{
    for (size_t i = 0; i < S; ++i)
    {
        std::cout << array[i] << "\n";
    }
}

template<size_t S>
void print_array(uint8_t (&array)[S])
{
    for (size_t i = 0; i < S; ++i)
    {
        std::cout << int(array[i]) << "\n";
    }
}

template<size_t S>
void print_array(int8_t (&array)[S])
{
    for (size_t i = 0; i < S; ++i)
    {
        std::cout << int(array[i]) << "\n";
    }
}

template<typename T, size_t S>
constexpr size_t array_size(T (&)[S])
{
    return S;
}
