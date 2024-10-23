#ifndef H_ALGORITHMS_INCLUDE_NUMERICAL_H_H
#define H_ALGORITHMS_INCLUDE_NUMERICAL_H_H

#include <type_traits> // For templates metaprogramming.

namespace Marvin
{

/*******************************************************************************
 * Helper section                                                              *
 ******************************************************************************/

template <typename T, typename U>
inline constexpr std::common_type_t<T, U> min(const T& lhs, const U&rhs)
{
    return lhs < rhs ? lhs : rhs;
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
inline constexpr T abs(const T& value)
{
    return value < static_cast<T>(0) ? -value : value;
}

/*******************************************************************************
 * End of Helper section                                                       *
 ******************************************************************************/

/*******************************************************************************
 * Square root calculation section                                             *
 ******************************************************************************/

inline constexpr double sqrtNewtonRaphson(double x, double curr, double prev)
{
    return curr == prev ? curr
                        : sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
}

inline constexpr double sqrt(double x)
{
    return sqrtNewtonRaphson(x, x, 0);
}

/*******************************************************************************
 * End of Square root calculation section                                      *
 ******************************************************************************/

/*******************************************************************************
 * Golden ratio section                                                        *
 ******************************************************************************/

inline constexpr double golden_ratio() noexcept
{
    return (sqrt(5) + 1.0) * 0.5;
}

/*******************************************************************************
 * End of Golden ratio section                                                 *
 ******************************************************************************/

/*******************************************************************************
 * Greatest common divisor (gcd) section                                       *
 * Contains few implementations concerning gcd                                 *
 ******************************************************************************/

// classic version of the greatest common divisor function
inline constexpr int gcd_v0_base(const int a, const int b) noexcept
{
    if (a < b)
    {
        return gcd_v0_base(b, a);
    }

    const auto r {a % b};
    return (r == 0) ? b : gcd_v0_base(b, r);
}

inline constexpr int gcd_v0(const int a, const int b) noexcept
{
    return (a == 0) ? abs(b) : (b == 0) ? abs(a) : gcd_v0_base(abs(a), abs(b));
}

// constexpr implementation of the greatest common divisor function
inline constexpr int gcd_v1_base(const int a, const int b) noexcept
{
    return (a < b)        ? gcd_v1_base(b, a)
           : (a % b == 0) ? b
                          : gcd_v1_base(b, a % b);
}

inline constexpr int gcd_v1(const int a, const int b) noexcept
{
    return (a == 0) ? abs(b) : (b == 0) ? abs(a) : gcd_v1_base(abs(a), abs(b));
}

// implementing "effective" algorithm
inline constexpr int gcd_v2_base(const int a, const int b) noexcept
{
    return (a == b) ? a : gcd_v2_base(min(a, b), abs(a - b));
}

inline constexpr int gcd(const int a, const int b) noexcept
{
    return (a == 0) ? abs(b) : (b == 0) ? abs(a) : gcd_v2_base(abs(a), abs(b));
}

inline constexpr void normalize_by_gcd(int& a, int& b)
{
    const auto divisor {gcd(a, b)};
    a /= divisor;
    b /= divisor;
}

inline constexpr int lcm(int a, int b) noexcept
{
    return abs(a * (b / gcd(a, b)));
}

// finding gcd plus a and b, such that d = a * m + b * n
int extended_gcd(int m, int n, int& a, int& b)
{
    m = abs(m);
    n = abs(n);
    if (m < n)
        return extended_gcd(n, m, b, a);
    int a_prime = b = 1;
    int b_prime = a = 0;
    int c = m, d = n;
    int q = c / d;
    while (int r = c % d)
    {
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
inline constexpr auto factorial(int n)
{
    int p {1};
    for (int i {2}; i <= n; ++i)
    {
        p *= i;
    }
    return p;
}

// recursive implementation
inline constexpr int factorial_recursive(const int n)
{
    return (n < 2) ? 1 : n * factorial_recursive(n - 1);
}

// using templates: can accept only const parameter
template <int N> inline constexpr int factorial_template() noexcept
{
    return N * factorial_template<N - 1>();
}
template <> inline constexpr int factorial_template<0>() noexcept
{
    return 1;
}

/*******************************************************************************
 * End of Factorial section                                                    *
 ******************************************************************************/



} // namespace Marvin

#endif // H_ALGORITHMS_INCLUDE_NUMERICAL_H_H
