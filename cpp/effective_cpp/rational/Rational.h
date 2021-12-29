#ifndef _RATIONAL_H_
#define _RATIONAL_H_

#include <stdexcept>
#include <type_traits>

class Bad_rational: public std::domain_error {
public:
    explicit Bad_rational():
        std::domain_error {"Bad rational: zero denominator"} {
    }
};

template <class Num = int>
class Rational {
    static_assert(std::is_integral<Num>::value,
            "template Rational<Num> takes only integral type as a parameter");
    public:
        using value_type = Num;
        // default numerator is zero and default denominator is one to make the
        // constructor able to present integers with one argument
        constexpr Rational(const Num = Num {0}, const Num = Num {1});

        Rational(const Rational&) = default;
        Rational(Rational&&) = default;
        Rational& operator=(const Rational&) = default;
        Rational& operator=(Rational&&) = default;

        ~Rational() = default;

        constexpr value_type numer() const { return num_; }
        constexpr value_type denom() const { return den_; }

        //Rational operator+(const Rational&);
        //Rational operator-(const Rational&);
        //Rational operator*(const Rational&);
        //Rational operator/(const Rational&);
        //const Rational inverse() const;

        /*
         * Declaring and defining multiplication operator as a friend function
         * in order to allow implicit conversions (e.g, 2 * Rational<int>()).
         * That is necessary, because the class is templated.
         * The access to private member is not important here.
         */
        friend constexpr Rational operator*(const Rational& lhs,
                const Rational& rhs) {
            return Rational { lhs.num_ * rhs.num_, lhs.den_ * rhs.den_ };
        }

        //Rational &simplify();
        ////const Rational simplify();
        //void normalize();
        //float to_float() const;
        //double to_double() const;

    private:
        value_type num_;
        value_type den_;
};

template <class Num>
constexpr Rational<Num>::Rational(const Num num, const Num den):
    num_ {num}, den_ {den} { }

template <class T>
std::ostream& operator<<(std::ostream& os, const Rational<T>& r) {
    return os << r.numer() << '/' << r.denom();
}

#endif

