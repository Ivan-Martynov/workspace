#include <iostream>
#include "Quaternion.h"

void test_quaternion() {
    Quaternion q;
    q += Quaternion {3, 4, 7, 8};
    q *= Quaternion {1, 2, 3, 4};

    std::cout << q << '\n';
}

int main()
{
    test_quaternion();
    
    return 0;
}
