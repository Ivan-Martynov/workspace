#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include <algorithm>
#include <functional>
#include <chrono>

template <typename Iterator, typename T>
struct accumulate_block
{
    void operator()(Iterator first, Iterator last, T& result)
    {
        result = std::accumulate(first, last, result);
    }
};

template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    using size_type = std::size_t;

    const size_type length {static_cast<size_type>(std::distance(first, last))};
    // Do nothing for an empty range.
    if (!length)
    {
        return init;
    }

    // Determine minimum block size and maximum number of threads, depending on
    // it.
    constexpr size_type min_per_thread {25};
    const size_type max_threads {
        (length - 1 + min_per_thread) / min_per_thread};

    const size_type hardware_thread_count {std::thread::hardware_concurrency()};
    // Using hardware information about the number of availabe cores and
    // choosing the minimun number of threads for calculation. If the number of
    // threads is more than the number of physical cores, then there will
    // overhead because of switching between the threads, thus decreasing the
    // perfomance.
    const size_type thread_count {
        std::min(std::max(hardware_thread_count, static_cast<size_type>(2)),
            max_threads)};

    // Number of entries per thread.
    const size_type block_size {length / thread_count};

    std::vector<T> results(thread_count, 0);
    // Launch one fewer, because there is already one thread running.
    //std::vector<std::thread> threads(thread_count - 1);
    std::vector<std::thread> threads {};
    Iterator block_start {first};
    //for (size_type i {0}; i < thread_count - 1; ++i)
    for (size_type i {0}; i < thread_count - 1; ++i)
    {
        Iterator block_end {block_start};
        std::advance(block_end, block_size);
        //threads[i] = std::thread(accumulate_block<Iterator, T>(), block_start,
        //    block_end, std::ref(results[i]));
        threads.push_back(std::thread(accumulate_block<Iterator, T>(),
            block_start, block_end, std::ref(results[i])));
        block_start = block_end;
    }
    // Process the final block.
    accumulate_block<Iterator, T>()(
        block_start, last, results[thread_count - 1]);

    // Using mem_fn to call a member function on each item.
    std::ranges::for_each(threads, std::mem_fn(&std::thread::join));

    return std::accumulate(results.cbegin(), results.cend(), init);
}

int main()
{
    constexpr std::size_t n {1000000};
    std::vector<int> vec(n);
    for (size_t i {1}; i <= n; ++i)
    {
        vec[i - 1] = i;
    }
    std::size_t count {0};
    for( size_t i {0}; i < 1000; ++i)
    {
        const auto start {std::chrono::high_resolution_clock::now()};
        // auto result {parallel_accumulate(vec.cbegin(), vec.cend(), 0)};
#if 1
        auto result {parallel_accumulate(vec.cbegin(), vec.cend(), 0)};
#else
        auto result {std::accumulate(vec.cbegin(), vec.cend(), 0)};
#endif
        if (!result)
        {
            std::cout << count << ": " << count * 1E-3 << "\n";
        }
        auto d {std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - start)};
        count += (std::chrono::high_resolution_clock::now() - start).count();
    }
    std::cout << count << ": " << count * 1E-3 << "\n";
    //std::cout << "Sum of numbers from " << 1 << " to " << n << " = " << result
    //          << " in " << " = " << d << "\n";

    return 0;
}
