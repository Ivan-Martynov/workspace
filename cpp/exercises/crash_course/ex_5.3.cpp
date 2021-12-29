#include <iostream>
#include <stdexcept>

using namespace std;

class DivideByZero: public runtime_error
{
    public:

        DivideByZero(void) : runtime_error("DivideByZero") { }
};

double divide(const double a, const double b)
{
    if (b == 0)
        throw DivideByZero();

    return a / b;
}

int main(void)
{
    cout << divide(5, 3) << endl;;
    cout << divide(2.7, 0) << endl;

    return 0;
}
