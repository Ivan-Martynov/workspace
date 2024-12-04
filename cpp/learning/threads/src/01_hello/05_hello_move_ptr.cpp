#include <iostream>
#include <memory>
#include <thread>

namespace
{

// Just a simple class.
class X
{
  public:
    void do_work() const
    {
        std::cout << "Class X instance pretends doing some work via a unique "
                     "pointer.\n";
    }
};

// Function with a parameter to be passed to thread's constructor.
static void process_object(std::unique_ptr<X> x_ptr)
{
    x_ptr->do_work();
}

} // namespace

int main()
{
    auto ptr {std::make_unique<X>()};

    // Construct a thread from a function and a parameter. The parameter is a
    // unique pointer which must be moved. Thus moving a unique_ptr.
    std::thread t {process_object, std::move(ptr)};
    if (t.joinable())
    {
        t.join();
    }

    return 0;
}
