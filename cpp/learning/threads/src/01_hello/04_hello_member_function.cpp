#include <iostream>
#include <thread>

namespace
{

// Just a simple class.
class X
{
  public:
    // Member function to be passed to thread's constuctor.
    void do_work() const
    {
        std::cout << "Class X instance pretends doing some work.\n";
    }
};

} // namespace

int main()
{
    X x {};
    // Constructing a thread from member function and the instance pointers.
    std::thread t {&X::do_work, &x};
    if (t.joinable())
    {
        t.join();
    }

    return 0;
}
