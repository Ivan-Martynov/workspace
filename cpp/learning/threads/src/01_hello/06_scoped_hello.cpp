#include "structures/scoped_thread.h"

#include <iostream>

namespace
{

// Just some simple function to run.
void do_something(int& i)
{
    if (i < 0)
    {
        i = -i;
    }
}

// Functor to use in thread construction.
struct func
{
    int& i;
    func(int& value) : i {value} {}

    void operator()()
    {
        for (int j {0}; j < 100000; ++j)
        {
            do_something(i);
        }
        std::cout << "Func struct: operation finished.\n";
    }
};

} // namespace

int main()
{
    int state {0};
    // Use a ScopedThread class to move and "autojoin" the thread.
    Marvin::ScopedThread {std::thread {func {state}}};

    return 0;
}
