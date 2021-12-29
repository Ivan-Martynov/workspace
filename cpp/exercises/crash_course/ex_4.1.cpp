#include <iostream>

using namespace std;

class Foo
{
    public:

        Foo(void) : _x(0)
        {
            cout << "Foo default constructor called" << endl;
        }

        Foo(int a) : _x(a)
        {
            cout << "Foo constructor called" << endl;
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

        virtual ~Foo(void)
        {
            cout << "Foo destructor called" << endl;
        }

    private:
        int _x;
};

class Bar : public Foo
{
    public:

        Bar(void) {
            cout << "Bar default constructor called" << endl;
        }

        Bar(int a) : Foo(a) {
            cout << "Bar constructor called" << endl;
        }

        ~Bar() {
            cout << "Bar destructor called" << endl;
        }
};

int main()
{
    Bar * b = new Bar(5);

    cout << *b << endl;

    delete b;

    Foo *f = new Foo(7);

    cout << *f << endl;

    Foo *c = new Bar(3);

    cout << *c << endl;

    delete f;
    delete c;

    return 0;
}
