#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "Vec.h"

template <size_t R, size_t C, class T> class Matrix; // forward declaration
/*
 * ---- Determinant structure ----
 */
template<size_t DIM,class T> struct dt {
    static T det(const Matrix<DIM,DIM,T>& src) {
        T ret=0;
        for (size_t i=DIM; i--; ret += src[0][i]*src.cofactor(0,i));
        return ret;
    }
};

template<class T> struct dt<1,T> {
    static T det(const Matrix<1,1,T>& src) {
        return src[0][0];
    }
};

/*
 * ---- Matrix class ----
 */
template <size_t R, size_t C, class T>
class Matrix {
public:
    Matrix(const T &t = T{}): m_() { for (size_t r {R}; r--; m_[r] = {t}); }

    Vec<C, T>& operator[](const int i) { return m_[i]; }
    const Vec<C, T>& operator[](const int i) const { return m_[i]; }

    Vec<R, T> column(const size_t i) const {
        Vec<R, T> res;
        for (size_t r {R}; r--; res[r] = m_[r][i]) { }
        return res;
    }

    T det() const { return dt<C, T>::det(*this); }

    Matrix<R-1, C-1, T> get_minor(const size_t row, const size_t col) const {
        Matrix<R - 1, C - 1, T> res;
        for (size_t r {R - 1}; r--;)
            for (size_t c {C - 1}; c--; res[r][c] =
                    m_[r < row ? r : r + 1][c < col ? c : c + 1]) { }
        return res;
    }

    T cofactor(const size_t r, const size_t c) const {
        return get_minor(r, c).det() * ((r + c) & 1 ? -1 : 1);
    }

    Matrix<R, C, T> adjugate() const {
        Matrix<R, C, T> res;
        for (size_t r {R}; r--;)
            for (size_t c {C}; c--; res[r][c] = cofactor(r, c)) { }
        return res;
    }

    Matrix<R, C, T> invert_transpose() const {
        Matrix<R, C, T> res {adjugate()};
        T t {res[0] * m_[0]};
        return res / t;
    }

    Matrix<R, C, T> transpose() const {
        Matrix<R, C, T> res;
        for (size_t c {C}; c--; res[c] = this->column(c)) { }
        return res;
    }

    Matrix<R, C, T> invert() const { return invert_transpose().transpose(); }
private:
    Vec<R, Vec<C, T>> m_;
};


template <size_t R, size_t C, class T>
std::ostream& operator<<(std::ostream &os, const Matrix<R, C, T> &m) {
    os << "{\n";
    for (size_t i {0}; i < R; ++i) os << m[i] << '\n';
    return os << '}';
}


#endif

