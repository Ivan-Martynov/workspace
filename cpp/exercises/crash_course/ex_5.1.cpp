#include <iostream>
#include <stdexcept>

using namespace std;

class FooException : public runtime_error
{
    public:

        FooException(void) : runtime_error("FooException") { };
};

class Foo
{
    public:

        Foo(void) {
            cout << "calling constructor" << endl;
            throw new FooException;
        }

        virtual ~Foo(void) {
            cout << "calling destructor" << endl;
        }
};

int main(void)
{
    Foo *foo = new Foo;

    delete foo;

    return 0;
}
