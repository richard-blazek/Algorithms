#include <iostream>
#include "sort.hpp"


int main()
{
    srand(time(0));

    int items[40];
    for (int& item: items)
    {
        item = rand();
    }

    quick_sort(items, items + 40);
    for (int item: items)
    {
        std::cout << item << "\n";
    }

    std::cout << "\n";
    return 0;
}