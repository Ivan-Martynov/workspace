#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

// Eratosthenes sieve algo
std::vector<size_t> eratosthenes_sieve(const size_t n) {
    std::vector<bool> res(n, true);
    size_t i {2};
    while (i * i <= n) {
        if (res[i]) {
            size_t j {i * i};
            while (j <= n) {
                res[j] = false;
                j += i;
            }
        }
        ++i;
    }
    std::vector<size_t> out {};
    // keep only odd numbers, in the following algo we'll skip even values
    for (size_t j {3}; j < n; ++j)
        if (res[j])
            out.push_back(j);
    return out;
}

auto print_prime_numbers(size_t m, const size_t n) {
    // use Eratosthenes sieve to generate prime numbers to check against
    const auto v = eratosthenes_sieve(static_cast<size_t>(std::sqrt(n) + 1));
    //const size_t sieve_num = v.size();
    std::vector<size_t> res {};
    if (m < 2 && n >= 2)
        m = 2;
    // add number two as the only even prime number if m == 2
    if (m == 2)
        printf("%lu\n", m);
        //res.push_back(2);
    // modify m to be an odd number if it is even
    if (!(m & 1))
        ++m;
    // check numbers between m and n divisible by prime numbers
    for (size_t i {m}; i <= n; i += 2) {
        for (const auto x: v)
            if ((i != x) && (i % x == 0))
                goto next_step;
        //printf("%lu\n", i);
        res.push_back(i);
next_step:;
    }
    return res;
}

size_t scan_size_t() {
    size_t n {0};
    for (register char c = getchar(); (c > 47 && c < 58); c = getchar())
        n = n * 10 + c - 48;
    return n;
}

int main() {

    // read the two value for a file
    //size_t m, n;
    size_t t = scan_size_t();
    //std::cin >> t;
    std::vector<size_t> ms(t), ns(t);
    //scanf("%lu", &t);

    for (size_t i {0}; i < t; ++i) {
        ms[i] = scan_size_t();
        ns[i] = scan_size_t();
    }
        //scanf("%lu%lu", &ms[i], &ns[i]);
        //std::cin >> ms[i] >> ns[i];

    for (size_t i {0}; i < t; ++i) {
        const auto v = print_prime_numbers(ms[i], ns[i]);
        //std::copy(v.begin(), v.end(),
        //        std::ostream_iterator<size_t>(std::cout, "\n"));
        //printf("%lu\n", v);
        for (const auto x: v)
            printf("%lu\n", x);
        //eratosthenes_sieve_segmented(ms[i], ns[i]);
        //auto v = prime_numbers(ms[i], ns[i]);
        //for (const auto x: v)
        //    std::cout << x << '\n';
        //std::cout << '\n';
    }


    //std::ifstream ifs {"input.txt"};
    //if (!(ifs >> t)) {
    //    throw std::runtime_error{"cannot open file"};
    //    return 1;
    //}
    //if (!(ifs >> m >> n)) {
    //    throw std::runtime_error{"cannot open file"};
    //    return 1;
    //}

    //constexpr size_t m {2}, n {5};
    //const auto v = prime_numbers(m, n);

    //if (v.empty())
    //    std::cout << "Absent\n";
    //else
    //    for (const auto x: v)
    //        std::cout << x << '\n';

    return 0;
}

