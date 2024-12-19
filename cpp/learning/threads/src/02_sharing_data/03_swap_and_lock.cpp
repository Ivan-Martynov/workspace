#include <iostream>
#include <vector>
#include <mutex>

class Big
{
  public:
    Big(int n) : m_vector(n) {}

    int count() const { return static_cast<int>(m_vector.size()); }

    void swap(Big& rhs) { std::swap(m_vector, rhs.m_vector); }

  private:
    std::vector<int> m_vector;
};

void swap(Big& lhs, Big& rhs)
{
    lhs.swap(rhs);
}

class X
{
  public:
    X(const Big& big) : m_details {big} {}

    int count() const { return m_details.count(); }

    friend void swap(X& lhs, X& rhs)
    {
        if (&lhs == &rhs)
        {
            return;
        }

        // Lock two or more mutexes.
        std::lock(lhs.m_mutex, rhs.m_mutex);
        // Argument std::adopt_lock indicates the mutex is already locked, so
        // the guard only needs to adopt the ownership of the mutex instead of
        // attempting to lock it.
        std::lock_guard left_lock(lhs.m_mutex, std::adopt_lock);
        std::lock_guard right_lock(rhs.m_mutex, std::adopt_lock);
        swap(lhs.m_details, rhs.m_details);
    }

  private:
    Big m_details;
    std::mutex m_mutex {};
};


int main()
{
    Big big_01 {1000};
    Big big_02 {10000};
    X first {big_01};
    X second {big_02};
    std::cout << "Before swap: " << first.count() << " vs " << second.count()
              << "\n";
    swap(first, second);
    std::cout << "After swap: " << first.count() << " vs " << second.count()
              << "\n";

    return 0;
}