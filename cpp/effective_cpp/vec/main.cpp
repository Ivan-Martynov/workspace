#include <iostream>
#include "Vec.h"
#include "Algebra_lib.h"

using namespace Algebra_lib;

// manual instantiation: might be useful to reduce compliation time
template class Algebra_lib::Vec<3, int>;
template class Algebra_lib::Vec<3, double>;
template class Algebra_lib::Vec<4, int>;
template class Algebra_lib::Vec<4, double>;

// shorthand names for common types
using Vec3i = Vec<3, int>;
using Vec3d = Vec<3, double>;
using Vec3f = Vec<3, float>;
using Vec4i = Vec<4, int>;
using Vec4d = Vec<4, double>;
using Vec4f = Vec<4, float>;

//template <class T>
//constexpr auto sum_elems(const T& t) noexcept { return t; }
//
//template <class ...T>
//constexpr auto sum_elems(T... args) noexcept {
//    auto res = 0;
//    std::cout << sizeof...(args) << '\n';
//    //for (const auto& x: args)
//    //    std::cout << x << ' ';
//    return res + sum_elems(std::forward<T>(args)...);
//}

void test_vec() {
    // testing initializing vectors
    Vec3i vi1 {2, -8, 5};
    //static_assert(vi1.size() == 3, "wrong size");
    vi1[1] = 7;
    Vec3i vi2 {4, -1, 3};
    //auto vi2_arr = vi2.array();
    std::cout << vi1 << "; " << vi2 << '\n';
    std::cout << "dot(vi1, vi2) = " << vi1.dot(vi2) << '\n';
    std::cout << "dot(vi1, vi1) = " << vi1.dot(vi1) << '\n';
    std::cout << "norm(vi1) = " << vi1.norm() << '\n';

    Vec4d vd1 {2.3, -9.4, 2.8};
    vd1.at(0) = 2.5;
    std::cout << vd1 << '\n';
    std::cout << "vd1 norm = " << vd1.norm();
    vd1.normalize();
    std::cout << "; normalized: " << vd1 << '\n';
    for (auto &x: vd1)
        x *= 2;
    std::cout << vd1 << '\n';

    std::cout << std::boolalpha;
    std::cout << std::is_convertible<double, int>::value << '\n';
    std::cout << (std::is_trivially_constructible<double, int>::value &&
        std::is_convertible<int, double>::value) << '\n';

    constexpr Vec3d vd2 {std::array<double, 3> {2.4, 8, -3.7}};
    std::cout << "vd2 = " << vd2 << '\n';
    std::cout << "vd2 = " << vd2.at(2) << '\n';

    constexpr Vec4d vd3 {};
    constexpr auto vd3_arr = vd3.array();
    std::cout << "vd3 = " << vd3 << '\n';
    std::cout << "vd3[1] = " << vd3_arr.at(1) << '\n';

    Vec4d vd4 (48.91);
    std::cout << "vd4 = " << vd4 << '\n';
}

void test_sum() {
    //sum_elems(1, 2, 3);
}

int main() {

    test_vec();
    //test_sum();

    return 0;
}

