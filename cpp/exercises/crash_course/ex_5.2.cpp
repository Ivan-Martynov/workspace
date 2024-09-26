#include <exception>
#include <iostream>
#include <typeinfo>

using namespace std;

class Foo
{
  public:
    virtual ~Foo() = default;
    virtual void method(void) {}
};

class Bar : public Foo
{
};

void unexpected(void)
{
    throw;
}

void function(void) // throw(int, bad_exception)
{
    throw 'x';
}

int main(void)
{
    // Bad alloc
    try
    {
        int* array = new int[1];
        int i = 0;
        i = array[0];
        cout << i << endl;
    }
    catch (bad_alloc& e)
    {
        cerr << "bad_alloc caught: " << e.what() << endl;
    }

    // Bad cast
    try
    {
        Foo f;
        // Bar & b = dynamic_cast<Bar &>(f);
    }
    catch (bad_cast& e)
    {
        cerr << "bad_cast caught: " << e.what() << endl;
    }

    // Bad exception
    // set_unexpected(unexpected);

    try
    {
        function();
    }
    catch (int)
    {
        cerr << "caught int" << endl;
    }
    catch (const std::exception& e)
    {
        cerr << "bad_exception caught: " << e.what() << endl;
    }

    return 0;
}
