#ifndef H_INCLUDE_STRUCTURES_THREAD_GUARD_H_H
#define H_INCLUDE_STRUCTURES_THREAD_GUARD_H_H

#include <thread>

namespace Marvin
{

class ThreadGuard
{
  public:
    explicit ThreadGuard(std::thread& t) : m_thread {t}
    {
        if (!m_thread.joinable())
        {
            throw std::logic_error("No thread");
        }
    }

    ~ThreadGuard() { m_thread.join(); }

    explicit ThreadGuard(const ThreadGuard&) = delete;
    ThreadGuard& operator=(const ThreadGuard&) = delete;

  private:
    std::thread& m_thread;
};

} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_THREAD_GUARD_H_H
