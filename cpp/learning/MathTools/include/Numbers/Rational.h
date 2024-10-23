#ifndef H_NUMBERS_INCLUDE_RATIONAL_H_H
#define H_NUMBERS_INCLUDE_RATIONAL_H_H

#include "Algorithms/Numerical.h" // For gcd.

#include <iostream>
#include <cstdint> // For std::intmax_t.

namespace Marvin
{

class bad_rational : public std::domain_error
{
  public:
    explicit bad_rational()
        : std::domain_error("Bad rational: zero denominator")
    {
    }
};

/**
 * @brief Simple class representing rational numbers with numerator and
 * denominator as integers.
 * @remarks The task is to decide how to handle cases, when the denominator is
 * zero:
 * - Forbidding creation of such numbers? Inform the user? Set to some default
 * value (for example, 0/1)?
 * - Allow and treat values like n/0 and -n/0 as Infinity and -Infinity?
 * - Allow having 0/0 value to represent Not-A-Number (aka NAN)?
 * Another issue is wheter to make it a template class or using std::intmax_t
 * as the value type. Templated class allows using smaller types - worth it?
 */
template <typename T = std::intmax_t> class Rational
//class Rational
{
  public:
    //using value_type = std::intmax_t;
    using value_type = T;

    // Placing private parts before public for learning purposes.
  private:
    value_type m_num; // Numerator.
    value_type m_den; // Denominator.

    static constexpr T m_default_numerator {static_cast<T>(0)};
    static constexpr T m_default_denominator {static_cast<T>(1)};

    /**
     * @brief Fix the sign of the rational number, so that the denominator is
     * never negative.
     */
    constexpr void m_fix_sign()
    {
        // If the denominator is less than zero, then move the minus sign to
        // numerator. Therefore, if the numerator was negative, it becomes
        // positive. If it was zero, it remains zero.
        if (m_den < 0)
        {
            m_num = -m_num;
            m_den = -m_den;
        }
    }

  public:
    /***************************************************************************
     * Constructors section                                                    *
     **************************************************************************/
    explicit constexpr Rational(const value_type num = m_default_numerator,
        const value_type den = m_default_denominator)
        : m_num {num}, m_den {den}
    {
        if (den == static_cast<T>(0))
        {
            throw bad_rational {};
        }

        m_fix_sign();

        normalize_by_gcd(m_num, m_den);
    }

    /***************************************************************************
     * End of Constructors section                                             *
     **************************************************************************/

    /***************************************************************************
     * Getters section                                                         *
     **************************************************************************/
    constexpr value_type numerator() const
    {
        return m_num;
    }

    constexpr value_type denominator() const
    {
        return m_den;
    }
    /***************************************************************************
     * End of Getters section                                                  *
     **************************************************************************/

    /***************************************************************************
     * Boolean operators section                                               *
     **************************************************************************/
    /**
     * @brief Equality operator.
     *
     * @param[in] other Rational number to compare to.
     * @return true if numbers are equal.
     * @return false if numbers are not equal.
     */
    constexpr bool operator==(const Rational& other) const
    {
        return (m_num == other.numerator()) && (m_den == other.denominator());
    }

    constexpr bool operator!=(const Rational& other) const
    {
        return !(operator==(other));
    }

    constexpr bool operator<(const Rational& other) const
    {
        return m_num * other.denominator() < m_den * other.numerator();
    }

    constexpr bool operator<=(const Rational& other) const
    {
        return m_num * other.denominator() <= m_den * other.numerator();
    }

    constexpr bool operator>(const Rational& other) const
    {
        return !(operator<=(other));
    }

    constexpr bool operator>=(const Rational& other) const
    {
        return !(operator<(other));
    }

    /***************************************************************************
     * End of Boolean operators section                                        *
     **************************************************************************/

    /***************************************************************************
     * Mathematical operators section                                          *
     **************************************************************************/
    constexpr Rational& operator+=(const Rational& other)
    {
        m_num = m_num * other.denominator() + m_den * other.numerator();
        m_den *= other.denominator();

        normalize_by_gcd(m_num, m_den);

        return *this;
    }

    constexpr Rational& operator-=(const Rational& other)
    {
        m_num = m_num * other.denominator() - m_den * other.numerator();
        m_den *= other.denominator();

        normalize_by_gcd(m_num, m_den);

        return *this;
    }

    /**
     * @brief Compound multiplication operator.
     * @remark The multiplication before applying gcd simplification might
     * overflow. Might consider simpifying numerator with second number's
     * denominator and denominator with second number's numerator. Then the
     * product might be smaller. Also, there will be no need for final
     * simplification. However, if numbers aren't reducable, there might be
     * overflow anyway.
     *
     * @param[in] other Rational number to multiply by.
     * @return constexpr Rational& reference to the currrent instance.
     */
    constexpr Rational& operator*=(const Rational& other)
    {
#if 1
        m_num *= other.numerator();
        m_den *= other.denominator();

        normalize_by_gcd(m_num, m_den);
#else
        const Rational a {m_num, other.denominator()};
        const Rational b {other.numerator(), m_den};

        m_num = a.numerator() * b.numerator();
        m_den = a.denominator() * b.denominator();
#endif

        return *this;
    }

    /**
     * @brief Compound division operator.
     * @remark The division before applying gcd simplification might
     * overflow. Might consider simpifying numerator with second number's
     * numerator and denominator with second number's denominator. Then the
     * product might be smaller. Also, there will be no need for final
     * simplification. However, if numbers aren't reducable, there might be
     * overflow anyway.
     *
     * @param[in] other Rational number to divide by.
     * @return constexpr Rational& reference to the currrent instance.
     */
    constexpr Rational& operator/=(const Rational& other)
    {
#if 1
        m_num *= other.denominator();
        m_den *= other.numerator();

        normalize_by_gcd(m_num, m_den);
#else
        const Rational a {m_num, other.numerator()};
        const Rational b {other.denominator(), m_den};

        m_num = a.numerator() * b.numerator();
        m_den = a.denominator() * b.denominator();
#endif
        m_fix_sign();

        return *this;
    }

    /***************************************************************************
     * End of Mathematical operators section                                   *
     **************************************************************************/

    /**
     * @brief Invert the fraction in place.
     */
    constexpr void invert()
    {
        std::swap(m_num, m_den);
        m_fix_sign();
    }
};

/**
 * @brief Calculate the reciprocal of a rational number.
 *
 * @tparam T Inner type.
 * @param[in] number Number to use for calculation (basically, swapping
 * numerator and denominator).
 * @return constexpr Rational<T>
 */
template <typename T>
inline constexpr Rational<T> reciprocal(const Rational<T>& number)
{
    return Rational {number.denominator(), number.numerator()};
}

/**
 * @brief Overload output operator for rational numbers.
 * 
 * @tparam T Inner type.
 * @param[in] stream Stream to ouput to.
 * @param[in] number Number to output.
 * @return std::ostream& Stream reference to return allowing chaining.
 */
template <typename T>
std::ostream& operator<<(
    std::ostream& stream, const Marvin::Rational<T>& number)
{
    stream << number.numerator() << "/" << number.denominator();
    return stream;
}

} // namespace Marvin

#endif // H_NUMBERS_INCLUDE_RATIONAL_H_H
