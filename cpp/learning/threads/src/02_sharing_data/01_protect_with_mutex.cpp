#include <vector>
#include <list>
#include <algorithm>
#include <mutex>
#include <thread>
#include <functional>
#include <iostream>

namespace
{

// Global data and mutex.
std::list<int> list {};
std::mutex mutex {};

void add_to_list(int value)
{
    std::lock_guard<std::mutex> guard {mutex};
    std::cout << "Adding element " << value << "\n";
    list.push_back(value);
}

bool list_contains(int value)
{
    std::lock_guard<std::mutex> guard {mutex};
    return std::find(list.cbegin(), list.cend(), value) != list.cend();
}

bool is_sorted()
{
    if (list.empty())
    {
        return true;
    }

    auto next {std::next(list.begin())};
    while (next != list.cend())
    {
        if (*std::prev(next) > *next)
        {
            return false;
        }
        next = std::next(next);
    }

    return true;
}

} // namespace

int main()
{
    auto add_range_lambda {[](int start, int end)
        {
            for (int i {start}; i < end; ++i)
            {
                add_to_list(i);
            }
        }};
    auto check_range_lambda {[](int start, int end)
        {
            for (int i {start}; i < end; ++i)
            {
                if (list_contains(i))
                {
                    return true;
                }
            }
            return false;
        }};

    constexpr int thread_count {4};
    std::vector<std::thread> add_threads(thread_count);
    std::vector<std::thread> check_threads(thread_count);
    for (int i {0}; i < thread_count; ++i)
    {
        add_threads[i] = std::thread(add_range_lambda, i * 10, i * 10 + 10);
        check_threads[i] = std::thread(check_range_lambda, i * 10, i * 10 + 10);
    }
    std::ranges::for_each(add_threads, std::mem_fn(&std::thread::join));
    std::ranges::for_each(check_threads, std::mem_fn(&std::thread::join));
    std::ranges::for_each(list, [](auto x) { std::cout << x << " "; });
    std::cout << "\nis sorted? = " << std::boolalpha << is_sorted()
              << std::noboolalpha << "\n";

    return 0;
}
