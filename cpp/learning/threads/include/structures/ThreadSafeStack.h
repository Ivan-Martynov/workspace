#ifndef H_INCLUDE_STRUCTURES_THREADSAFESTACK_H_H
#define H_INCLUDE_STRUCTURES_THREADSAFESTACK_H_H

#include <stack>
#include <mutex>
#include <memory>
#include <exception>

namespace Marvin
{

struct EmptyStack : std::exception
{
    const char* what() const throw();
};

template <typename T>
class ThreadSafeStack
{
    using value_type = T;

  public:
    explicit ThreadSafeStack() {}

    explicit ThreadSafeStack(const ThreadSafeStack& other)
    {
        // The actual copy is done in the constructor's body with a lock.
        std::lock_guard guard {other.m_mutex};
        m_data = other.m_data;
    }

    ThreadSafeStack& operator=(const ThreadSafeStack& other) = delete;

    void push(const value_type& value)
    {
        std::lock_guard guard {other.m_mutex};
        m_data.push(value);
    }

    std::shared_ptr<value_type> pop()
    {
        std::lock_guard guard {other.m_mutex};

        // Throw an exception if the stack is empty before trying to pop.
        if (m_data.empty())
        {
            throw EmptyStack {};
        }

        // Allocate the return value before modifying the stack.
        const auto result {std::make_shared<value_type>(m_data.top())};

        m_data.pop();

        return result;
    }

    void pop(value_type& value)
    {
        std::lock_guard guard {other.m_mutex};

        // Throw an exception if the stack is empty before trying to pop.
        if (m_data.empty())
        {
            throw EmptyStack {};
        }

        value = m_data.top();

        m_data.pop();
    }

    bool empty() const
    {
        std::lock_guard guard {other.m_mutex};
        return m_data.empty();
    }

  private:
    std::stack<value_type> m_data {};
    mutable std::mutex m_mutex {};
};

} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_THREADSAFESTACK_H_H
