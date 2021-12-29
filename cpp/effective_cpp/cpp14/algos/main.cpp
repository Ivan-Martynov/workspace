#include <iostream>
#include "Algo_lib.h"
#include "Generating_function.h"
#include <complex>

void test_gcd() {
    using namespace Algo_lib;
    constexpr int a {544}, b {-119};

    std::cout << "Greatest common divisor:\n";
    std::cout << gcd_v0(a, b) << '\n';
    std::cout << gcd_v1(std::abs(b), std::abs(a)) << '\n';
    std::cout << gcd_v2(1384, 2405) << '\n';
    std::cout << "Extended greatest common divisor:\n";
    int q, p;
    const int d = extended_gcd(1769, 551, q, p);
    std::cout << d << ' ' << q << ' ' << p << '\n';
    std::cout << gcd_v0(2, 0) << '\n';
    std::cout << gcd_v1(0, -6) << '\n';
    std::cout << gcd_v2(0, 0) << '\n';
}

void test_factorial() {
    using namespace Algo_lib;

    std::cout << "Factorials:\n";
    constexpr size_t n {5};
    for (size_t i {0}; i <= n; ++i) {
        std::cout << "fac(" << i << ") = " << fac_v0(i) << '\n';
        std::cout << "fac(" << i << ") = " << fac_v1(i) << '\n';
    }
    constexpr size_t m {6};
    std::cout << "fac<" << m << ">() = " << fac_v2<m>() << '\n';
}

void test_binomial() {
    using namespace Algo_lib;

    std::cout << "Binomial coefficients:\n";
    constexpr size_t n {7};
    for (size_t i {0}; i <= n + 1; ++i) {
        std::cout << "binom_v0(" << n << ", " << i << ") = " <<
            binom_v0(n, i) << '\n';
        std::cout << "binom_v1(" << n << ", " << i << ") = " <<
            binom_v1(n, i) << '\n';
        std::cout << "binom_v2(" << n << ", " << i << ") = " <<
            binom_v2(n, i) << '\n';
    }
    constexpr size_t p {0}, q {0};
    std::cout << "binom_v3<" << p << ", " << q << ">() = " <<
        binom_v3<p, q>() << '\n';
}

void test_harmonic() {
    using namespace Algo_lib;

    std::cout << "Harmonic numbers:\n";
    size_t n {7};
    std::cout << "harmonic_v0(" << n << ") = " << harmonic_v0(n) << '\n';
    n = 19;
    std::cout << "harmonic_v1(" << n << ") = " << harmonic_v1(n) << '\n';
    constexpr size_t m {128};
    std::cout << "harmonic_v2<" << m << ">() = " << harmonic_v2<m>() << '\n';
}

void test_fibonacci() {
    using namespace Algo_lib;
    size_t n {5};
    std::cout << "Fibonacci sequence:\n";
    std::cout << "fib_v0(" << n << ") = " << fib_v0(n) << '\n';
    n = 7;
    std::cout << "fib_v1(" << n << ") = " << fib_v1(n) << '\n';
    constexpr size_t m {10};
    std::cout << "fib_v3<" << m << ">() = " << fib_v3<m>() << '\n';
    for (size_t i {0}; i < 10; ++i) {
        std::cout << "fib_v1(" << i << ") = " << fib_v1(i) << '\n';
        std::cout << "fib_v2(" << i << ") = " << fib_v2(i) << '\n';
    }
    constexpr size_t p {27};
    std::cout << "fib_v3<" << p << ">() = " << fib_v3<p>() << '\n';
}

// test for variadic template (parameter pack)
template <class T>
constexpr T sum_vals(const T& t) noexcept { return t; }
template <class T, class ... U>
constexpr T sum_vals(const T& t, const U&... args) {
    std::cout << sizeof...(args) << '\n';
    return sum_vals(t) + sum_vals(args...);
}

// test generating functions
void test_generating_function() {
    using namespace Algo_lib;
    Generating_function<int> g1 {2, 3, 4};
    Generating_function<int> g2 {std::vector<int>(4)};
    Generating_function<int> g3 {7};
    Generating_function<int> g4 {};
    std::cout << "g1 = " << g1 << '\n';
    std::cout << "g2 = " << g2 << '\n';
    std::cout << "g3 = " << g3 << '\n';
    std::cout << "g4 = " << g4 << '\n';
    constexpr auto x = 1.5;
    std::cout << "g1(" << x << ") = " << g1.value(x) << '\n';
    std::cout << "g2(" << x << ") = " << g2.value(x) << '\n';
    std::cout << "g3(" << x << ") = " << g3.value(x) << '\n';
    std::cout << "g4(" << x << ") = " << g4.value(x) << '\n';

    //g1 *= 2;
    //g1 += g3;
    //std::cout << "g1 = " << g1 << '\n';

    Generating_function<double> g5 {1.5, 2.4, 8.3, 9};
    std::cout << "g5 = " << g5 << '\n';
    //g5 *= 2.5f;
    //std::cout << "g5 = " << g5 << '\n';

    Generating_function<double> gtemp = 2 * g1 * 3 * 4 + 7 * 3 * g2 * 1 +
        1.5 * g3 * 2.3 + 8 * g4 * 2 + 0.5 * g5 * 4;
    std::cout << "gtemp = " << gtemp << '\n';

    std::cout << "g1 = " << g1 << '\n';
    gtemp = g1.shift(-1);
    std::cout << "gtemp = " << gtemp << '\n';
    gtemp = g1.shift(2);
    std::cout << "gtemp = " << gtemp << '\n';

    //gtemp = g5 * g2;
    gtemp = g5 * Generating_function<int>{8, 0, -3, 5};
    std::cout << "gtemp = " << gtemp << '\n';

    auto g6 = 0.5 * (g5.change(1) + g5.change(-1));
    auto g7 = 0.5 * (g5.change(1) - g5.change(-1));
    std::cout << "g6 = " << g6 << '\n';
    std::cout << "g7 = " << g7 << '\n';

    std::cout << "g5' = " << g5.derivative() << '\n';
    std::cout << "Sg5 = " << g5.integral() << '\n';

    auto gbin1 = binomial_series(5);
    std::cout << "gbin1 = " << gbin1 << '\n';
    std::cout << "gbin1(0.5) = " << gbin1.value(0.5) << '\n';
    std::cout << std::pow(1.5, 5) << '\n';

    auto gexp1 = exponential_series(8);
    std::cout << "gexp1 = " << gexp1 << '\n';
    std::cout << "gexp1(2) = " << gexp1.value(2) << '\n';
    std::cout << std::exp(2) << '\n';

    auto glog1 = logarithm_series(6);
    std::cout << "glog1 = " << glog1 << '\n';
    std::cout << "glog1(0.5) = " << glog1.value(0.5) << '\n';
    std::cout << std::log(1.5) << '\n';

    //std::vector<int> v1 {2, 8, 4, 7};
    //std::vector<double> v2 {2.5, 9.8, 4, 7};
    //v1 = std::vector<int>{v2.begin(), v2.end()};
    //for (const auto v: v1)
    //    std::cout << v << ' ';
    //std::cout << '\n';

    //std::cout << std::numeric_limits<double>::infinity() << '\n';
    //std::complex<double> cpx {1, 2};
    //std::cout << std::is_arithmetic<decltype(cpx)>::value << '\n';
    //g1 += g5;
    //auto g_sum = 2 * g1 + (g2 * 3 + 4 * g3) * 2 + g4 * 8;
    //std::cout << "g1 + g2 + g3 + g4 = " << g_sum << '\n';
    ////std::cout << Generating_function<double>{2.3, 4.5, 9}.value(x) << '\n';

    //std::cout << "g1 * 2 = " << 2 * g1 * 4 << '\n';
}

void test_prime() {
    using namespace Algo_lib;
    constexpr size_t n {100000};
    auto v = eratosthenes_sieve(n);
    std::cout << v.size() << '\n';
    v = eratosthenes_sieve_segmented(n);
    std::cout << v.size() << '\n';
    //const auto v = primes(n);
    //for (const auto x : v)
    //    std::cout << x << ' ';
    //std::cout << '\n';
}

int main() {

    const auto num_reps = 1;

    //const auto t = Algo_lib::time_func(num_reps, test_factorial);
    //const auto t = Algo_lib::time_func(num_reps, test_gcd);
    //const auto t = Algo_lib::time_func(num_reps, test_fibonacci);
    //const auto t = Algo_lib::time_func(num_reps, test_binomial);
    //const auto t = Algo_lib::time_func(num_reps, test_harmonic);
    //const auto t = Algo_lib::time_func(num_reps, test_generating_function);
    const auto t = Algo_lib::time_func(num_reps, test_prime);

    std::cout << "Execution time: " <<
        std::chrono::duration<double>(t).count() << " seconds\n";

    //std::cout << sum_vals(-1, 22, 13, -8) << '\n';

    return 0;
}

