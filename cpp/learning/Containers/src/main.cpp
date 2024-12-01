#include "array_alloc.h"
#include "array_fixed.h"

#include <iostream>

template <typename T>
static void print_array(const T& array)
{
//    std::cout << "Length = " << array.length()
//              << "; size in bytes = " << array.size_in_bytes() << "\n";
    //if (array.empty())
    //{
    //    std::cout << "Empty array";
    //}
    for (auto x : array)
    {
        std::cout << x << ", ";
    }

    std::cout << "\n";
}

static Marvin::ArrayAlloc<int> fun()
{
    // Most likely copy elision will be performed.
    Marvin::ArrayAlloc arr(4, 5);
    return arr;
}

static Marvin::ArrayAlloc<int> clone_and_double(
    const Marvin::ArrayAlloc<int>& arr)
{
    Marvin::ArrayAlloc<int> res(arr.capacity());
    for (int i {0}; i < arr.length(); ++i)
    {
        res[i] = arr[i] << 1;
    }

    return res;
}

static void test_array_alloc()
{
    std::cout << "\nTesting ArrayAlloc.\n";
    Marvin::ArrayAlloc<int> empty_array {};

    print_array(empty_array);
    print_array(Marvin::ArrayAlloc<int>(3));

    Marvin::ArrayAlloc array(3, 7);
    array[1] = 4;
    print_array(array);
    array.resize(2);
    print_array(array);
    array.resize(5);
    print_array(array);
    array.reserve(15);
    print_array(array);

    array = Marvin::ArrayAlloc(8, 2);
    print_array(array);

    print_array(fun());

    auto arr_01 {std::move(array)};
    print_array(arr_01);

    auto arr_02 {clone_and_double(arr_01)};
    arr_02.push_back(-78);
    arr_02.emplace_back(78);
    print_array(arr_02);

    Marvin::ArrayAlloc arr_03 {8, 2};
    print_array(arr_03);
    arr_03 = {-32, 48, 1, -5};
    print_array(arr_03);
}

[[maybe_unused]]
static void test_array_fixed()
{
    std::cout << "\nTesting ArrayFixed.\n";

    Marvin::ArrayFixed<int, 3> emtpy;
    print_array(emtpy);

    Marvin::ArrayFixed<float, 7> array_01(5.3f);
    print_array(array_01);

    Marvin::ArrayFixed<long double, 7> array_02(27.083);
    print_array(array_02);

    Marvin::ArrayFixed<int, 4> array_03 {{2, 4, 5}};
    print_array(array_03);
    print_array(Marvin::ArrayFixed<double, 4>  {2.3, 4.0, 5, 7.3, 9});
    print_array(Marvin::ArrayFixed<double, 4>  {2.5, 4.1, 5.03, 9.8});
}

int main()
{
    test_array_alloc();
    test_array_fixed();

    return 0;
}