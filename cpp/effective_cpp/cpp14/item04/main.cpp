/*
 * Makefile may use -std=c++14 as an option
 */

#include <iostream>
#include <vector>
#include <typeinfo>

//using boost for type info
//#include <boost/type_index.hpp>

// a way to display types via a compiler: not to define the class
template <class T>
class Type_display;

template <class C>
void print_array(const C& c) {
    for (const auto& x: c)
        std::cout << x << ' ';
    std::cout << '\n';
}

void test_array() {
    //using boost::typeindex::type_id_with_cvr;

    const int val = 42;
    auto x = val;
    auto y = &val;

    // using compiler to identify the types: works because the class
    // Type_display is declared but not defined
    //Type_display<decltype(x)> xType;
    //Type_display<decltype(y)> yType;

    // output the types of the variables
    std::cout << typeid(x).name() << '\n';
    std::cout << typeid(y).name() << '\n';

    // using boost
    //std::cout << "x: " << type_id_with_cvr<decltype(x)>().pretty_name() << '\n';
    //std::cout << "y: " << type_id_with_cvr<decltype(y)>().pretty_name() << '\n';
}

int main() {

    test_array();

    return 0;
}

