#include <iostream>

using namespace std;

class Functor
{
    public:

        Functor(const int value) : _value(value) { }
        int operator()(const int value) {
            return _value + value;
        }

    private:
        int _value;
};

int main(void)
{
    Functor f(1);

    cout << "f(3) = " << f(3) << endl;

    return 0;
}
