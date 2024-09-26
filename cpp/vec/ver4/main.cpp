#include "Vec.h"
#include <iostream>

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

void test_vec()
{
    // testing initializing vectors
    Vec3i vi1 {2, -8, 5};
    static_assert(vi1.size() == 3, "wrong size");
    std::cout << vi1[1] << '\n';
    vi1[1] = 7;
    std::cout << vi1[1] << '\n';
}

int main()
{
    //int x;

    test_vec();

    return 0;
}
