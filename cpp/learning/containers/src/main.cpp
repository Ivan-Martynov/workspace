#include "ArrayAlloc.h"
#include "ArrayFixed.h"
#include "SinglyLinkedList.h"

#include <forward_list>
#include <iostream>
#include <chrono>
#include <memory>
#include <stack>
#include <array>
#include <set>
#include <cmath>

namespace {

template <typename T>
void print_array(const T& array)
{
    //std::cout << "Length = " << array.length() << "\n";
    //              << "; size in bytes = " << array.size_in_bytes() << "\n";
    // if (array.empty())
    //{
    //    std::cout << "Empty array";
    //}
    for (auto x : array) {
        std::cout << x << ", ";
    }

    std::cout << "\n";
}

[[maybe_unused]]
Marvin::ArrayAlloc<int> fun()
{
    // Most likely copy elision will be performed.
    Marvin::ArrayAlloc arr {Marvin::init_container_with_size, 4, 5};
    return arr;
}

[[maybe_unused]]
Marvin::ArrayAlloc<int> clone_and_double(const Marvin::ArrayAlloc<int>& arr)
{
    Marvin::ArrayAlloc<int> res {
        Marvin::init_container_with_size, arr.capacity()};
    for (int i {0}; i < arr.length(); ++i) {
        res[i] = arr[i] << 1;
    }

    return res;
}

[[maybe_unused]]
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

    Marvin::ArrayFixed<int, 3> empty {};
    empty[0] = 3;
    print_array(empty);

    Marvin::ArrayFixed<float, 7> array_01 {
        Marvin::init_container_with_size, 5.3f};
    *(array_01.data() + 3) = 2.38f;
    print_array(array_01);

    const Marvin::ArrayFixed<long double, 7> array_02 {
        Marvin::init_container_with_size, 27.083};
    print_array(array_02);

    Marvin::ArrayFixed<int, 4> array_03 {};
    int i {0};
    for (auto it {array_03.rbegin()}; it != array_03.crend(); ++it)
    {
        *it = ++i;
    }
    Marvin::ArrayFixed<int, 4> array_04 {};
    print_array(array_03);
    print_array(array_04);
    Marvin::swap(array_04, array_03);
    print_array(array_03);
    print_array(array_04);
    if (array_03 == array_04) {
        std::cout << "array_03 equals array_04\n";
    } else {
        std::cout << "array_03 does not equal array_04\n";
    }
    array_03 = array_04;
    if (array_03 == array_04) {
        std::cout << "array_03 equals array_04\n";
    } else {
        std::cout << "array_03 does not equal array_04\n";
    }
}

[[maybe_unused]]
void test_sll()
{
    std::cout << "\nTesting SLL.\n";

    print_array(
        Marvin::SinglyLinkedList<int> {Marvin::init_container_with_size, 4});

    Marvin::SinglyLinkedList<float> list_01 {
        Marvin::init_container_with_size, 4, 2.0f};
    print_array(list_01);
    auto it {list_01.begin()};
    for (int i {0}; i < list_01.length(); ++i)
    {
        *it++ = i + 1;
    }
    //list_01.front() = 8;
    print_array(list_01);
    //++it;
    //list_01.pop_front();
    it = list_01.begin();
    it = list_01.erase_after(it, std::next(it, 2));
    list_01.pop_front();
    list_01.push_front(7);
    list_01.emplace_front(17.0f);
    //list_01.sort();
    if (list_01.empty()) {
        std::cout << "Empty list.\n";
    }
    //*it = 5;
    print_array(list_01);
    list_01.clear();
    print_array(list_01);
    //    const auto it {list_01.insert_after(list_01.begin(), 3.0f)};

#if 1
    Marvin::ArrayAlloc<std::string> str_array {"One", "Two", "Three"};
    Marvin::SinglyLinkedList<std::string> str_list {
        Marvin::init_container_with_size, 8, "help"};
    std::ostringstream stream {};
    stream << "Next";
    str_list.emplace_front(std::string_view {"Front"});
    str_list.emplace_front(stream.str().c_str());
    //str_list.erase_after(str_list.cbegin(), str_list.cend());
    //str_list.pop_front();
    // std::next(str_list.cbegin(), 3), std::next(str_list.cbegin(), 6));
    print_array(str_list);
    Marvin::SinglyLinkedList<std::string> list_03 {str_array.cbegin(), str_array.cend()};
    print_array(list_03);
    auto list_02 {Marvin::SinglyLinkedList<std::string> {"Using", "initializer", "list"}};
    print_array(list_02);
    list_03.assign(8, "Val");
    print_array(Marvin::SinglyLinkedList<std::string> {list_03});
    str_list = list_03;
    str_list.emplace_front("Emplacing");
    auto it_03
        = str_list.insert_after(std::next(str_list.cbegin(), 3), "Insert 1");
    std::string s {"Insert 2"};
    it_03 = str_list.insert_after(it_03, s);
    it_03 = str_list.insert_after(it_03, 3, "Insert 3");
    it_03 = str_list.insert_after(it_03, "Insert 4");
    it_03 = str_list.insert_after(
        it_03, std::next(list_02.cbegin(), 1), std::next(list_02.cbegin(), 3));
    it_03 = str_list.insert_after(it_03, {
                                             "After 1",
                                             "After 2",
                                             "After 3",
                                             "After 4",
                                         });
    it_03 = str_list.emplace_after(it_03, "Insert 5");
    // str_list.sort();
    std::cout << "Before swap:\n";
    print_array(str_list);
    std::cout << str_list.length() << "\n";
    print_array(list_02);
    std::cout << list_02.length() << "\n";
    swap(list_02, str_list);
    std::cout << "After swap:\n";
    print_array(str_list);
    std::cout << str_list.length() << "\n";
    print_array(list_02);
    std::cout << list_02.length() << "\n";
    std::cout << "Before splice:\n";
    print_array(str_list);
    std::cout << str_list.length() << "\n";
    print_array(list_02);
    std::cout << list_02.length() << "\n";
    list_02.splice_after(list_02.cbegin(), str_list);
    std::cout << "After splice:\n";
    print_array(str_list);
    std::cout << str_list.length() << "\n";
    print_array(list_02);
    std::cout << list_02.length() << "\n";

    list_02.splice_after(list_02.cbegin(), {"Insert 6", "Insert 7"});
    print_array(list_02);
#endif
}
 
struct TreeNodeInt {
    int value;
#if 0
    std::unique_ptr<TreeNodeInt> left;
    std::unique_ptr<TreeNodeInt> right;
#else
    TreeNodeInt* left;
    TreeNodeInt* right;
#endif

    TreeNodeInt(int v, TreeNodeInt* l, TreeNodeInt* r)
        : value {v}, left {l}, right {r}
    {
    }

    explicit TreeNodeInt(int v) : TreeNodeInt {v, nullptr, nullptr} {}
};

[[maybe_unused]]
void traverse_tree_inorder_recursive(
    TreeNodeInt* node_ptr, std::vector<TreeNodeInt*>& target)
{
    if (node_ptr) {
        traverse_tree_inorder_recursive(node_ptr->left, target);
        target.push_back(node_ptr);
        traverse_tree_inorder_recursive(node_ptr->right, target);
    }
}

[[maybe_unused]]
std::vector<TreeNodeInt*> traverse_tree_inorder_iterative(TreeNodeInt* root)
{
    std::stack<TreeNodeInt*> stack {};
    std::vector<TreeNodeInt*> res {};
    auto node_ptr {root};
    while (node_ptr || !stack.empty()) {
        while (node_ptr) {
            stack.push(node_ptr);
            node_ptr = node_ptr->left;
        }
        node_ptr = stack.top();
        stack.pop();
        res.push_back(node_ptr);
        node_ptr = node_ptr->right;
    }

    return res;
}

[[maybe_unused]]
std::vector<TreeNodeInt*> traverse_tree_inorder_morris(TreeNodeInt* root)
{
    std::vector<TreeNodeInt*> res {};

    auto* node_ptr {root};
    while (node_ptr) {
        if (auto* left_ptr {node_ptr->left}; !left_ptr) {
            res.push_back(node_ptr);
            node_ptr = node_ptr->right;
        } else {
            while (left_ptr->right && (left_ptr->right != node_ptr)) {
                left_ptr = left_ptr->right;
            }

            if (left_ptr->right) {
                left_ptr->right = nullptr;
                res.push_back(node_ptr);
                node_ptr = node_ptr->right;
            } else {
                left_ptr->right = node_ptr;
                node_ptr = node_ptr->left;
            }
        }
    }

    return res;
}

[[maybe_unused]]
void test_tree()
{
    std::set<int> set_01 {};
    std::cout << "Testing tree.\n";
    auto* root {new TreeNodeInt {1}};
    root->left = new TreeNodeInt {2};
    root->right = new TreeNodeInt {3};
    //std::cout << root->value << "\n";
    std::vector<TreeNodeInt*> values {traverse_tree_inorder_morris(root)};
    //std::vector<TreeNodeInt*> values {traverse_tree_inorder_iterative(root)};
    //traverse_tree_inorder_recursive(root, values);
    //print_array(values);
    for (auto& v : values)
    {
        std::cout << v->value << ' ';
        delete v;
    }
    std::cout << '\n';
}

} // namespace

int main()
{
    //test_array_alloc();
    //test_array_fixed();

    if (3 > 5) {
    }

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
    //test_tree();
    test_sll();

    return 0;
}