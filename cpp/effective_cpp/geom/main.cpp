#include <iostream>
#include "Geometry.h"

using namespace Geometry_lib;

void test_point() {
    Point p1 {};
    std::cout << p1.x() << ", " << p1.y() << '\n';
}

int main() {

    test_point();

    return 0;
}

