#include <iostream>

using namespace std;
const int ArraySize = 16; // example of external declaration

int main()
{
    double factorials[ArraySize];
    factorials[0] = factorials[1] = 1.0;
    for (int i = 2; i < ArraySize; i++)
    {
	factorials[i] = i * factorials[i - 1];
	cout << i << "! = " << factorials[i] << endl;
    }
    return 0;
}
