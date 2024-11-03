#ifndef H_INCLUDE_NUMBERS_RATIONALTEMPLATED_H_H
#define H_INCLUDE_NUMBERS_RATIONALTEMPLATED_H_H

namespace Marvin
{

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
template <typename T> class RationalTemplated
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
    explicit constexpr RationalTemplated(const value_type num = m_default_numerator,
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
    constexpr const value_type& numerator() const &
    {
        return m_num;
    }

    /**
     * @brief Get numerator of the rational number.
     *
     * @return constexpr value_type r-value of the numerator value.
     */
    constexpr value_type numerator() const &&
    {
        return std::move(m_num);
    }

    /**
     * @brief Get denominator of the rational number.
     *
     * @return constexpr const value_type& Const l-value reference to the
     * denominator value.
     */
    constexpr const value_type& denominator() const &
    {
        return m_den;
    }

    /**
     * @brief Get denominator of the rational number.
     *
     * @return constexpr value_type r-value of the denominator value.
     */
    constexpr value_type denominator() const &&
    {
        return std::move(m_den);
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
    constexpr bool operator==(const RationalTemplated& other) const
    {
        return (m_num == other.numerator()) && (m_den == other.denominator());
    }

    constexpr bool operator!=(const RationalTemplated& other) const
    {
        return !(operator==(other));
    }

    constexpr bool operator<(const RationalTemplated& other) const
    {
        return m_num * other.denominator() < m_den * other.numerator();
    }

    constexpr bool operator>(const RationalTemplated& other) const
    {
        return m_num * other.denominator() > m_den * other.numerator();
    }

    constexpr bool operator<=(const RationalTemplated& other) const
    {
        return !(operator>(other));
    }

    constexpr bool operator>=(const RationalTemplated& other) const
    {
        return !(operator<(other));
    }

    /***************************************************************************
     * End of Boolean operators section                                        *
     **************************************************************************/

    /***************************************************************************
     * Mathematical operators section                                          *
     **************************************************************************/
    constexpr RationalTemplated& operator+=(const RationalTemplated& other)
    {
        m_num = m_num * other.denominator() + m_den * other.numerator();
        m_den *= other.denominator();

        normalize_by_gcd(m_num, m_den);

        return *this;
    }

    constexpr RationalTemplated& operator-=(const RationalTemplated& other)
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
     * @return constexpr RationalTemplated& reference to the currrent instance.
     */
    constexpr RationalTemplated& operator*=(const RationalTemplated& other)
    {
#if 1
        m_num *= other.numerator();
        m_den *= other.denominator();

        normalize_by_gcd(m_num, m_den);
#else
        const RationalTemplated a {m_num, other.denominator()};
        const RationalTemplated b {other.numerator(), m_den};

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
     * @param[in] other RationalTemplated number to divide by.
     * @return constexpr RationalTemplated& reference to the currrent instance.
     */
    constexpr RationalTemplated& operator/=(const RationalTemplated& other)
    {
#if 1
        m_num *= other.denominator();
        m_den *= other.numerator();

        normalize_by_gcd(m_num, m_den);
#else
        const RationalTemplated a {m_num, other.numerator()};
        const RationalTemplated b {other.denominator(), m_den};

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

template <typename T, typename U>
constexpr RationalTemplated<std::common_type_t<T, U>> operator+(
    const RationalTemplated<T>& lhs, const RationalTemplated<T>& rhs)
{
    return RationalTemplated {lhs} += rhs;
}

/**
 * @brief Calculate the reciprocal of a rational number.
 *
 * @tparam T Inner type.
 * @param[in] number Number to use for calculation (basically, swapping
 * numerator and denominator).
 * @return constexpr RationalTemplated<T>
 */
template <typename T>
inline constexpr RationalTemplated<T> reciprocal(const RationalTemplated<T>& number)
{
    return RationalTemplated {number.denominator(), number.numerator()};
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
    std::ostream& stream, const Marvin::RationalTemplated<T>& number)
{
    stream << number.numerator() << "/" << number.denominator();
    return stream;
}

} // namespace Marvin

#endif // H_INCLUDE_NUMBERS_RATIONALTEMPLATED_H_H
