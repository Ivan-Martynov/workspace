#ifndef THREAD_RAII_H
#define THREAD_RAII_H

#include <thread>

/*
 * Class to represent a thread as a RAII object
 */
class Thread_RAII {
public:
    // enum for specifying whether the thread should join or detach in dtor
    enum class Dtor_action { join, detach };

    Thread_RAII(std::thread&& t, Dtor_action a): t_{std::move(t)}, a_{a} { }

    // explicitly forbid copying
    Thread_RAII(const Thread_RAII&) = delete;
    Thread_RAII& operator=(const Thread_RAII&) = delete;

    // use default moving
    Thread_RAII(Thread_RAII&&) = default;
    Thread_RAII& operator=(Thread_RAII&&) = default;

    // custom dtor controlling joining or detaching the thread
    ~Thread_RAII() {
        if (t_.joinable())
            (a_ == Dtor_action::join) ? t_.join() : t_.detach();
    }

    std::thread& get() { return t_; }

private:
    std::thread t_;
    Dtor_action a_;
};

#endif /* THREAD_RAII_H */

