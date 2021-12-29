#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono>

// Eratosthenes sieve algo
std::vector<size_t> eratosthenes_sieve(const size_t n) {
    std::vector<bool> res(n, true);
    size_t i {1};
    while ((++i) * i <= n) {
        if (res[i]) {
            size_t j {i * i};
            while (j <= n) {
                res[j] = false;
                j += i;
            }
        }
        //++i;
    }
    std::vector<size_t> out {};
    for (size_t i {2}; i < n; ++i)
        if (res[i])
            out.push_back(i);
    return out;
}


// Eratosthenes sieve algo
//std::vector<size_t> eratosthenes_sieve(const size_t n) {
//    std::vector<bool> res(n, true);
//    size_t i {1};
//    while (++i * i <= n) {
//        if (res[i]) {
//            size_t j {i * i};
//            while (j <= n) {
//                res[j] = false;
//                j += i;
//            }
//        }
//        //++i;
//    }
//    std::vector<size_t> out {};
//    for (size_t j {2}; j < n; ++j)
//        if (res[j])
//            out.push_back(j);
//    return out;
//}
//
void factorise_trial(size_t n) {
    std::vector<size_t> out {};
    std::cout << n << " = ";

    const auto primes = eratosthenes_sieve(static_cast<size_t>(std::sqrt(n) + 1));
    while (true) {
        bool b = true;
        for (const auto x: primes) {
            if (n % x == 0) {
                out.push_back(x);
                n = n / x;
                b = false;
                break;
            }
        }
        if (b) {
            if (n > 1)
                out.push_back(n);
            break;
        }
    }

    if (!out.empty()) {
        for (size_t i {0}; i < out.size() - 1; ++i)
            std::cout << out[i] << '*';
        std::cout << out.back() << '\n';
    } else {
        std::cout << n << '\n';
    }
}

void factorise_trial2(size_t n) {
    std::vector<size_t> out {};
    std::cout << n << " = ";

    const auto primes = eratosthenes_sieve(static_cast<size_t>(std::sqrt(n) + 1));

    std::ostringstream oss {};
    for (const auto x: primes) {
        while (n % x == 0) {
            //std::cout << x << '*';
            oss << x << '*';
            n = n / x;
        }
    }
    const auto s = oss.str();
    if (n > 1)
        //std::cout << n << '\n';
        std::cout << s << n << '\n';
    else
        //std::cout << '\n';
        std::cout << s.substr(0, s.length() - 1) << '\n';
}

void factorise_trial3(size_t n) {
    std::vector<size_t> out {};
    std::cout << n << " = ";

    for (const auto x:
            eratosthenes_sieve(static_cast<size_t>(std::sqrt(n) + 1)))
        while (n % x == 0) {
            out.push_back(x);
            n = n / x;
        }

    if (n > 1)
        out.push_back(n);

    for (size_t i {0}; i < out.size() - 1; ++i)
        std::cout << out[i] << '*';
    std::cout << out.back() << '\n';
}


int main() {
    using namespace std::chrono;

    //constexpr size_t n {static_cast<size_t>(std::pow(2, 32)) - 1};
    constexpr size_t n {9};
    constexpr size_t num_iter = 1;

    auto t0 = high_resolution_clock::now();
    for (size_t i {0}; i < num_iter; ++i)
        factorise_trial(n);
    const auto t1 = duration<double>(high_resolution_clock::now() - t0).count();

    t0 = high_resolution_clock::now();
    for (size_t i {0}; i < num_iter; ++i)
        factorise_trial2(n);
    const auto t2 = duration<double>(high_resolution_clock::now() - t0).count();

    std::cout << std::fixed << t1 << ' ' << t2 << " seconds\n";

    return 0;
}

