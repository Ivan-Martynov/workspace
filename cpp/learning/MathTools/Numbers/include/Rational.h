#ifndef H_NUMBERS_INCLUDE_RATIONAL_H_H
#define H_NUMBERS_INCLUDE_RATIONAL_H_H

#include <iostream>
#include <cstdint> // For std::intmax_t.
#include <numeric> // For std::gcd.

namespace Marvin
{

/**
 * @brief Simple class representing rational numbers with numerator and
 * denominator as integers.
 *
 */
template <typename T = std::intmax_t>
class Rational
{
  private:
    T m_num; // Numerator.
    T m_den; // Denominator.

    /**
     * @brief Simplify the fraction using a divisor - greatest common divisor.
     *
     * @param[in] divisor Greatest common divisor.
     */
    constexpr void m_simplify(const T divisor)
    {
        // If the denominator is less than zero, then move the minus sign to
        // numerator. Therefore, if the numerator was negative, it becomes
        // positive. If it was zero, it remains zero.
        if (m_den < 0)
        {
            m_num = -m_num;
            m_den = -m_den;
        }

        m_num /= divisor;
        m_den /= divisor;
    }

  public:
    /***************************************************************************
     * Constructors section                                                    *
     **************************************************************************/
    explicit constexpr Rational(const T num = 0, const T den = 1)
        : m_num {den ? num : 0}, m_den {den ? den : 1}
    {
        if (den == 0)
        {
            std::cerr << "Denominator cannot be zero: setting to 0/1.\n";
        }
        else
        {
            m_simplify(std::gcd(m_num, m_den));
        }
    }

    explicit constexpr Rational(const Rational& other)
        : m_num {other.numerator()}, m_den {other.denominator()}
    {
    }

    constexpr Rational& operator=(const Rational& other)
    {
        if (this != other)
        {
            m_num = other.numerator();
            m_den = other.denominator();
        }

        return *this;
    }

    /***************************************************************************
     * End of Constructors section                                             *
     **************************************************************************/

    /***************************************************************************
     * Getters section                                                         *
     **************************************************************************/
    constexpr T numerator() const
    {
        return m_num;
    }

    constexpr T denominator() const
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

        m_simplify(std::gcd(m_num, m_den));

        return *this;
    }

    constexpr Rational& operator-=(const Rational& other)
    {
        m_num = m_num * other.denominator() - m_den * other.numerator();
        m_den *= other.denominator();

        m_simplify(std::gcd(m_num, m_den));

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

        m_simplify(std::gcd(m_num, m_den));
#else
        const Rational a {m_num, other.denominator()};
        const Rational b {other.numerator(), m_den};

        m_num = a.numerator() * b.numerator();
        m_den = a.denominator() * b.denominator();
#endif

        return *this;
    }
    /***************************************************************************
     * End of Mathematical operators section                                   *
     **************************************************************************/
};


} // namespace Marvin

#endif // H_NUMBERS_INCLUDE_RATIONAL_H_H
