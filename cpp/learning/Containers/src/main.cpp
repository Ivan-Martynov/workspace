#include "array_alloc.h"

#include <iostream>

template <typename T>
static void print_array(const Marvin::array_alloc<T>& array)
{
    std::cout << "Length = " << array.length()
              << "; capacity = " << array.capacity()
              << "; size in bytes = " << array.size_in_bytes() << "\n";
    for (int i {0}; i < array.length(); ++i)
    {
        std::cout << array[i] << ", ";
    }

    std::cout << "\n";
}

static Marvin::array_alloc<int> fun()
{
    // Most likely copy elision will be performed.
    Marvin::array_alloc arr(4, 5);
    return arr;
}

static Marvin::array_alloc<int> clone_and_double(
    const Marvin::array_alloc<int>& arr)
{
    Marvin::array_alloc<int> res(arr.capacity());
    for (int i {0}; i < arr.length(); ++i)
    {
        res[i] = arr[i] << 1;
    }

    return res;
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

    array = Marvin::array_alloc(8, 2);
    print_array(array);

    print_array(fun());

    auto arr_01 {std::move(array)};
    print_array(arr_01);

    auto arr_02 {clone_and_double(arr_01)};
    arr_02.push_back(-78);
    arr_02.emplace_back(78);
    print_array(arr_02);

    Marvin::array_alloc arr_03 {8, 2};
    print_array(arr_03);
    arr_03 = {-32, 48, 1, -5};
    print_array(arr_03);
}

int main()
{
    test_array_alloc();

    return 0;
}