/*******************************************************************************
 * This small library contains implementation of algorithms from the book of   *
 * Donald Knuth: The Art Of Computer Programming Volume 1                      *
 ******************************************************************************/

#ifndef ALGO_LIB_H
#define ALGO_LIB_H

#include <iostream>
#include <algorithm>
#include <chrono>

namespace Algo_lib {

// measuring time for a function
auto time_func = [](size_t n, auto&& f, auto&&... pars) {
    using namespace std::chrono;
    auto t = high_resolution_clock::now();
    // call function n times
    for (size_t i {0}; i < n; ++i)
        std::forward<decltype(f)>(f)(std::forward<decltype(pars)>(pars)...);
    return high_resolution_clock::now() - t;
};

// alias for convenience
using size_t = std::size_t;

// keeping square root of number five since it appears few times
constexpr static auto sqrt_of_5 = std::sqrt(5);
// golden ratio
constexpr static auto phi = 0.5 * (sqrt_of_5 + 1.0);
constexpr static auto golden_ratio() noexcept { return phi; }

/*******************************************************************************
 * Greatest common divisor (gcd) section                                       *
 * Contains few implementations concerning gcd                                 *
 ******************************************************************************/

// classic version of the greatest common divisor function
constexpr int gcd_v0_base(const int a, const int b) noexcept {
    if (a < b)
        return gcd_v0_base(b, a);
    const auto r = a % b;
    if (r == 0)
        return b;
    else
        return gcd_v0_base(b, r);
}
constexpr int gcd_v0(const int a, const int b) noexcept {
    using std::abs;
    return (a == 0) ? abs(b) : (b == 0) ? abs(a) : gcd_v0_base(abs(a), abs(b));
}

// constexpr implementation of the greatest common divisor function
constexpr int gcd_v1_base(const int a, const int b) noexcept {
    return (a < b) ? gcd_v1_base(b, a) : (a % b == 0) ? b :
        gcd_v1_base(b, a % b);
}
constexpr int gcd_v1(const int a, const int b) noexcept {
    using std::abs;
    //auto gcd_lambda = [c = abs(a), d = abs(b)] {
    //    return gcd_v1_base(c, d);
    //};
    return (a == 0) ? abs(b) : (b == 0) ? abs(a) : gcd_v1_base(abs(a), abs(b));
    //return (a == 0) ? abs(b) : (b == 0) ? abs(a) : gcd_lambda();
}

// implementing "effective" algorithm
constexpr int gcd_v2_base(const int a, const int b) noexcept {
    using std::abs; using std::min;
    return abs(a - b) == 0 ? b : gcd_v2_base(min(a, b), abs(a - b));
}
constexpr int gcd_v2(const int a, const int b) noexcept {
    using std::abs;
    return (a == 0) ? abs(b) : (b == 0) ? abs(a) : gcd_v2_base(abs(a), abs(b));
}

// finding gcd plus a and b, such that d = a * m + b * n
int extended_gcd(int m, int n, int& a, int& b) {
    m = std::abs(m);
    n = std::abs(n);
    if (m < n)
        return extended_gcd(n, m, b, a);
    int a_prime = b = 1;
    int b_prime = a = 0;
    int c = m, d = n;
    int q = c / d;
    while (int r = c % d) {
        c = d;
        d = r;
        int t = a_prime;
        a_prime = a;
        a = t - q * a;
        t = b_prime;
        b_prime = b;
        b = t - q * b;
        q = c / d;
    }
    return d;
}

/*******************************************************************************
 * End of Greatest common divisor (gcd) section                                *
 ******************************************************************************/



/*******************************************************************************
 * Factorial section                                                           *
 ******************************************************************************/

// loop implementation
constexpr size_t fac_v0(const size_t n) noexcept {
    size_t p {1};
    for (size_t i {2}; i <= n; ++i)
        p *= i;
    return p;
}

// recursive implementation
constexpr size_t fac_v1(const size_t n) noexcept {
    return (n < 2) ? 1 : n * fac_v1(n - 1);
}

// using templates: can accept only const parameter
template <size_t N>
constexpr size_t fac_v2() noexcept {
    return N * fac_v2<N - 1>();
}
template<> constexpr size_t fac_v2<0>() noexcept { return 1; }

/*******************************************************************************
 * End of Factorial section                                                    *
 ******************************************************************************/



/*******************************************************************************
 * Binomial coefficient section                                                *
 ******************************************************************************/

// loop implementation
constexpr size_t binom_v0(const size_t n, const size_t k) {
    if (k > n)
        return 0;
    if (k == 0 || k == n)
        return 1;
    double p {1};
    for (size_t i {1}; i <= std::min(k, n - k); ++i)
        p *= static_cast<double>(n + 1 - i) / i;
    return static_cast<size_t>(p);
}

// loop implementation: return zero on overflow
constexpr size_t binom_v1(size_t n, size_t k) noexcept {
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    size_t p {1};
    for (size_t i {1}; i <= k; ++i, --n) {
        if (p / i > std::numeric_limits<size_t>::max() / n) // overflow
            return 0;
        p = p / i * n + p % i * n /i;
    }
    return p;
}

// recursive implementation
constexpr size_t binom_v2(const size_t n, size_t k) {
    return (k > n) ? 0 : (k == 0 || n < 2) ? 1 :
        binom_v2(n - 1, std::min(k, n - k)) +
        binom_v2(n - 1, std::min(k, n - k) - 1);
}

// using templates
template <size_t N, size_t K>
struct Binom_v3_s {
    constexpr static size_t val = (K > N) ? 0 : Binom_v3_s<N - 1, K>::val +
        Binom_v3_s<N - 1, K - 1>::val;
};
template<size_t K> struct Binom_v3_s<0, K> { constexpr static size_t val = 1; };
template<size_t N> struct Binom_v3_s<N, 0> { constexpr static size_t val = 1; };
template<size_t N> struct Binom_v3_s<N, N> { constexpr static size_t val = 1; };
template<> struct Binom_v3_s<0, 0> { constexpr static size_t val = 1; };

template <size_t N, size_t K>
constexpr size_t binom_v3() {
    return Binom_v3_s<N, K>::val;
}

/*******************************************************************************
 * End of Binomial coefficient section                                         *
 ******************************************************************************/



/*******************************************************************************
 * Harmonic number section                                                     *
 ******************************************************************************/

// loop implementation
constexpr auto harmonic_v0(const size_t n) noexcept {
    double h {0.0};
    for (size_t i {1}; i <= n; ++i)
        h += 1.0 / i;
    return h;
}

// recursive implementation
constexpr double harmonic_v1(const size_t n) noexcept {
    return (n < 2) ? n : 1.0 / n + harmonic_v1(n - 1);
}

// using templates
template <size_t N>
constexpr double harmonic_v2() noexcept {
    return 1.0 / N + harmonic_v2<N - 1>();
}
template<> constexpr double harmonic_v2<0>() noexcept { return 0; }

/*******************************************************************************
 * End of Harmonic number section                                              *
 ******************************************************************************/



/*******************************************************************************
 * Fibonacci sequence section                                                  *
 ******************************************************************************/

// loop implementation
constexpr size_t fib_v0(const size_t n) noexcept {
    if (n == 0 || n == 1)
        return n;
    size_t f0 {1}, f1 {1}, f {1};
    for (size_t i {2}; i < n; ++i) {
        f  = f0 + f1;
        f0 = f1;
        f1 = f;
    }
    return f;
}

// recursive implementation
constexpr size_t fib_v1(const size_t n) noexcept {
    return (n < 2) ? n : fib_v1(n - 1) + fib_v1(n - 2);
}

// apply golden ratio estimation with rounding
constexpr size_t fib_v2(const size_t n) noexcept {
    return std::round(std::pow(phi, n) / sqrt_of_5);
}

// using templates: can accept only const parameter
template <size_t N>
constexpr size_t fib_v3() noexcept {
    return fib_v3<N - 1>() + fib_v3<N - 2>();
}
template<> constexpr size_t fib_v3<1>() noexcept { return 1; }
template<> constexpr size_t fib_v3<0>() noexcept { return 0; }

/*******************************************************************************
 * End of Fibonacci sequence section                                           *
 ******************************************************************************/



/*******************************************************************************
 * Stirling section                                                            *
 ******************************************************************************/

// Stirling number of the first kind for signed values
constexpr int stirling_kind1_v0(const size_t n, const size_t k) {
    return (k == 0) ? ((n == 0) ? 1 : 0) : (n == 0) ? 0 :
        stirling_kind1_v0(n - 1, k - 1) - (n - 1) * stirling_kind1_v0(n - 1, k);
}

// Stirling number of the first kind for unsigned values
constexpr size_t stirling_kind1_v1(const size_t n, const size_t k) {
    return (k == 0) ? ((n == 0) ? 1 : 0) : (n == 0) ? 0 :
        stirling_kind1_v1(n - 1, k - 1) + (n - 1) * stirling_kind1_v1(n - 1, k);
}

// Stirling number of the second kind
constexpr size_t stirling_kind2_v0(const size_t n, const size_t k) {
    return (k == 0) ? 0 : (n == k) ? 1 :
        stirling_kind2_v0(n - 1, k - 1) + k * stirling_kind2_v0(n - 1, k);
}

/*******************************************************************************
 * End of Stirling section                                                     *
 ******************************************************************************/



/*******************************************************************************
 * Polynomial section                                                          *
 ******************************************************************************/

// value of the polynomial based on the provided argument
template <class T, class U>
constexpr auto poly_val_v0_base(const std::vector<T>& v, const U val) {
    auto res = v.back() * val;
    for (auto i = v.size() - 2; i > 0; --i)
        res = (res + v[i]) * val;
    return res + v.front();
}
template <class T, class U>
constexpr auto poly_val_v0(const std::vector<T>& v, const U val) {
    return v.empty() ? 0 : (v.size() == 1) ? v[0] : poly_val_v0_base(v, val);
}

/*******************************************************************************
 * End of Polynomial section                                                   *
 ******************************************************************************/



/*******************************************************************************
 * Prime numbers section                                                       *
 ******************************************************************************/

// Eratosthenes sieve - simple algorithm
std::vector<size_t> eratosthenes_sieve(const size_t n) {
    std::vector<bool> is_prime(n, true);
    size_t i_sqr {4};
    for (size_t i {2}; i_sqr <= n; ++i, i_sqr = i * i)
        if (is_prime[i])
            for (size_t j {i_sqr}; j <= n; j += i)
                is_prime[j] = false;
    std::vector<size_t> res {};
    // keep only odd numbers, in the following algo we'll skip even values
    for (size_t i {2}; i <= n; ++i)
        if (is_prime[i])
            res.push_back(i);
    return res;
}

// Eratosthenes sieve - segmented algorithm
std::vector<size_t> eratosthenes_sieve_segmented(const size_t n) {
    const auto delta = static_cast<size_t>(std::sqrt(n) + 1);
    const auto primes = eratosthenes_sieve(delta);
    auto res = primes;
    for (size_t low {delta}; low <= n; low += delta) {
        std::vector<bool> sieve(delta, true);
        const auto high = std::min(low + delta, n);

        for (size_t i {0}; i < primes.size(); ++i) {
            auto min_num = (low / primes[i]) * primes[i];
            if (min_num < low)
                min_num += primes[i];
            for (size_t j {min_num}; j < high; j += primes[i])
                sieve[j - low] = false;
        }

        for (size_t i {low}; i < high; ++i)
            if (sieve[i - low])
                res.push_back(i);
    }
    return res;
}

// choose algorithm for primes depending on the input
std::vector<size_t> primes(const size_t n) {
    return (n < 100) ? eratosthenes_sieve(n) : eratosthenes_sieve_segmented(n);
}

/*******************************************************************************
 * Enf of Prime numbers section                                                *
 ******************************************************************************/

} /* Algo_lib namespace */

#endif /* ALGO_LIB_H */

