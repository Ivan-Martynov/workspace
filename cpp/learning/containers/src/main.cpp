#include "ArrayAlloc.h"
#include "ArrayFixed.h"
#include "SinglyLinkedList.h"

#include <forward_list>
#include <iostream>
#include <chrono>

namespace
{

template <typename T>
void print_array(const T& array)
{
    std::cout << "Length = " << array.length() << "\n";
    //              << "; size in bytes = " << array.size_in_bytes() << "\n";
    // if (array.empty())
    //{
    //    std::cout << "Empty array";
    //}
    for (auto x : array)
    {
        std::cout << x << ", ";
    }

    std::cout << "\n";
}

Marvin::ArrayAlloc<int> fun()
{
    // Most likely copy elision will be performed.
    Marvin::ArrayAlloc arr {Marvin::init_container_with_size, 4, 5};
    return arr;
}

Marvin::ArrayAlloc<int> clone_and_double(const Marvin::ArrayAlloc<int>& arr)
{
    Marvin::ArrayAlloc<int> res {
        Marvin::init_container_with_size, arr.capacity()};
    for (int i {0}; i < arr.length(); ++i)
    {
        res[i] = arr[i] << 1;
    }

    return res;
}

void test_array_alloc()
{
    std::cout << "\nTesting ArrayAlloc.\n";
    Marvin::ArrayAlloc<int> empty_array {};

    print_array(empty_array);
    print_array(Marvin::ArrayAlloc<int> {Marvin::init_container_with_size, 3});

    Marvin::ArrayAlloc array {Marvin::init_container_with_size, 3, 7};
    array[1] = 4;
    print_array(array);
    array.resize(2);
    print_array(array);
    array.resize(5);
    print_array(array);
    array.reserve(15);
    print_array(array);

    array = Marvin::ArrayAlloc {Marvin::init_container_with_size, 8, 2};
    print_array(array);

    print_array(fun());

    auto arr_01 {std::move(array)};
    print_array(arr_01);

    auto arr_02 {clone_and_double(arr_01)};
    arr_02.push_back(-78);
    arr_02.emplace_back(78);
    print_array(arr_02);

    Marvin::ArrayAlloc arr_03 {Marvin::init_container_with_size, 8, 2};
    print_array(arr_03);
    Marvin::ArrayAlloc arr_04 {8, 2};
    print_array(arr_04);
}

[[maybe_unused]]
void test_array_fixed()
{
    std::cout << "\nTesting ArrayFixed.\n";

    Marvin::ArrayFixed<int, 3> emtpy {};
    print_array(emtpy);

    Marvin::ArrayFixed<float, 7> array_01 {
        Marvin::init_container_with_size, 5.3f};
    print_array(array_01);

    Marvin::ArrayFixed<long double, 7> array_02 {
        Marvin::init_container_with_size, 27.083};
    print_array(array_02);
}

void test_sll()
{
    std::cout << "\nTesting SinglyLinkedList.\n";

    print_array(
        Marvin::SinglyLinkedList<int> {Marvin::init_container_with_size, 4});

    Marvin::SinglyLinkedList<float> list_01 {
        Marvin::init_container_with_size, 4, 2.0f};
    const auto it {list_01.insert_after(list_01.begin(), 3.0f)};
    list_01.emplace_after(it, 23.0f);
    list_01.push_front(5);
    list_01.erase_after(list_01.cbegin());
#if 1
    print_array(list_01);
    Marvin::SinglyLinkedList list_02 {1.3, 2.8, 5.93, -7.89};
    print_array(list_02);
    print_array(
        Marvin::SinglyLinkedList {Marvin::init_container_with_size, 10, -7.89});
    print_array(
        Marvin::SinglyLinkedList<float> {1.35, 102.8, 508.93, -7.89, 3});
    print_array(std::move(list_02));
    print_array(list_02);
    print_array(
        Marvin::SinglyLinkedList<float> {list_01.begin(), list_01.end()});
    Marvin::SinglyLinkedList<double> list_03 {std::move(list_02)};
    print_array(list_03);
    print_array(list_02);
    list_02 = std::move(list_03);
    print_array(list_02);
    print_array(list_03);

    auto list_04 {list_01};
    list_04.push_front(1.25f);
    list_04.emplace_front(-2.328901f);
    list_04.emplace_front(-2.328901f);
    list_04.push_front(103);
    print_array(list_04);
    Marvin::SinglyLinkedList<float> list_05 {2.5f, 2.8f, 2.3f, 2.4f, 2.1f};
    list_01 = list_05;
    list_01.insert_after(list_01.begin(), 2.7f);
    list_01.emplace_after(std::next(list_01.begin()), 2.9f);
    list_01.emplace_front(2.0f);
    list_01.assign(8, 12.4f);
    list_01 = {12.4f, 89.23f, -98.81f};
    list_01.assign(std::next(list_05.begin()), list_05.end());
    print_array(list_01);
    Marvin::SinglyLinkedList<Marvin::ArrayAlloc<int>> list_06 {
        std::move(Marvin::ArrayAlloc {1, 2, 4}), {5, 9, 8}};
    list_06.emplace_front(std::initializer_list<int> {2, 0, -3, 4, 9});
    list_06.emplace_after(
        list_06.begin(), std::initializer_list<int> {2, 0, -3, 4, 9});
    std::cout << "Length of linked list with two dynamically allocated arrays "
                 "as its items = "
              << std::distance(list_06.cbegin(), list_06.cend()) << "\n";
#endif
}

} // namespace

int main()
{
    test_array_alloc();
    test_array_fixed();
    test_sll();

#if 0
    std::forward_list<float> list {1, 2, 3};
    std::forward_list<float>::iterator it = list.begin();
    //    it._M_node->_M_next = nullptr;
    list.emplace_after(it, 4);
    const float f1 {6};
    auto t {std::chrono::high_resolution_clock::now()};
    auto i1 {static_cast<double>(
        (std::chrono::high_resolution_clock::now() - t).count())};
    float f2 {i1};
    std::cout << f1 + f2 << "\n";
#endif

    return 0;
}