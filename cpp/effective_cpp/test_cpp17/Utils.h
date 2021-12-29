#ifndef UTILS_H
#define UTILS_H

// chrono for measuring time (e.g., function execution time)
#include <chrono>

#define BOOST_FILESYSTEM_NO_DEPRECATED

namespace log_measurement {

// measuring time for a function
auto time_func = [](size_t n, auto&& f, auto&&... pars) {
    using namespace std::chrono;
    auto t = high_resolution_clock::now();
    // call function n times
    for (size_t i {0}; i < n; ++i)
        std::forward<decltype(f)>(f)(std::forward<decltype(pars)>(pars)...);
    return high_resolution_clock::now() - t;
};

// positive modulo (i.e., avoiding negative results)
constexpr int modulo(const int val, const int mod_base) noexcept {
    return (val % mod_base + mod_base) % mod_base;
}

// helper method for calculating squared value
//template <class T, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
template <class T>
constexpr T sqr(const T val) noexcept { return val * val; }


} // surface_testing namespace

#endif // UTILS_H
