#include "Vec.h"
#include "Mat.h"
#include <iostream>
#include <cassert>

using Vec3i = Vec<3, int>;
using Vec3d = Vec<3, double>;
using Vec3f = Vec<3, float>;
using Vec4i = Vec<4, int>;
using Vec4f = Vec<4, float>;

void test_vec() {
    Vec3i vi1;
    constexpr Vec3i vi2 {std::array<int, 3> {1, 2, 3}};
    constexpr Vec3d vf1 {std::array<double, 3> {1.4, 2.6, 3.5}};
    std::cout << vi1 + vi2 << '\n';
    std::cout << vi2 + vf1 << '\n';
    std::cout << (vi2 + vf1) * 2 / 3 << '\n';
    std::cout << vi2 / 3.0 << '\n';
    std::cout << vi2 * vf1 << '\n';
    Vec4f vf3 {7.7};
    std::cout << vf3.norm() << '\n';
    std::cout << vf3.normalize() << '\n';
    std::cout << vf3 << '\n';
    const Vec4i vi3 {vf3};
    //const Vec3i vi4 {1, 2, 3};
    std::cout << vi3 << '\n';
    std::cout << (vi2 ^ vf1) << '\n';
}

void test_vec2() {
    // testing initializing vectors
    Vec<3, int> vi1 (5);
    static_assert(vi1.size() == 3, "wrong size");
    std::cout << "vi1 = " << vi1 << '\n';
    constexpr Vec<2, double> vd1 {std::array<double, 2> {2.7, 9.1}};
    std::cout << "vd1 = " << vd1 << '\n';
    Vec<4, char> vc1 {'a', 'b', 'c', 'd'};
    std::cout << "vc1 = " << vc1 << '\n';
    Vec<5, long> vl1;
    std::cout << "vl1 = " << vl1 << '\n';
    Vec<3, float> vf1 {1.4, 3.5};
    std::cout << "vf1 = " << vf1 << '\n';
    Vec3i vi2 {vi1};
    Vec3i vi3 = vi2;
    std::cout << "vi3 = " << vi3 << '\n';
    Vec3d vd2 {vi3};
    std::cout << "vd2 = " << vd2 << '\n';
    Vec<3, long> vl2 {vf1};
    std::cout<< "vl2 = " << vl2 << '\n';
    Vec<3, int> vi4 = vd1;
    std::cout<< "vi4 = " << vi4 << '\n';
    Vec<2, float> vf2 = vc1;
    std::cout << "vf2 = " << vf2 << '\n';
    std::cout << "vi1 + vf1 = " << vi1 + vf1 << '\n';
    std::cout << "vd1 + 7 = " << vd1 + 7 << '\n';
    std::cout << "vi1 - Vec<3, double> {4.8, 2.7, 3.1} = " << vi1 -
        Vec<3, double> {4.8, 2.7, 3.1} << '\n';
    std::cout << "Vec<4, float> {1.5, 2.5, 3.5, 4.5} * 2 = " <<
        Vec<4, float> {1.5, 2.5, 3.5, 4.5} * 2 << '\n';
    std::cout << "Vec<4, int> {10, 20, 30, 40} / 4.0 = " <<
        Vec<4, int> {10, 20, 30, 40} / 4.0 << '\n';
    const Vec<3, double> vd3 {0.5, 2.4, 3.8};
    std::cout << "vd3 = " << vd3 << '\n';
    auto sd = vi1 * vd3;
    int sint {vi1 * vi1};
    std::cout << "dot product vi1 * vd3 = " << sd << '\n';
    std::cout << "dot product vi1 * vi1 = " << sint << '\n';
    double vi_norm {vi1.norm()};
    std::cout << "vi1.norm() = " << vi_norm << '\n';
    std::cout << "vi1 normalized = " << vi1.normalize() << '\n';
    std::cout << "vi1 = " << vi1 << '\n';
    std::cout << "vd3.norm() = " << vd3.norm() << '\n';
    std::cout << "vd3 normalized = " << vd3.normalize() << '\n';
    std::cout << "vd3 = " << vd3 << '\n';
    Vec<4, double> vd4 {1.5, 2.4, 3.9, 4.7};
    std::cout << "vd4 = " << vd4 << '\n';
    vd4 *= 2;
    std::cout << "vd4 *= 2: " << vd4 << "; vd4.norm() = " << vd4.norm() << '\n';
    normalize(vd4);
    std::cout << "vd4 = " << vd4 << '\n';
    std::cout << "vi1 x vd3 = " << (vi1 ^ vd3) << '\n';
    std::cout << "vi1 += vd3: " << (vi1 += vd3) << '\n';
}

void test_vec3() {
    // check init
    Vec<2, double> vd1 {std::array<double, 2> {2.7, 9.1}};
    static_assert(vd1.size() == 2, "wrong size");
    if (vd1 != Vec<2, double> {2.7, 9.1}) std::cout << "wrong vector\n";
    constexpr Vec<2, float> vf1 {std::array<float, 2> {3.0, 4.0}};
    constexpr Vec<2, int> vi1 {std::array<int, 2> {3, 4}};
    if (vf1 != vi1) std::cout << "wrong vector\n";
    // check normalization
    assert(vi1.norm() == 5);
    Vec<2, int> vi1_norm {vi1.normalize()};
    if (vi1_norm != Vec<2, int> {0, 0}) std::cout << "wrong vector\n";
    Vec<2, double> vf1_norm {vf1.normalize()};
    if (vf1_norm != Vec<2, float> {0.6, 0.8}) std::cout << "wrong vector\n";
    // check compound operators
    Vec<2, float> vf2 {1.5, 3.2};
    Vec<2, char> vc1 {4, 8};
    vf2 += vc1;
    vc1 *= 7.5;
    vc1 += vd1;
    std::cout << vf2 << '\n';
    std::cout << vc1 << '\n';
    std::cout << +vc1.x() << ' ' << +vc1.y() << ' ' << +vc1.z() << ' '
        << +vc1.w() << '\n';
    std::cout << vf2.x() << ' ' << vf2.y() << ' ' << vf2.z() << ' '
        << vf2.w() << '\n';
    std::cout << vf1.x() << ' ' << vf1.y() << ' ' << vf1.z() << ' '
        << vf1.w() << '\n';
    Vec<3, int> vi2;
    std::cout << "Enter 3 ints:\n";
    std::cin >> vi2;
    std::cout << vi2 << '\n';
}

void test_mat() {
    // using standard (round) brackets for filling the matrix
    Mat<2, 2, int> mi1 (7);
    if (mi1 != Mat<2, 2, int>(7))
        std::cout << "wrong matrix\n";
    Mat<2, 2, float> mf1 (7.3);
    mf1.at(1).at(1) = 4.9; mf1[0].at(1) = 2.8;
    mf1[0][0] = 5.3; mf1.at(1)[0] = 6.4;
    std::cout << "mf1 size: " << mf1.size() << '\n';
    Vec<2, int> mi1_size {mi1.size()};
    if (mi1_size != Vec<2, int>(2))
        std::cout << "wrong matrix size\n";
    Mat<3, 2, int> mi2 {std::array<std::array<int, 2>, 3> {
        std::array<int, 2> {12, 19}, std::array<int, 2> {14, 15},
            std::array<int, 2> {15, 11}}};
    std::cout << "mi2 size: " << mi2.size() << '\n';
    Mat<3, 4, double> md1 {{1.5, 2.4, 9.6}, {3.7, 8.2}, {7.5}};
    std::cout << md1 << '\n';
    Mat<3, 4, int> mi3 {md1};
    std::cout << mi3 << '\n';
    Mat<2, 3, int> mi4 = md1;
    std::cout << mi4 << '\n';
    Mat<5, 2, float> mf2 (Vec<2, float> {1.3, 5.9});
    std::cout << mf2 << '\n';
    Mat<2, 2, int> mi5 (Vec<2, float> {1.3, 5.9} + 0.5);
    std::cout << mi5 << '\n';
    Mat<4, 2, double> md2 {1.5, 2.4, 3.7, 9.8};
    if ((md2 += 3.4) != Mat<4, 2, double>{4.9, 5.8, 7.1, 13.2})
        std::cout << "compound plus failed\n";
    Mat<2, 3, int> mi6 {{2, 5, 8}, {4, 3, 9}};
    if ((mi6 += Vec<2, int>{4, 7}) != Mat<2, 3, int>{{6, 9, 12}, {11, 10, 16}})
        std::cout << "compound plus failed\n";
    Mat<2, 2, double> md3 {{2.5, 5.4}, {4.2, 3.7}};
    if ((md3 += Mat<2, 2, int> {{8, 1}, {3, 4}}) !=
            Mat<2, 2, double>{{10.5, 6.4}, {7.2, 7.7}})
        std::cout << "compound plus failed\n";
    Mat<2, 2, int> mi7 {{2, 8}, {3, 7}};
    std::cout << (mi7 *= 5.0) << '\n';
    std::cout << (mi7 /= 3) << '\n';
    std::cout << mi7.col(1) << '\n';
    //Mat<2, 2, int> mi8;
    //std::cout << "Enter 4 ints for a 2x2 matrix:\n";
    //std::cin >> mi8;
    //std::cout << mi8 << '\n';
}

void test_mat_arithmetic() {
    Mat<2, 3, int> mi1 {{1, 2, 3}, {4, 5, 6}}, mi2 {{8, 5, 4}, {6, 7, 2}};
    Mat<2, 3, double> md1 {{1.5, 2.5, 4.5}, {0.2, 6.4, 8.6}};
    auto ma1 = mi1 + mi2 + md1;
    Mat<2, 3, int> mi3 = mi1 + mi2 + md1;
    std::cout << ma1 << '\n';
    std::cout << mi3 << '\n';
    auto ma2 = mi1 * 3 + mi2 * 1.5 + md1 / 4;
    std::cout << ma2 << '\n';
    Vec<3, double> vd1 {8.5, 5.4, 2.7};
    std::cout << mi1 << '\n';
    std::cout << vd1 << '\n';
    std::cout << "mi1 * vd1 = " << mi1 * vd1 << '\n';
    Mat<3, 4, int> mi4 {{-2, 3, -8, 5}, {4, 7, -9, 3}, {0, -5, 4, 1}};
    std::cout << mi4 << '\n';
    std::cout << "mi1 * mi4 = " << mi1 * mi4 << '\n';
    auto m_eye1 = eye<4>();
    std::cout << m_eye1 << '\n';
}

int main() {

    //test_vec();
    //test_vec2();
    //test_vec3();
    //test_mat();
    test_mat_arithmetic();

    return 0;
}

