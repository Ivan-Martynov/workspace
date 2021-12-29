#ifndef GENERATING_FUNCTION_H
#define GENERATING_FUNCTION_H

#include <iostream>
#include <fstream>
#include <vector>
#include <type_traits>
#include "Algo_lib.h"

/*
 * Class representing generation function concept (from Knuth books).
 * Essentially a generation function is a polynomial here, because we assume
 * sequences to be finite.
 */
namespace Algo_lib {

// forward declarations
template <class T> class Generating_function;
template <class T> std::ostream& operator<<(std::ostream&,
        const Generating_function<T>&);

template <class T>
class Generating_function {
public:
    // conventional alias names
    using value_type     = T;
    using size_type      = typename std::vector<T>::size_type;
    using iterator       = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    // iterators utils
    constexpr iterator begin() noexcept { return seq_.begin(); }
    constexpr iterator end() noexcept { return seq_.end(); }
    constexpr const_iterator begin() const noexcept { return seq_.begin(); }
    constexpr const_iterator end() const noexcept { return seq_.end(); }
    constexpr const_iterator cbegin() const noexcept { return seq_.cbegin(); }
    constexpr const_iterator cend() const noexcept { return seq_.cend(); }
    constexpr const_iterator rbegin() const noexcept { return seq_.rbegin(); }
    constexpr const_iterator rend() const noexcept { return seq_.rend(); }
    constexpr const_iterator crbegin() const noexcept { return seq_.crbegin(); }
    constexpr const_iterator crend() const noexcept { return seq_.crend(); }

    // default constructor: empty sequence
    Generating_function(): seq_{} { };
    // constructor: copying sequence values
    explicit Generating_function(const std::vector<T>& v): seq_{v} { }
    // constructor taking initializer_list as a parameter
    explicit Generating_function(const std::initializer_list<T>& i): seq_{i} { }

    // copy constructor
    explicit Generating_function(const Generating_function& o): seq_{o.seq_} { }

    // copy constructor taking a Generating_function of another type:
    // enabled only if type U can be implicitly cast to type T
    //template <class U, typename = std::enable_if_t<
    //    std::is_same<T, std::common_type_t<T, U>>::value>>
    // no restrictions: may truncate
    template <class U>
        Generating_function(const Generating_function<U>& o):
            seq_{o.cbegin(), o.cend()} { }
    // copy assignment
    Generating_function& operator=(const Generating_function& o) {
        if (*this != o) seq_ = o.seq_; return *this;
    };
    // copy assignment taking a Generating_function of another type:
    // enabled only if type U can be implicitly cast to type T
    //template <class U, typename = std::enable_if_t<
    //    std::is_same<T, std::common_type_t<T, U>>::value>>
    // no restrictions: may truncate
    template <class U>
        Generating_function& operator=(const Generating_function<U>& o) {
            seq_ = std::vector<T>{o.cbegin(), o.cend()}; return *this;
        }

    // default move constructor/assignment
    Generating_function(Generating_function&&) = default;
    Generating_function& operator=(Generating_function&&) = default;

    // default constructor
    ~Generating_function() = default;

    // compound arithmetic addition
    // checking types
    //template <class U, typename = std::enable_if_t<
    //            std::is_same<T, std::common_type_t<T, U>>::value>>
    // no restrictions: may truncate
    template <class U>
    Generating_function& operator+=(const Generating_function<U>& rhs) noexcept
    {
        for (size_t i {0}; i < std::min(size(), rhs.size()); ++i)
            seq_[i] += rhs[i];
        return *this;
    }
    //template <class U, typename = std::enable_if_t<
    //            std::is_same<T, std::common_type_t<T, U>>::value>>
    //Generating_function& operator+=(Generating_function<U>&& rhs) noexcept
    //{
    //    for (size_t i {0}; i < std::min(size(), rhs.size()); ++i)
    //        seq_[i] += rhs[i];
    //    return *this;
    //}

    // compound arithmetic subtraction
    template <class U>
    Generating_function& operator-=(const Generating_function<U>& rhs) noexcept
    {
        for (size_t i {0}; i < std::min(size(), rhs.size()); ++i)
            seq_[i] -= rhs[i];
        return *this;
    }
    // compound arithmetic multiplication: checking types
    //template <class U, typename = std::enable_if_t<
    //            std::is_same<T, std::common_type_t<T, U>>::value>>
    // no restrictions: may truncate
    template <class U>
        Generating_function& operator*=(const U rhs) {
            for_each(seq_.begin(), seq_.end(), [rhs](T &x) { x *= rhs; });
            return *this;
        }

    //Generating_function& shift(const int);
    const Generating_function shift(const int) const;
    const Generating_function change(const T) const;

    // index operator
    constexpr value_type operator[](const size_t i) const { return seq_[i]; }
    constexpr value_type& operator[](const size_t i) { return seq_[i]; };
    constexpr value_type& at(const size_t i) { return seq_.at(i); }
    const value_type& at(const size_t i) const { return seq_.at(i); }

    const std::vector<T> sequence() const noexcept { return seq_; }
    size_t size() const noexcept { return seq_.size(); }
    bool empty() const noexcept { return seq_.empty(); }

    template <class U>
        constexpr auto value(const U x) const { return poly_val_v0(seq_, x); }

    // First derivative
    const Generating_function derivative() const;
    // Integral
    const Generating_function integral() const;

private:
    std::vector<T> seq_;
    // output operator as a friend function
    friend std::ostream& operator<< <> (std::ostream&,
            const Generating_function&);
};

/*
 * Sum of two Generating_function objects.
 * Allows using different types returning the common type
 */
template <class T, class U>
auto operator+(const Generating_function<T>& lhs,
        const Generating_function<U>& rhs) {
    //std::cout << "simple + version\n";
    return (lhs.size() > rhs.size()) ?
        Generating_function<std::common_type_t<T, U>>{lhs} += rhs :
        Generating_function<std::common_type_t<T, U>>{rhs} += lhs;
}
// sum version with lhs as an rvalue
template <class T, class U>
auto operator+(Generating_function<T>&& lhs,
        const Generating_function<U>& rhs) {
    //std::cout << "rvalue + version\n";
    return (std::is_same<T, std::common_type_t<T, U>>::value) ?
        std::move(lhs += rhs) :
        std::move(Generating_function<std::common_type_t<T, U>>{lhs} += rhs);
}
// sum version with rhs as an rvalue
template <class T, class U>
auto operator+(const Generating_function<T>& lhs,
        Generating_function<U>&& rhs) { return operator+(rhs, lhs); }
// sum version with lhs and rhs as rvalues
template <class T, class U>
Generating_function<std::common_type_t<T, U>> operator+(
        Generating_function<T>&& lhs, Generating_function<U>&& rhs) {
    //std::cout << "rvalue + rvalue + version\n";
    if (std::is_same<T, std::common_type_t<T, U>>::value)
        return std::move(lhs += rhs);
    else
        return std::move(Generating_function<std::common_type_t<T, U>>{lhs}
                += rhs);
}

/*
 * Difference of two Generating_function objects.
 * Allows using different types returning the common type
 */
template <class T, class U>
auto operator-(const Generating_function<T>& lhs,
        const Generating_function<U>& rhs) {
    return Generating_function<std::common_type_t<T, U>>{lhs} -= rhs;
}
// difference version with lhs as an rvalue
template <class T, class U>
auto operator-(Generating_function<T>&& lhs,
        const Generating_function<U>& rhs) {
    //std::cout << "rvalue + version\n";
    return (std::is_same<T, std::common_type_t<T, U>>::value) ?
        std::move(lhs -= rhs) :
        std::move(Generating_function<std::common_type_t<T, U>>{lhs} -= rhs);
}
// difference version with rhs as an rvalue
template <class T, class U>
auto operator-(const Generating_function<T>& lhs,
        Generating_function<U>&& rhs) { return operator-(rhs, lhs); }
// difference version with lhs and rhs as rvalues
template <class T, class U>
Generating_function<std::common_type_t<T, U>> operator-(
        Generating_function<T>&& lhs, Generating_function<U>&& rhs) {
    if (std::is_same<T, std::common_type_t<T, U>>::value)
        return std::move(lhs -= rhs);
    else
        return std::move(Generating_function<std::common_type_t<T, U>>{lhs}
                -= rhs);
}

/*
 * Product of a Generating_function object and value.
 * Allows using different types returning the common type
 */
template <class T, class U,
         typename = std::enable_if_t<std::is_arithmetic<U>::value>>
Generating_function<T> operator*(const Generating_function<T>& lhs,
        const U rhs) {
    //std::cout << "simple * version\n";
    return Generating_function<std::common_type_t<T, U>>{lhs} *= rhs;
}
// product version with lhs as an rvalue
template <class T, class U,
         typename = std::enable_if_t<std::is_arithmetic<U>::value>>
Generating_function<T> operator*(Generating_function<T>&& lhs, const U rhs) {
    //std::cout << "rvalue * version\n";
    return std::move(lhs *= rhs);
}
// product version with a Generating_function object on the right side
template <class T, class U,
         typename = std::enable_if_t<std::is_arithmetic<U>::value>>
Generating_function<T> operator*(const U lhs,
        const Generating_function<T>& rhs) { return operator*(rhs, lhs); }
// product version with an rvalue Generating_function object on the right side
template <class T, class U,
         typename = std::enable_if_t<std::is_arithmetic<U>::value>>
Generating_function<T> operator*(const U lhs, Generating_function<T>&& rhs) {
    return operator*(rhs, lhs);
}
/*
 * Product of two Generating_function objects.
 * Allows using different types returning the common type
 */
template <class T, class U>
auto operator*(const Generating_function<T>& lhs,
        const Generating_function<U>& rhs) {
    using C = std::common_type_t<T, U>;
    const size_t n = lhs.size();
    if (rhs.size() != n) {
        std::cerr << "Sizes should be the same, returning first operand\n";
        return lhs;
    }
    std::vector<C> v(n);
    for (size_t i {0}; i < n; ++i)
        for (size_t j {0}; j <= i; ++j)
            v[i] += lhs[j] * rhs[i - j];
    return Generating_function<C>{v};
}

// Shifting the Generating_function object: G(z) is a generating function for
// <a_n> = a0, a1, ... => z^m*G(z) is a generating function for
// <a_(n-m)> = 0, ..., 0, a0, a1, ...
//template<class T>
//Generating_function<T>& Generating_function<T>::shift(const int m) {
//    for (int i {0}; i < m; ++i)
//        seq_.insert(seq_.cbegin(), 0);
//    return *this;
//}
template <class T>
const Generating_function<T> Generating_function<T>::shift(const int m) const {
    if (m < 0) {
        std::vector<T> v {};
        std::copy(cbegin() - m, cend(), std::back_inserter(v));
        return Generating_function<T>{v};
    }
    std::vector<T> v {seq_};
    for (int i {0}; i < m; ++i)
        v.insert(v.cbegin(), 0);
    return Generating_function<T>{v};
}

// Multiply the z parameter with a constant, thus changing the sequence
template <class T>
const Generating_function<T> Generating_function<T>::change(const T c) const {
    std::vector<T> v {seq_};
    T t = 1;
    for (size_type i {1}; i < v.size(); ++i) {
        t *= c;
        v[i] *= t;
    }
    return Generating_function<T>{v};
}

// First derivative
template <class T>
const Generating_function<T> Generating_function<T>::derivative() const {
    const auto n = size(); // size of derivative parameters vector
    if (n < 1)
        return Generating_function<T>{};
    std::vector<T> v(n - 1);
    for (size_type i {1}; i < n; ++i)
        v[i - 1] = i * seq_[i];
    return Generating_function<T>{v};
}

// Integral
template <class T>
const Generating_function<T> Generating_function<T>::integral() const {
    const auto n = size(); // size of derivative parameters vector
    std::vector<T> v(n + 1);
    v.front() = 0;
    for (size_type i {1}; i <= n; ++i)
        v[i] = 1.0 / i * seq_[i - 1];
    return Generating_function<T>{v};
}

// definition of friend output operator
template <class T>
std::ostream& operator<<(std::ostream& os, const Generating_function<T>& f) {
    os << "{ ";
    if (!f.seq_.empty())
        os << f.seq_[0];
    for (size_t i {1}; i < f.seq_.size(); ++i)
        os << ", " << f.seq_[i];
    return os << " }";
}

/*
 * Various series
 */

// Binomial series (1 + z)^r if r > 0 or (1 - z)^(-r + 1) if (r < 0)
Generating_function<int> binomial_series(int r) {
    const bool b = r < 0;
    if (b)
        r = -r;

    std::vector<int> v(r + 1);
    if (b)
        for (int i {0}; i <= r; ++i)
            v[i] = binom_v1(i + r, r);
    else
        for (int i {0}; i <= r; ++i)
            v[i] = binom_v1(r, i);

    return Generating_function<int>{v};
}

// Exponential series e^z
Generating_function<double> exponential_series(const size_t n) {
    std::vector<double> v(n, 1.0);
    for (size_t i {2}; i < n; ++i)
        v[i] /= fac_v0(i);
    return Generating_function<double>{v};
}

// Logarithm series: ln(1 + z)
Generating_function<double> logarithm_series(const size_t n) {
    std::vector<double> v(n + 1, 1.0);
    v[0] = 0;
    int coeff = 1;
    for (int i {2}; i <= static_cast<int>(n); ++i) {
        coeff *= -1;
        v[i] /= coeff * i;
    }
    return Generating_function<double>{v};
}


}

#endif /* GENERATING_FUNCTION_H */

