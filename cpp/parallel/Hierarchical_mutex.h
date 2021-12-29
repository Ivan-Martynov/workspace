#ifndef HIERARCHICAL_MUTEX_H
#define HIERARCHICAL_MUTEX_H

#include <mutex>
#include <thread>

class Hierarchical_mutex {
public:
    explicit Hierarchical_mutex(unsigned long v): hierarchy_value_{v},
             prev_hierarchy_value_{0} { }

    Hierarchical_mutex(const Hierarchical_mutex&) = default;
    Hierarchical_mutex& operator=(const Hierarchical_mutex&) = default;
    Hierarchical_mutex(Hierarchical_mutex&&) = default;
    Hierarchical_mutex& operator=(Hierarchical_mutex&&) = default;
    ~Hierarchical_mutex() = default;

    void lock() {
        check_for_hierarchy_violation();
        internal_mutex_.lock();
        update_hierarchy_value();
    }

    void unlock() {
        this_thread_hierarchy_value_ = prev_hierarchy_value_;
        internal_mutex_.unlock();
    }

    bool try_lock() {
        check_for_hierarchy_violation();
        if (!internal_mutex_.try_lock())
            return false;
        update_hierarchy_value();
        return true;
    }

private:
    std::mutex internal_mutex_;
    unsigned long const hierarchy_value_;
    unsigned long prev_hierarchy_value_;
    static thread_local unsigned long this_thread_hierarchy_value_;

    void check_for_hierarchy_violation() {
        if (this_thread_hierarchy_value_ <= hierarchy_value_)
            throw std::logic_error {"mutex hierarchy violated"};
    }

    void update_hierarchy_value() {
        prev_hierarchy_value_ = this_thread_hierarchy_value_;
        this_thread_hierarchy_value_ = hierarchy_value_;
    }
};

thread_local unsigned long Hierarchical_mutex::this_thread_hierarchy_value_(
        std::numeric_limits<unsigned long>::max());

#endif /* HIERARCHICAL_MUTEX_H */

