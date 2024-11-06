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
class Rational
{
  public:
    using value_type = std::intmax_t;
    // using value_type = int;

    // Placing private parts before public for learning purposes.
  private:
    value_type m_num; // Numerator.
    value_type m_den; // Denominator.

    static constexpr value_type m_default_numerator {0};
    static constexpr value_type m_default_denominator {1};

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

    constexpr Rational& m_add(value_type num, value_type den)
    {
        m_num = m_num * den + m_den * num;
        m_den *= den;

        normalize_by_gcd(m_num, m_den);

        return *this;
    }

    template <typename T>
    constexpr T m_cast_to() const
    {
        return static_cast<T>(m_num) / m_den;
    }

  public:
    /***************************************************************************
     * Constructors section                                                    *
     **************************************************************************/
    explicit constexpr Rational(const value_type num = m_default_numerator,
        const value_type den = m_default_denominator)
        : m_num {num}, m_den {den}
    {
        if (den == 0)
        {
//            throw bad_rational {};
        }

        m_fix_sign();

        normalize_by_gcd(m_num, m_den);
    }

    //explicit constexpr Rational(Rational& other)
    //    : m_num {other.m_num}, m_den {other.m_den}
    //{
    //}

    //explicit Rational(Rational&& other)
    //    : m_num {other.m_num}, m_den {other.m_den}
    //{
    //    std::cout << "Calling move constructor\n";
    //}

    /***************************************************************************
     * End of Constructors section                                             *
     **************************************************************************/

    /***************************************************************************
     * Getters section                                                         *
     **************************************************************************/

    // Using ref-qualification to have two versions - one for const l-value
    // reference and one for right-value. The & qualifer overloads function to
    // match only l-value implicit objects. The && qualifer overloads function
    // to match only r-value implicit objects.

    /**
     * @brief Get numerator of the rational number.
     *
     * @return constexpr const value_type& Const l-value reference to the
     * numerator value.
     */
    constexpr const value_type& numerator() const&
    {
        return m_num;
    }

    /**
     * @brief Get numerator of the rational number.
     *
     * @return constexpr value_type r-value of the numerator value.
     */
    constexpr value_type numerator() const&&
    {
        return std::move(m_num);
    }

    /**
     * @brief Get denominator of the rational number.
     *
     * @return constexpr const value_type& Const l-value reference to the
     * denominator value.
     */
    constexpr const value_type& denominator() const&
    {
        return m_den;
    }

    /**
     * @brief Get denominator of the rational number.
     *
     * @return constexpr value_type r-value of the denominator value.
     */
    constexpr value_type denominator() const&&
    {
        return std::move(m_den);
    }

    /***************************************************************************
     * End of Getters section                                                  *
     **************************************************************************/

    /***************************************************************************
     * Boolean operators section                                               *
     **************************************************************************/

    constexpr bool is_positive_infinity() const
    {
        return (m_num > 0) && (m_den == 0);
    }

    constexpr bool is_negative_infinity() const
    {
        return (m_num < 0) && (m_den == 0);
    }

    constexpr bool is_infinity() const
    {
        return (m_num != 0) && (m_den == 0);
    }

    constexpr bool is_nan() const
    {
        return (m_num == 0) && (m_den == 0);
    }

    constexpr bool operator==(const Rational& other) const
    {
        return (m_num == other.numerator()) && (m_den == other.denominator());
    }

    constexpr bool operator!=(const Rational& other) const
    {
        return !(*this == other);
    }

    constexpr bool operator<(const Rational& other) const
    {
        return m_num * other.denominator() < m_den * other.numerator();
    }

    constexpr bool operator>(const Rational& other) const
    {
        return (other < *this);
    }

    constexpr bool operator<=(const Rational& other) const
    {
        return !(*this > other);
    }

    constexpr bool operator>=(const Rational& other) const
    {
        return !(*this < other);
    }

    /***************************************************************************
     * End of Boolean operators section                                        *
     **************************************************************************/

    /***************************************************************************
     * Typecasts operators section                                             *
     **************************************************************************/

    constexpr operator float() const
    {
        return m_cast_to<float>();
    }

    constexpr operator double() const
    {
        return m_cast_to<double>();
    }

    constexpr operator long double() const
    {
        return m_cast_to<long double>();
    }

    /***************************************************************************
     * End of Typecasts operators section                                      *
     **************************************************************************/

    /***************************************************************************
     * Mathematical operators section                                          *
     **************************************************************************/

    /**
     * @brief Compound addition operator.
     * @remark One possible modification is to find gcd for denominators. That
     * way we could have lesser multipliers, thus reducing the risk of overflow.
     *
     * @param[in] other Number to sum with.
     * @return constexpr Rational& reference to the currrent instance.
     */
    constexpr Rational& operator+=(const Rational& other)
    {
        return m_add(other.numerator(), other.denominator());
    }

    /**
     * @brief Compound subtraction operator.
     * @remark Same as of += operator.
     *
     * @param[in] other Number to subtract.
     * @return constexpr Rational& reference to the currrent instance.
     */
    constexpr Rational& operator-=(const Rational& other)
    {
        return m_add(-other.numerator(), other.denominator());
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

constexpr auto operator+(const auto& lhs, const auto& rhs)
{
    return Rational {lhs} += rhs;
}

/**
 * @brief Calculate the reciprocal of a rational number.
 *
 * @param[in] number Number to use for calculation (basically, swapping
 * numerator and denominator).
 * @return constexpr Rational number.
 */
inline constexpr Rational reciprocal(const Rational& number)
{
    return Rational {number.denominator(), number.numerator()};
}

/**
 * @brief Overload output operator for rational numbers.
 *
 * @param[in] stream Stream to ouput to.
 * @param[in] number Number to output.
 * @return std::ostream& Stream reference to return allowing chaining.
 */
std::ostream& operator<<(std::ostream& stream, const Marvin::Rational& number)
{
    stream << number.numerator() << "/" << number.denominator();
    return stream;
}

} // namespace Marvin

#endif // H_NUMBERS_INCLUDE_RATIONAL_H_H
