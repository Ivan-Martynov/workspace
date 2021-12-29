#include <iostream>

using namespace std;

class Foo
{
    public:

        int x_;

        Foo(int a = 0) : x_(a)
        {
            // x_ = a;
            cout << "Foo default constructor called" << endl;
        }

        Foo(const Foo &other) : x_(other.x_)
        {
            // x_ = other.x_;
            cout << "Foo copy constructor called" << endl;
        }

        Foo &operator = (const Foo &other)
        {
            cout << "Foo operator = called" << endl;

            // Gracefully handle sefl assignment
            if (this == &other)
                return *this;

            x_ = other.x_;
            return *this;
        }

        ~Foo(void)
        {
            cout << "Foo destructor called" << endl;
        }
};

int main()
{
    Foo f(5);
    cout << "f.x_ = " << f.x_ << endl;

    Foo f2(f);
    cout << "f2.x_ = " << f2.x_ << endl;

    Foo f3 = f;
    cout << "f3.x_ = " << f3.x_ << endl;

    return 0;
}
