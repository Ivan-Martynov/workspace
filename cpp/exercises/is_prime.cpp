#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

std::vector<size_t> prime_numbers(const size_t n) {
    std::vector<size_t> res {2};
    for (size_t i {3}; i <= n; i += 2) {
        if ((i > 10) && (i % 10 == 5))
            continue;
        for (const auto &x: res) {
            if (x * x > i) {
                res.push_back(i);
                goto next_step;
            }
            if (i % x == 0)
                goto next_step;
        }
        res.push_back(i);
        next_step:;
    }
    return res;
}

std::vector<size_t> eratosthenes_sieve(const size_t n) {
    std::vector<bool> res(n, true);
    const double n_sqrt {std::sqrt(n)};
    size_t i {1};
    while (++i <= n_sqrt) {
        if (res[i]) {
            size_t j {i * i};
            while (j <= n) {
                res[j] = false;
                j += i;
            }
        }
    }
    std::vector<size_t> out {};
    for (size_t j {2}; j < n; ++j)
        if (res[j])
            out.push_back(j);
    return out;
}

std::vector<size_t> prime_numbers(size_t m, const size_t n) {
    // use Eratosthenes sieve to generate prime numbers to check against
    auto v = eratosthenes_sieve(static_cast<size_t>(std::sqrt(n) + 1));
    std::vector<size_t> res {};
    // add number two as the only even prime number if m == 2
    if (m == 2)
        res.push_back(2);
    // modify m to be odd number if it is even
    if (m % 2 == 0)
        ++m;
    // check numbers between m and n divisible by prime numbers
    for (size_t i {m}; i <= n; i += 2) {
        for (const auto x: v) {
            // make sure the number is not already prime
            if ((i != x) && (i % x == 0)) {
                goto next_step;
            }
        }
        res.push_back(i);
next_step:;
    }
    return res;
}

void test_prime() {
    //constexpr size_t m {2}, n {300000};
    size_t m, n;
    std::cout << "Enter two numbers: ";
    std::cin >> m >> n;

    //using namespace std::chrono;
    //auto t0 = high_resolution_clock::now();

    const auto v = prime_numbers(m, n);
    //const auto v = eratosthenes_sieve(n);
    if (v.empty())
        std::cerr << "Absent\n";
    else
        for (const auto&x: v)
            std::cout << x << '\n';

    //std::cout << std::fixed <<
    //    duration<double>(high_resolution_clock::now() - t0).count()
    //    << " seconds\n";
}

int main() {

    test_prime();

    return 0;
}

