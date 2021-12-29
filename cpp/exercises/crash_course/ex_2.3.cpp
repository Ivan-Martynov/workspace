#include <iostream>

using namespace std;

void swap_int(int &a, int &b)
{
    int c = a;
    a = b;
    b = c;
}

void swap_pointers(int *a, int *b)
{
    int c = *a;
    *a = *b;
    *b = c;
}

void swap_ref_ptr(int *&a, int *&b)
{
    int *c = a;
    a = b;
    b = c;
}

int main()
{
    int a = 5, b = 3;

    cout << "a = " << a << ", b = " << b << endl;
    swap_int(a, b);
    cout << "a = " << a << ", b = " << b << endl;

    int *p = &a;
    int *q = &b;

    cout << "p = " << p << ", q = " << q << endl;
    cout << "*p = " << *p << ", *q = " << *q << endl;
    swap_pointers(p, q);
    cout << "*p = " << *p << ", *q = " << *q << endl;
    cout << "p = " << p << ", q = " << q << endl;

    cout << "*p = " << *p << ", *q = " << *q << endl;
    swap_ref_ptr(p, q);
    cout << "*p = " << *p << ", *q = " << *q << endl;



    return 0;
}
