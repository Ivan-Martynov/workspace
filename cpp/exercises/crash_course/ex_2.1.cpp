#include <iostream>

using namespace std;

int main()
{
    // pointer to a char
    char *p1 = new char;

    cout << &p1 << endl;

    // constant pointer to a char
    char * const p2 = p1;

    // pointer to a constant char
    const char *p3 = p1;

    // constant pointer to a constant char
    const char * const p4 = p2;

    // reference to a char
    char &r1 = *p2;

    cout << r1 << endl;

    // reference to a constant char
    const char &r2 = *p4;

    // avoid compilation warning
    if (*p3 == r1) {}
    if (*p4 == r2) {}

    return 0;
}
