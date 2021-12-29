#include <iostream>

int main()
{
    using namespace std;
    double wages[3] = {10000.0, 20000.0, 30000.0};
    short stacks[3] = {3, 2, 1};
    // two way to obtain the array address
    double *pw = wages; // array's name is equal to address
    short *ps = &stacks[0]; // use address operation with arrays elements

    cout << "pw = " << pw << ", *pw = " << *pw << endl;
    pw++;
    cout << "add 1 to pw pointer: \n";
    cout << "pw = " << pw << ", *pw = " << *pw << "\n\n";
    cout << "ps = " << ps << ", *ps = " << *ps << "\n\n";
    ps++;
    cout << "add 1 to ps pointer: \n";
    cout << "ps = " << ps << ", *ps = " << *ps << "\n\n";
    cout << "calling two elements using array notation \n";
    cout << "stacks[0] = " << stacks[0] << ", stacks[1] = " << stacks[1] << endl;
    cout << "calling two elements using pointer notation \n";
    cout << "*stacks = " << *stacks << ", *(stacks + 1) = " << *(stacks + 1) << endl;
    cout << sizeof(wages) << " = size of array wages \n";
    cout << sizeof(pw) << " = size of pointer pw \n";

    return 0;
}

