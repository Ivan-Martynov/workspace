#include <iostream>

int main()
{
    using namespace std;
    
    int *pt = new int; //allocate space for int
    *pt = 1001;	// assign value
    cout << "int value = " << *pt << ": place = " << pt << endl;

    double  *pd = new double;
    *pd = 100000001.0;
    cout << "double value = " << *pd << ": place = " << pd << endl;

    cout << "pt size = " << sizeof(pt);
    cout << ": *pt size = " << sizeof(*pt) << endl;
    cout << "pd size = " << sizeof(pd);
    cout << ": *pd size = " << sizeof(*pd) << endl;
    return 0;
}

