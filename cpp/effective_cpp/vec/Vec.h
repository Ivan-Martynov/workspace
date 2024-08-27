#ifndef VEC_H
#define VEC_H

#include "Algebra_lib.h"
#include <array>
#include <cmath> // std::sqrt function for norm()
#include <complex>
#include <fstream>
#include <iostream>
#include <numeric>
#include <type_traits>

namespace Algebra_lib
{

template <size_t N, class Num> class Vec
{
    static_assert(N > 0, "Vec size must be positive");
    static_assert(N <= max_vec_size, "Vec size must not exceed max_vec_size");
    static_assert(
        std::is_arithmetic<Num>::value
            || std::is_base_of<std::complex<Num>, std::decay_t<Num>>::value,
        "The type for Vec should be arithmetic");

  public:
    using value_type = Num;
    using size_type = typename std::array<value_type, N>::size_type;
    using iterator = typename std::array<value_type, N>::iterator;
    using reverse_iterator =
        typename std::array<value_type, N>::reverse_iterator;
    using const_iterator = typename std::array<value_type, N>::const_iterator;
    using const_reverse_iterator =
        typename std::array<value_type, N>::const_reverse_iterator;

    /*
     **************************************************
     * Ctors, assignment operators and dtor           *
     **************************************************
     */
    // ctors
    constexpr Vec() noexcept;
    constexpr explicit Vec(const std::array<Num, N>&) noexcept;
    Vec(const value_type) noexcept;
    template <class Comp>
    constexpr explicit Vec(const std::array<Comp, N>&) noexcept;
    explicit Vec(const std::initializer_list<Num>&) noexcept;

    Vec(const Vec&) noexcept;
    Vec(Vec&&) = default;
    Vec& operator=(const Vec&) = default;
    Vec& operator=(Vec&&) = default;
    ~Vec() = default;

    template <class Comp> Vec(const Vec<N, Comp>&) noexcept;

    /*
     * ------------ Iterators and access ------------
     */
    // begin iterators
    constexpr iterator begin() noexcept
    {
        return V_.begin();
    }
    constexpr const_iterator begin() const noexcept
    {
        return V_.begin();
    }
    constexpr const_iterator cbegin() const noexcept
    {
        return V_.cbegin();
    }

    // end iterators
    constexpr iterator end() noexcept
    {
        return V_.end();
    }
    constexpr const_iterator end() const noexcept
    {
        return V_.end();
    }
    constexpr const_iterator cend() const noexcept
    {
        return V_.cend();
    }

    // reverse begin iterators
    constexpr reverse_iterator rbegin() noexcept
    {
        return V_.rbegin();
    }
    constexpr const_reverse_iterator rbegin() const noexcept
    {
        return V_.rbegin();
    }
    constexpr const_reverse_iterator crbegin() const noexcept
    {
        return V_.crbegin();
    }

    // reverse end iterators
    constexpr reverse_iterator rend() noexcept
    {
        return V_.rend();
    }
    constexpr const_reverse_iterator rend() const noexcept
    {
        return V_.rend();
    }
    constexpr const_reverse_iterator crend() const noexcept
    {
        return V_.crend();
    }

    // index access
    constexpr value_type& operator[](const size_t i) noexcept
    {
        return V_[i];
    }
    constexpr value_type operator[](const size_t i) const noexcept
    {
        return V_[i];
    }
    constexpr value_type& at(const size_t i)
    {
        return V_.at(i);
    }
    constexpr value_type at(const size_t i) const
    {
        return V_.at(i);
    }
    /*
     * ------------ End Iterators and access ------------
     */

    // arithemtic operators (compound)
    constexpr Vec& operator+=(const value_type rhs) noexcept
    {
        for (auto& x : V_)
            x += rhs;
        return *this;
    }
    constexpr Vec& operator+=(const Vec& rhs) noexcept
    {
        for (size_type i {0}; i < N; ++i)
            V_[i] += rhs[i];
        return *this;
    }
    constexpr Vec& operator-=(const value_type rhs) noexcept
    {
        for (auto& x : V_)
            x -= rhs;
        return *this;
    }
    constexpr Vec& operator-=(const Vec& rhs) noexcept
    {
        for (size_type i {0}; i < N; ++i)
            V_[i] -= rhs[i];
        return *this;
    }
    constexpr Vec& operator*=(const value_type rhs) noexcept
    {
        for (auto& x : V_)
            x *= rhs;
        return *this;
    }
    constexpr Vec& operator*=(const Vec& rhs) noexcept
    {
        for (size_type i {0}; i < N; ++i)
            V_[i] *= rhs[i];
        return *this;
    }
    constexpr Vec& operator/=(const value_type rhs)
    {
        if (rhs == 0)
            throw std::overflow_error {"division by zero"};
        for (auto& x : V_)
            x /= rhs;
        return *this;
    }
    constexpr Vec& operator/=(const Vec& rhs)
    {
        for (size_type i {0}; i < N; ++i)
        {
            if (rhs[i] == 0)
            {
                throw std::overflow_error {"division by zero"};
            }
            V_[i] /= rhs[i];
        }
        return *this;
    }

    constexpr std::array<Num, N> array() const noexcept
    {
        return V_;
    }
    constexpr size_t size() const noexcept
    {
        return N;
    }

    constexpr Num dot(const Vec&) const noexcept;
    constexpr double norm() const noexcept;
    constexpr void normalize() noexcept;

  private:
    std::array<Num, N> V_;
};

// default constructor
template <size_t N, class Num> constexpr Vec<N, Num>::Vec() noexcept : V_ {} {}

// constructor using std::array
template <size_t N, class Num>
constexpr Vec<N, Num>::Vec(const std::array<Num, N>& A) noexcept : V_ {A}
{
}

// constructor filling array with the same value
template <size_t N, class Num> Vec<N, Num>::Vec(const Num val) noexcept : V_ {}
{
    V_.fill(val);
}

template <size_t N, class Num>
template <class Comp>
constexpr Vec<N, Num>::Vec(const std::array<Comp, N>& A) noexcept : V_ {A}
{
}

// constructor using initializer_list: the Vec is initialized to zero values; if
// the list is too short, then the rest of the Vec is zero; if the list is too
// long, then the excessive values are ignored
template <size_t N, class Num>
inline Vec<N, Num>::Vec(const std::initializer_list<Num>& IL) noexcept : V_ {}
{
    auto iter = std::begin(IL);
    for (size_t i {0}; i < std::min(N, IL.size()); ++i, ++iter)
        V_[i] = *iter;
}

template <size_t N, class Num>
inline Vec<N, Num>::Vec(const Vec<N, Num>& other) noexcept : V_ {other.V_}
{
}

// dot product
template <size_t N, class Num>
constexpr Num Vec<N, Num>::dot(const Vec<N, Num>& rhs) const noexcept
{
    auto res = Num {};
    for (size_t i {0}; i < N; ++i)
        res += V_[i] * rhs[i];
    return res;
    // return std::inner_product(V_.cbegin(), V_.cend(), rhs.cbegin(), Num{});
}

template <size_t N, class Num>
constexpr double Vec<N, Num>::norm() const noexcept
{
    double d {0};
    for (const auto x : V_)
        d += x * x;
    return std::sqrt(d);
    // return std::sqrt(dot(*this));
}

template <size_t N, class Num> constexpr void Vec<N, Num>::normalize() noexcept
{
    operator/=(norm());
}

template <size_t N, class Num>
std::ostream& operator<<(std::ostream& os, const Vec<N, Num>& v)
{
    os << '(' << v[0];
    for (size_t i {1}; i < N; ++i)
        os << ", " << v[i];
    return os << ')';
}

} // namespace Algebra_lib

#endif /* VEC_H */
