#include "array_alloc.h"

#include <iostream>

template <typename T>
static void print_array(const Marvin::array_alloc<T>& array)
{
    for (int i {0}; i < array.length(); ++i)
    {
        std::cout << array[i] << ", ";
    }

    std::cout << "\n";
}

static void test_array_alloc()
{
    Marvin::array_alloc<int> empty_array {};

    print_array(empty_array);
    print_array(Marvin::array_alloc<int>(3));

    Marvin::array_alloc array(3, 7);
    array[1] = 4;
    print_array(array);
    array.resize(2);
    print_array(array);
    array.resize(5);
    print_array(array);
    array.reserve(15);
    print_array(array);
}

int main()
{
    test_array_alloc();

    return 0;
}