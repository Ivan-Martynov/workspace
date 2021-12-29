#include <iostream>

using namespace std;

void foo(void);

class Foo
{
    public:

        Foo(void)
        {
            ::foo();
            foo();
        }

        void foo(void)
        {
            cout << "Foo:foo() class method called" << endl;
        }
};

void foo(void)
{
    cout << "Function foo called" << endl;
}

int main()
{
    Foo *f = new Foo();

    // f->foo();

    delete f;

    return 0;
}
