#include "sort.hpp"

#include <iostream>

int main()
{
    srand(time(0));

    int items[4];
    for (int& item: items)
    {
        item = rand();
    }

    quick_sort(items, items + 4);
    for (int item: items)
    {
        std::cout << item << "\n";
    }

    std::cout << "\n";
    std::cout << "Steps: " << steps << "\n";
    return 0;
}