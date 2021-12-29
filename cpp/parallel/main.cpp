#include <iostream>
#include <algorithm>
#include <vector>
#include <thread>
#include <list>
#include <mutex>
#include <unistd.h>
#include <future>

// measuring time for a function
auto time_func = [](size_t n, auto&& f, auto&&... pars) {
    using namespace std::chrono;
    auto t = high_resolution_clock::now();
    // call function n times
    for (size_t i {0}; i < n; ++i)
        std::forward<decltype(f)>(f)(std::forward<decltype(pars)>(pars)...);
    return high_resolution_clock::now() - t;
};

auto num_of_threads(const size_t num_elems, const size_t min_per_thread = 25) {
    return std::min(std::max(std::thread::hardware_concurrency(), 0u),
            static_cast<unsigned int>((num_elems + min_per_thread - 1) /
                min_per_thread));
}

std::list<int> list_01;
std::mutex mutex_01;
void add_to_list(const int val) {
    std::lock_guard<std::mutex> mtx_guard {mutex_01};
    list_01.push_back(val);
}

bool list_contains(const int val) {
    std::lock_guard<std::mutex> mtx_guard {mutex_01};
    return std::find(list_01.begin(), list_01.end(), val) != list_01.end();
}

template <class Iter, class T>
struct accum_block {
    void operator()(Iter b, Iter e, T& res) {
        res = std::accumulate(b, e, res);
    }
};

template <class Iter, class T>
struct accum_block_async {
    T operator()(Iter b, Iter e, const T res) {
        return std::accumulate(b, e, res);
    }
};

template <class Iter, class T>
T par_accum(Iter b, Iter e, T init) {
    const size_t len = std::distance(b, e);
    if (len == 0)
        return init;

    const size_t min_per_thread {25};
    const size_t max_threads {(len + min_per_thread - 1) / min_per_thread};
    size_t num_threads {std::thread::hardware_concurrency()};
    num_threads = std::min(std::max(num_threads, static_cast<size_t>(0)),
            max_threads);
    const size_t block_size {len / num_threads};

    std::vector<T> res(num_threads);
    // one thread is already running...
    std::vector<std::thread> threads(num_threads - 1);

    Iter block_start = b;
    for (size_t i {0}; i < num_threads - 1; ++i) {
        Iter block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(accum_block<Iter, T>(),
            block_start, block_end, std::ref(res[i]));
        block_start = block_end;
        std::cout << "this_thread id: " << std::this_thread::get_id() << '\n';
    }
    accum_block<Iter, T>()(block_start, e, res.back());
    std::for_each(threads.begin(), threads.end(), mem_fn(&std::thread::join));
    return std::accumulate(res.begin(), res.end(), init);
}

template <class Iter, class T>
T par_sum_async(Iter b, Iter e, T init) {
    const auto len = std::distance(b, e);
    const auto num_threads = num_of_threads(len);
    const auto block_size = len / num_threads;

    std::vector<std::future<T>> tasks(num_threads - 1);
    Iter block_start = b;
    for (size_t i {0}; i < num_threads - 1; ++i) {
        Iter block_end = block_start;
        std::advance(block_end, block_size);
        tasks[i] = std::async(std::launch::async, std::accumulate<Iter, T>,
            block_start, block_end, 0);
        block_start = block_end;
    }
    T res = std::accumulate(block_start, e, init);
    for (auto& t: tasks)
        res += t.get();
    return res;
}

void hello() {
    std::cout << "Hello, Concurrent World!\n";
    sleep(2);
}

void test_1_1() {
    auto t1 = std::thread{hello};
    auto t2 = std::thread{hello};
    t1.join();
    t2.join();
}

void test_par_sum(std::vector<int>& vi, size_t init) {
    size_t sum = par_accum(vi.begin(), vi.end(), init);
    std::cout << "sum = " << sum << '\n';
}

void test_par_sum_async(std::vector<int>& vi, size_t init) {
    size_t sum = par_sum_async(vi.begin(), vi.end(), init);
    std::cout << "sum = " << sum << '\n';
}

void test_nonpar_sum(std::vector<int>& vi, size_t init) {
    size_t sum = std::accumulate(vi.begin(), vi.end(), init);
    std::cout << "sum = " << sum << '\n';
}

void test_2_8() {
    constexpr size_t num_elems {9000000}, init{20};
    std::vector<int> vi(num_elems, init);

    auto t = time_func(1, test_par_sum, vi, init);
    std::cout << std::chrono::duration<double>(t).count() << '\n';

    t = time_func(1, test_nonpar_sum, vi, init);
    std::cout << std::chrono::duration<double>(t).count() << '\n';

    t = time_func(1, test_par_sum_async, vi, init);
    std::cout << std::chrono::duration<double>(t).count() << '\n';
}

int main() {

    test_1_1();
    //test_2_8();
    //auto t = time_func(1, test_2_8);

    //std::cout << std::chrono::duration<double>(t).count() << '\n';

    //std::cout << std::thread::hardware_concurrency() <<
    //    " concurrent threads are supported\n";

    return 0;
}

