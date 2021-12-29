#include <iostream>

int main()
{
    int i1 = 1;
    int i2 = 2;

    int const *p1 = &i1;
    int *const p2 = &i1;
    int const * const p3 = &i1;

    p1 = &i2;    // OK
    // *p1 = 2;  // WRONG

    // p2 = &i2;  // WRONG
    *p2 = 2;      // OK

    // p3 = &i2;  // WRONG
    // *p3 = 2;   // WRONG

    // To have no warning at compilation for unused variables
    if( p3 == p2 ) { };
    if( p1 == p2 ) { };

    return 0;
}
