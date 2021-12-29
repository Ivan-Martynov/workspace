#include <iostream>

using namespace std;

int main()
{
    unsigned char age = 65;
    cout << static_cast<unsigned>(age) << endl;
    cout << static_cast<void const *>(&age) <<endl;

    double f = 3.14159;
    cout.unsetf(ios::floatfield);
    cout.precision(5);
    cout << f << endl;

    cout.precision(10);
    cout << f << endl;
    cout.setf(ios::fixed, ios::floatfield);
    cout << f << endl;

    cout << "Enter a number or -1 to quit: ";
    int i = 0;

    while (cin >> i) {
        if (i == -1)
            break;
        cout << "You entered " << i << '\n';
    }

    return 0;
}
