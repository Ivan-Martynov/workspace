#include <iostream>

int main()
{
    using namespace std;

    double *p3 = new double[3];
    p3[0] = 0.2;
    p3[1] = 0.5;
    p3[2] = 0.8;

    cout << "p3[1] = " << p3[1] << ".\n";
    p3++;
    cout << "Now p3[0] = " << p3[0] << " and ";
    cout << "p3[1] = " << p3[1] << ".\n";
    p3--;
    cout << "Now p3[0] = " << p3[0] << " and ";
    cout << "p3[1] = " << p3[1] << ".\n";
    delete [] p3;

    return 0;
}

