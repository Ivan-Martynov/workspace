#ifndef MAT_H
#define MAT_H

#include "Vec.h"

/*
 * Class Mat:
 *      fixed size two-dimensional array: exploiting Vec class
 *      Num type hints that the type should be numeric
 *      Decided to use fixed size array because the Mat class is going to be
 *      used mainly for small (2x2 - 4x4 elements) matrices
 */
template <size_t R, size_t C, class Num>
class Mat {
    Vec<R, Vec<C, Num>> M_;
public:
    // ctors
    Mat(const Num& = 0); // fill matrix with a value
    Mat(const Vec<C, Num>&); // fill each matrix row with a Vec
    Mat(const std::array<std::array<Num, C>, R>&);
    Mat(const std::initializer_list<Vec<C, Num>>&);
    template <size_t R2, size_t C2, class Comp>
    Mat(const Mat<R2, C2, Comp>&); // templated copy ctor: e.g., double to int
    template <size_t R2, size_t C2, class Comp>
    Mat& operator=(const Mat<R2, C2, Comp>&);
    ~Mat() = default;

    // templated compound arithemtic operators
    template <class Comp>
    Mat& operator+=(const Comp&);
    template <class Comp>
    Mat& operator+=(const Vec<R, Comp>&);
    template <class Comp>
    Mat& operator+=(const Mat<R, C, Comp>&);
    template <class Comp>
    Mat& operator-=(const Comp&);
    template <class Comp>
    Mat& operator-=(const Vec<R, Comp>&);
    template <class Comp>
    Mat& operator-=(const Mat<R, C, Comp>&);
    template <class Comp>
    Mat& operator*=(const Comp&);
    template <class Comp>
    Mat& operator/=(const Comp&);

    // iterators: using in loops (for (auto m: M))
    using iterator = typename Vec<R, Vec<C, Num>>::iterator;
    using const_iterator = typename Vec<R, Vec<C, Num>>::const_iterator;
    iterator begin() { return M_.begin(); }
    const_iterator begin() const { return M_.begin(); }
    const_iterator cbegin() const { return M_.cbegin(); }
    iterator end() { return M_.end(); }
    const_iterator end() const { return M_.end(); }
    const_iterator cend() const { return M_.cend(); }

    // index and range-check index operators
    Vec<C, Num>& operator[](const size_t i) { return M_[i]; }
    const Vec<C, Num>& operator[](const size_t i) const { return M_[i]; }
    Vec<C, Num>& at(const size_t i) { return M_.at(i); }
    const Vec<C, Num>& at(const size_t i) const { return M_.at(i); }
    Vec<C, Num>& row(const size_t i) { return M_.at(i); }
    const Vec<C, Num>& row(const size_t i) const { return M_.at(i); }
    Vec<R, Num> col(const size_t);
    const Vec<R, Num> col(const size_t) const;

    // size methods
    constexpr size_t nrows() { return R; }
    constexpr size_t ncols() { return C; }
    constexpr Vec<2, size_t> size() const {return std::array<size_t, 2>{R, C};}
};

/*
 * ------------------ Mat ctors and operators ------------------
 */
template <size_t R, size_t C, class Num>
Mat<R, C, Num>::Mat(const Num& val): M_() { for (auto &m: M_) m = val; }

template <size_t R, size_t C, class Num>
Mat<R, C, Num>::Mat(const Vec<C, Num>& v): M_{} { for (auto &m: M_) m = v; }

template <size_t R, size_t C, class Num>
Mat<R, C, Num>::Mat(const std::array<std::array<Num, C>, R> &M): M_() {
    for (size_t i {R}; i--; M_[i] = M[i]) { }
}

template <size_t R, size_t C, class Num>
Mat<R, C, Num>::Mat(const std::initializer_list<Vec<C, Num>> &IL): M_() {
    auto iter = std::begin(IL);
    for (size_t i {0}; i < std::min(R, IL.size()); ++i, ++iter) M_[i] = *iter;
}

template <size_t R, size_t C, class Num>
template <size_t R2, size_t C2, class Comp>
Mat<R, C, Num>::Mat(const Mat<R2, C2, Comp> &o): M_() {
    for (auto i = std::min(R, R2); i--;)
        for (auto j = std::min(C, C2); j--; M_[i][j] = o[i][j]) { }
}

template <size_t R, size_t C, class Num>
template <size_t R2, size_t C2, class Comp>
Mat<R, C, Num>& Mat<R, C, Num>::operator=(const Mat<R2, C2, Comp> &o) {
    if (this != &o)
        for (auto i = std::min(R, R2); i--;)
            for (auto j = std::min(C, C2); j--; M_[i][j] = o[i][j]) { }
    return *this;
}

template <size_t R, size_t C, class Num>
inline Vec<R, Num> Mat<R, C, Num>::col(const size_t j) {
    Vec<R, Num> v;
    for (auto i = R; i--; v[i] = M_[i][j]) { }
    return v;
}

template <size_t R, size_t C, class Num>
inline const Vec<R, Num> Mat<R, C, Num>::col(const size_t j) const {
    Vec<R, Num> v;
    for (auto i = R; i--; v[i] = M_[i][j]) { }
    return v;
}

template <size_t R, size_t C, class Num>
std::istream& operator>>(std::istream &is, Mat<R, C, Num> &M) {
    for (auto &m: M)
        if (!(is >> m)) break;
    return is;
}

template <size_t R, size_t C, class Num>
std::ostream& operator<<(std::ostream &os, const Mat<R, C, Num> &M) {
    os << "{\n";
    for (const auto &m: M) os << m << '\n';
    return os << '}';
}

template <size_t R1, size_t C1, class Num, size_t R2, size_t C2, class Comp>
inline bool operator==(const Mat<R1,C1,Num> &M1, const Mat<R2,C2,Comp> &M2) {
    if (R1 != R2 || C1 != C2) return false;
    for (size_t i {0}; i < R1; ++i)
        if (M1[i] != M2[i]) return false;
    return true;
}

template <size_t R1, size_t C1, class Num, size_t R2, size_t C2, class Comp>
inline bool operator!=(const Mat<R1,C1,Num> &M1, const Mat<R2,C2,Comp> &M2) {
    return !(M1 == M2);
}

/*
 * ------------------ Mat arithmetic compound operators ------------------
 */
// add a value to a matrix
template <size_t R, size_t C, class Num> template <class Comp>
inline Mat<R, C, Num>& Mat<R, C, Num>::operator+=(const Comp &rhs) {
    for (auto &m: M_) m += rhs;
    return *this;
}

// add a column vector to each colum of a matrix (element-wise)
template <size_t R, size_t C, class Num> template <class Comp>
inline Mat<R, C, Num>& Mat<R, C, Num>::operator+=(const Vec<R, Comp> &rhs) {
    for (auto j = C; j--;)
        for (auto i = R; i--; M_[i][j] += rhs[i]) { }
    return *this;
}

// add a matrix to a matrix
template <size_t R, size_t C, class Num> template <class Comp>
inline Mat<R, C, Num>& Mat<R, C, Num>::operator+=(const Mat<R, C, Comp> &rhs) {
    for (auto i = R; i--; M_[i] += rhs[i]) { }
    return *this;
}

// subtract a value from a matrix
template <size_t R, size_t C, class Num> template <class Comp>
inline Mat<R, C, Num>& Mat<R, C, Num>::operator-=(const Comp &rhs) {
    for (auto &m: M_) m -= rhs;
    return *this;
}

// subtract a column from each colum of a matrix (element-wise)
template <size_t R, size_t C, class Num> template <class Comp>
inline Mat<R, C, Num>& Mat<R, C, Num>::operator-=(const Vec<R, Comp> &rhs) {
    for (auto j = C; j--;)
        for (auto i = R; i--; M_[i][j] -= rhs[i]) { }
    return *this;
}

// subtract a matrix from a matrix
template <size_t R, size_t C, class Num> template <class Comp>
inline Mat<R, C, Num>& Mat<R, C, Num>::operator-=(const Mat<R, C, Comp> &rhs) {
    for (auto i = R; i--; M_[i] -= rhs[i]) { }
    return *this;
}

// multiply a matrix by a value
template <size_t R, size_t C, class Num> template <class Comp>
inline Mat<R, C, Num>& Mat<R, C, Num>::operator*=(const Comp &rhs) {
    for (auto &m: M_) m *= rhs;
    return *this;
}

// divide a matrix by a value
template <size_t R, size_t C, class Num> template <class Comp>
inline Mat<R, C, Num>& Mat<R, C, Num>::operator/=(const Comp &rhs) {
    for (auto &m: M_) m /= rhs;
    return *this;
}

/*
 * ------------------ Mat arithmetic operators ------------------
 */
// add a value to a matrix
template <size_t R, size_t C, class Num, class Comp>
inline Mat<R, C, class std::common_type<Num, Comp>::type>
operator+(const Mat<R, C, Num> &lhs, const Comp &rhs) {
    Mat<R, C, class std::common_type<Num, Comp>::type> M {lhs};
    return M += rhs;
}

// add a column vector to each colum of a matrix (element-wise)
template <size_t R, size_t C, class Num, class Comp>
inline Mat<R, C, class std::common_type<Num, Comp>::type>
operator+(const Mat<R, C, Num> &lhs, const Vec<R, Comp> &rhs) {
    Mat<R, C, class std::common_type<Num, Comp>::type> M {lhs};
    return M += rhs;
}

// add a matrix to a matrix
template <size_t R, size_t C, class Num, class Comp>
inline Mat<R, C, class std::common_type<Num, Comp>::type>
operator+(const Mat<R, C, Num> &lhs, const Mat<R, C, Comp> &rhs) {
    Mat<R, C, class std::common_type<Num, Comp>::type> M {lhs};
    return M += rhs;
}

// subtract a value from a matrix
template <size_t R, size_t C, class Num, class Comp>
inline Mat<R, C, class std::common_type<Num, Comp>::type>
operator-(const Mat<R, C, Num> &lhs, const Comp &rhs) {
    Mat<R, C, class std::common_type<Num, Comp>::type> M {lhs};
    return M -= rhs;
}

// subtract a column vector from each colum of a matrix (element-wise)
template <size_t R, size_t C, class Num, class Comp>
inline Mat<R, C, class std::common_type<Num, Comp>::type>
operator-(const Mat<R, C, Num> &lhs, const Vec<R, Comp> &rhs) {
    Mat<R, C, class std::common_type<Num, Comp>::type> M {lhs};
    return M -= rhs;
}

// subtract a matrix from a matrix
template <size_t R, size_t C, class Num, class Comp>
inline Mat<R, C, class std::common_type<Num, Comp>::type>
operator-(const Mat<R, C, Num> &lhs, const Mat<R, C, Comp> &rhs) {
    Mat<R, C, class std::common_type<Num, Comp>::type> M {lhs};
    return M -= rhs;
}

// multiply a matrix by a value
template <size_t R, size_t C, class Num, class Comp>
inline Mat<R, C, class std::common_type<Num, Comp>::type>
operator*(const Mat<R, C, Num> &lhs, const Comp &rhs) {
    Mat<R, C, class std::common_type<Num, Comp>::type> M {lhs};
    return M *= rhs;
}

// divide a matrix by a value
template <size_t R, size_t C, class Num, class Comp>
inline Mat<R, C, class std::common_type<Num, Comp>::type>
operator/(const Mat<R, C, Num> &lhs, const Comp &rhs) {
    Mat<R, C, class std::common_type<Num, Comp>::type> M {lhs};
    return M /= rhs;
}

// multiply matrix by a vector: output is a vector
template <size_t R, size_t C, class Num, class Comp>
inline Vec<R, class std::common_type<Num, Comp>::type>
operator*(const Mat<R, C, Num> &lhs, const Vec<C, Comp> &rhs) {
    Vec<R, class std::common_type<Num, Comp>::type> v;
    for (auto i = R; i--; v[i] = lhs[i] * rhs) { }
    return v;
}

// multiply matrix by a matrix
template <size_t R, size_t RC, size_t C, class Num, class Comp>
inline Mat<R, C, class std::common_type<Num, Comp>::type>
operator*(const Mat<R, RC, Num> &lhs, const Mat<RC, C, Comp> &rhs) {
    Mat<R, C, class std::common_type<Num, Comp>::type> M;
    for (auto i = R; i--;)
    for (auto j = C; j--; M[i][j] = lhs[i] * rhs.col(j)) { }
    return M;
}

/*
 * ------------------ Mat functions ------------------
 */
template <size_t N>
Mat<N, N, int> eye() {
    Mat<N, N, int> M (0);
    for (auto i = N; i--; M[i][i] = 1) { }
    return M;
}



#endif

