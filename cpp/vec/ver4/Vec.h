#ifndef VEC_H
#define VEC_H

#include "Algebra_lib.h"
#include <array>
#include <fstream>
#include <cmath> // std::sqrt function for norm()

namespace Algebra_lib {

template <size_t N, class Num>
class Vec {
    static_assert(N > 0, "Vec size must be positive");
    static_assert(N <= max_vec_size, "Vec size must not exceed max_vec_size");
public:
    using value_type = Num;
    /*
     **************************************************
     * Ctors, assignment operators and dtor           *
     **************************************************
     */
    // ctors
    constexpr Vec();
    explicit constexpr Vec(const std::array<Num, N>&);
    explicit Vec(const std::initializer_list<Num>&);

    // index operator
    const value_type& operator[](const size_t i) const { return V_[i]; }
    value_type& operator[](const size_t i) { return
        const_cast<value_type&>(static_cast<const Vec<N, Num>&>(*this)[i]); }

    constexpr size_t size() const { return N; }

private:
    std::array<Num, N> V_;
};

// default constructor
template <size_t N, class Num>
constexpr Vec<N, Num>::Vec(): V_{} { }

// constructor using std::array
template <size_t N, class Num>
constexpr Vec<N, Num>::Vec(const std::array<Num, N> &A): V_{A} { }

// constructor using initializer_list: the Vec is initialized to zero values; if
// the list is too short, then the rest of the Vec is zero; if the list is too
// long, then the excessive values are ignored
template <size_t N, class Num>
inline Vec<N, Num>::Vec(const std::initializer_list<Num> &IL): V_{} {
    auto iter = std::begin(IL);
    for (size_t i {0}; i < std::min(N, IL.size()); ++i, ++iter)
        V_[i] = *iter;
}


} // end Algebra_lib

#endif /* VEC_H */

