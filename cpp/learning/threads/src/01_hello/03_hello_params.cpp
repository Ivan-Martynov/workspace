#include <iostream>
#include <thread>

namespace
{

// Function with a parameter to be passed to thread's constructor.
void hello(int i, const std::string& s)
{
    std::cout << "Got integer " << i << ", and string " << s << ".\n";
} 

} // namespace

int main()
{
    std::string s {"Tesing string"};
    // Construct a thread from a function and a parameter. The parameter is a
    // const reference and the reference should be provided via a
    // reference_wrapper, which can be obtained with helper function ref/cref.
    std::thread t {hello, 3, std::cref(s)};
    if (t.joinable())
    {
        t.join();
    }

    return 0;
}
