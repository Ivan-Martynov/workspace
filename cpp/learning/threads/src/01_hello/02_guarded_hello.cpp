#include "structures/thread_guard.h"

#include <iostream>

namespace
{

// Function to be passed to thread's constructor.
void guarded_hello()
{
    std::cout << "Hello, concurrent world! En garde!\n";
} 

} // namespace

int main()
{
    // Construct a thread from a function.
    std::thread t {guarded_hello};
    // Use a ThreadGuard class to "autojoin" the thread.
    Marvin::ThreadGuard{t};

    return 0;
}
