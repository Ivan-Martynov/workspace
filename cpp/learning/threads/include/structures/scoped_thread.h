#ifndef H_INCLUDE_STRUCTURES_SCOPED_THREAD_H_H
#define H_INCLUDE_STRUCTURES_SCOPED_THREAD_H_H

#include <thread>

namespace Marvin
{

class ScopedThread
{
  public:
    explicit ScopedThread(std::thread t) : m_thread {std::move(t)}
    {
        if (!m_thread.joinable())
        {
            throw std::logic_error("No thread");
        }
    }

    ~ScopedThread() { m_thread.join(); }

  private:
    std::thread m_thread;

    explicit ScopedThread(const ScopedThread&) = delete;
    ScopedThread& operator=(const ScopedThread&) = delete;
};

} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_SCOPED_THREAD_H_H
