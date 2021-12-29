#include <iostream>
#include <cassert>
#include "Vec.h"
#include "Mat.h"

using namespace Algebra_lib;

// manual instantiation: might be useful to reduce compliation time
template class Vec<3, int>;
template class Vec<3, double>;
template class Vec<4, int>;
template class Vec<4, double>;
template class Mat<4, 4, double>;

// shorthand names for common types
using Vec3i = Vec<3, int>;
using Vec3d = Vec<3, double>;
using Vec3f = Vec<3, float>;
using Vec4i = Vec<4, int>;
using Vec4d = Vec<4, double>;
using Vec4f = Vec<4, float>;
using Mat4d = Mat<4, 4, double>;

// viewport matrix
inline Mat<4, 4, double> viewport(const int xx, const int yy,
        const int w, const int h, const int d) {
    Mat<4, 4, double> m {eye<4>()};
    const double half_w {w / 2.0}, half_h {h / 2.0}, half_d {d / 2.0};
    m[0][0] = half_w; m[1][1] = half_h; m[2][2] = half_d;
    m[0][3] = xx + half_w; m[1][3] = yy + half_h; m[2][3] = half_d;
    return m;
}

void test_vec() {
    // testing initializing vectors
    // default ctor
    constexpr Vec<3, int> vi1 {};
    static_assert(vi1.size() == 3, "wrong size");
    std::cout << "vi1 = " << vi1 << '\n';
    // ctor with std::array (double)
    constexpr Vec<2, double> vd1 {std::array<double, 2> {2.7, 9.1}};
    static_assert(vd1.size() == 2, "wrong size");
    std::cout << "vd1 = " << vd1 << '\n';
    // ctor with initializer_list (char)
    Vec<4, char> vc1 {'a', 'b', 'c', 'd'};
    std::cout << "vc1 = " << vc1 << ":";
    for (const auto c: vc1) // little test for auto and char output
        std::cout << ' ' << char(c);
    std::cout << '\n';
    Vec<5, long> vl1 {12000, 2345 * 1983, -45000 * 25, 29, -7};
    std::cout << "vl1 = " << vl1 << '\n';
    Vec<3, float> vf1 {1.4, 3.5}; // trailing part is filled with default (0)
    std::cout << "vf1 = " << vf1 << '\n';
    // copy ctor
    Vec3i vi2 {vi1};
    // assignment operator
    Vec3i vi3 = vi2;
    std::cout << "vi3 = " << vi3 << '\n';
    // explicit element-wise conversion from int to double
    //Vec3d vd2 {double(vi3.x()), double(vi3.y()), double(vi3.z())};
    Vec3d vd2 {vi3};
    std::cout << "vd2 = " << vd2 << '\n';
    // explicit narrowing from float to long
    Vec<3, long> vl2 {long(vf1.x()), long(vf1.y()), long(vf1.z())};
    std::cout<< "vl2 = " << vl2 << '\n';
    // test resize()
    auto vi4 = resize<4>(Vec<2, int>{2, 7}, 4);
    std::cout<< "vi4 = " << vi4 << '\n';
    auto vd3 = resize<2>(Vec<5, double> {1.3, 8.4, 2.9, -4.2, -0.7});
    std::cout<< "vd3 = " << vd3 << '\n';
    Vec<2, double> vd4 {resize<2>(vc1)};
    std::cout << "vd4 = " << vd4 << '\n';
    // test arithmetics
    std::cout << "vd4 + vd1 = " << vd4 + vd1 << '\n';
    std::cout << "vi4 + Vec4d {1.2, 2.5, 3.8, 3.7} = " <<
        static_cast<Vec4d>(vi4) + Vec4d {1.2, 2.5, 3.8, 3.7} << '\n';
    std::cout << "vd1 + 7.0 = " << vd1 + 7.0 << '\n';
    std::cout << "Vec<3, double>(vi1) - Vec<3, double> {4.8, 2.7, 3.1} = " <<
        Vec<3, double>(vi1) - Vec<3, double> {4.8, 2.7, 3.1} << '\n';
    std::cout << "Vec<4, float> {1.5, 2.5, 3.5, 4.5} * 2 = " <<
        Vec<4, float> {1.5, 2.5, 3.5, 4.5} * 2.0f << '\n';
    std::cout << "Vec4d(Vec<4, int> {10, 20, 30, 40}) / 4.0 = " <<
        Vec4d(Vec<4, int> {10, 20, 30, 40}) / 4.0 << '\n';
    const Vec<3, double> vd5 {0.5, 2.4, 3.8};
    std::cout << "vd5 = " << vd5 << '\n';
    // test fill
    vi2.fill(3);
    std::cout << "vi2 = " << vi2 << '\n';
    const auto sd = Vec3d(vi2) * vd5;
    const auto sint = dot(vi2, vi2);
    std::cout << "element-wise product vi2 * vd5 = " << sd << '\n';
    std::cout << "dot product: dot(vi2, vi2) = " << sint << '\n';
    const auto vi_norm = vi2.norm();
    std::cout << "vi2.norm() = " << vi_norm << '\n';
    std::cout << "vi2 normalized = " << vi2.normalize() << '\n';
    std::cout << "vd5.norm() = " << vd5.norm() << '\n';
    std::cout << "vd5 normalized = " << vd5.normalize() << '\n';
    auto temp = vd5.normalize();
    std::cout << temp << '\n';
    std::cout << "cross product: vd1 x vd5 = " <<
        cross(resize<3>(vd1), vd5) << '\n';

    // check init
    Vec<2, double> vd6 {std::array<double, 2> {2.7, 9.1}};
    static_assert(vd6.size() == 2, "wrong size");
    if (vd6 != Vec<2, double> {2.7, 9.1}) std::cout << "wrong vector\n";
    constexpr Vec<2, float> vf3 {std::array<float, 2> {3.0, 4.0}};
    constexpr Vec<2, int> vi5 {std::array<int, 2> {3, 4}};
    if (vf3 != vi5) std::cout << "wrong vector\n";
    // check normalization
    assert(vi5.norm() == 5);
    Vec<2, int> vi5_norm {vi5.normalize()};
    try {
        std::cout << vi5_norm.z() << '\n';
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: Z coordinate: " << e.what() << '\n';
    }
    if (vi5_norm != Vec<2, int> {0, 0})
        std::cout << "wrong vector\n";
    Vec<2, double> vf3_norm {vf3.normalize()};
    if (vf3_norm != Vec<2, float> {0.6, 0.8})
        std::cout << "wrong vector\n";
    // check compound operators
    Vec<2, double> vd7 {1.5, 3.2};
    Vec<2, char> vc2 {4, 8};
    vc2 *= 7.5; // warning: truncated value (7.5 -> 7)
    vd7 += Vec<2, double>(vc2);
    std::cout << vd7 << '\n';
    std::cout << vc2 << '\n';
    try {
        std::cout << +vc2.x() << ' ' << +vc2.y() << ' ' << +vc2.z() << ' '
            << +vc1.w() << '\n';
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: coordinate: " << e.what() << '\n';
    }
    Vec<3, int> vi6;
    std::cout << "Enter 3 integer values:\n";
    std::cin >> vi6;
    std::cout << vi6 << '\n';
}

void test_mat() {
    // test default ctor
    Mat4d md1;
    // test fill
    md1.fill(1);
    std::cout << "md1 = " << md1 << '\n';
    // test fill_col
    md1.fill_col(1, Vec4d {1.1, 2.2, 3.3, 4.4});
    std::cout << "md1.fill_col(1, Vec4d {1.1, 2.2, 3.3, 4.4}) = " << md1 <<
        '\n';
    // test fill_row
    md1.fill_row(3, Vec4d {0.9, -4.7, 5.3, -7.1});
    std::cout << "md1.fill_row(3, Vec4d {0.9, -4.7, 5.3, -7.1}) = " << md1 <<
        '\n';
    // test compound plus
    std::cout << "md1 += 3 = " << (md1 += 3) << '\n';
    // test operator[] and at()
    md1.at(0).at(3) = 4.9;
    md1[1].at(2) = 2.8;
    md1[0][2] = 5.3;
    md1.at(1)[3] = 6.4;
    std::cout << "md1 = " << md1 << '\n';
    std::cout << "md1 / 7.0 = " << md1 / 7.0 << '\n';
    // test filling
    Mat<3, 3, int> mi1;
    mi1.fill_each_row(Vec3i {2, 4, 8});
    std::cout << "mi1 = " << mi1 << '\n';
    Mat<3, 3, int> mi2;
    mi2.fill_each_col(Vec3i {1, 3, 9});
    std::cout << "mi2 = " << mi2 << '\n';
    // test matrix difference
    std::cout << "mi1 - mi2 = " << mi1 - mi2 << '\n';
    std::cout << "mi1 - mi2 - mi1 * 2 = " << mi1 - mi2 - mi1 * 2 << '\n';
    // test multiplication matrix by a vector
    Vec3i vi1 {5, 10, 1};
    std::cout << "vi1 = " << vi1 << '\n';
    std::cout << "mi2 * vi1 = " << mi2 * vi1 << '\n';
    // test ctor with std::array
    Mat<3, 2, int> mi3 {std::array<std::array<int, 2>, 3> {
        std::array<int, 2> {12, 19}, std::array<int, 2> {14, 15},
            std::array<int, 2> {15, 11}}};
    std::cout << "mi3 = " << mi3 << '\n';
    // test size()
    std::cout << "mi3 size: " << mi3.size() << '\n';
    // test ctor with std::array
    Mat<2, 4, int> mi4 {Vec4i{2, 5, 8, 4}, Vec4i{4, 3, 9, 1}};
    std::cout << "mi4 = " << mi4 << '\n';
    // test matrix multiplication
    std::cout << "mi3 * mi4 = " << mi3 * mi4 << '\n';
    // test assignment
    auto mi5 = mi4;
    std::cout << "mi5 = " << mi5 << '\n';
    std::cout << "mi5 *= 2 = " << (mi5 *= 2) << '\n';
    auto mi6 = mi4.transpose();
    std::cout << "mi6 = " << mi6 << '\n';
    auto md2 = eye<3>();
    std::cout << "md2 = " << md2 << '\n';
    std::cout << "det3(md2) = " << det(md2) << '\n';
    std::cout << "det3(mi1 - mi2) = " << det(mi1 - mi2) << '\n';
    std::cout << "det4(eye<4>() * 4) = " << det(eye<4>() * 4) << '\n';
    Mat<4, 4, int> mi7 {Vec4i {1, 2, 4, 6}, Vec4i {0, 1, 3, 5},
        Vec4i {2, 0, 1, 0}, Vec4i {4, 7, 9, 1}};
    std::cout << "mi7 = " << mi7 << '\n';
    std::cout << "det(mi7) = " << det(mi7) << '\n';
    std::cout << "det_recursive(2, 5, 4, 3) = " << det_recursive(Mat<2, 2, int>
            {Vec<2, int>{2, 5}, Vec<2, int>{4, 3}}) << '\n';
    assert(det(mi7) == -58);
    constexpr int hil_n {4};
    Mat<hil_n, hil_n, double> m_hilbert_1;
    for (int i {0}; i < hil_n; ++i)
        for (int j {0}; j < hil_n; ++j)
            m_hilbert_1[i][j] = 1.0 / (i + j + 1);
    std::cout << "m_hilbert_1 = " << m_hilbert_1 << '\n';
    std::cout << "1 / det(m_hilbert_1) = " << 1 / det_recursive(m_hilbert_1) <<
        '\n';
    auto m_view = viewport(1, 2, 3, 4, 5);
    std::cout << m_view << '\n';
}

int main() {

    test_vec();
    // test_mat();

    return 0;
}

