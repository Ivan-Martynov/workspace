#include <iostream>
#include <thread>

namespace
{

// Function to be passed to thread's constructor.
void hello()
{
    std::cout << "Hello, concurrent world!\n";
} 

} // namespace

int main()
{
    // Construct a thread from a function.
    std::thread t {hello};
    if (t.joinable())
    {
        t.join();
    }

    return 0;
}
