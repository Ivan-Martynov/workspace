#include <iostream>

void cheers(int);      // prototype, no return value
double cube(double x); // prototype, return value is double

int main(void)
{
    using namespace std;
    cheers(5);
    cout << "enter number: ";
    double side;
    cin >> side;
    double volume = cube(side);
    cout << side << " in cubes = ";
    cout << volume << " cubic feet.\n";
    int y = 2;
    cheers(cube(y));
    return 0;
}

void cheers(int n)
{
    using namespace std;
    for (int i = 0; i < n; ++i)
	cout << "Thank you! ";
    cout << endl;
}

double cube(double x)
{
    return x * x * x;
}
