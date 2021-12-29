#include <iostream>

using namespace std;

class Foo
{
    public:

        Foo(int a = 0) : _x(a)
        {
            cout << "Foo default constructor called" << endl;
        }

        Foo(const Foo &other) : _x(other._x)
        {
            cout << "Foo copy constructor called" << endl;
        }

        Foo &operator = (const Foo &other)
        {
            cout << "Foo operator = called" << endl;

            // Gracefully handle sefl assignment
            if (this == &other)
                return *this;

            _x = other._x;
            return *this;
        }

        friend istream &operator >> (istream &input, Foo & other)
        {
            return input >> other._x;
        }

        friend ostream &operator << (ostream &output, Foo & other)
        {
            return output << "x = " << other._x;
        }

        ~Foo(void)
        {
            cout << "Foo destructor called" << endl;
        }

    private:
        int _x;
};

int main()
{
    Foo f(5);

    cout << f << endl;

    cout << "Enter new value for object f: " << endl;
    cin >> f;
    cout << f << endl;

    return 0;
}
