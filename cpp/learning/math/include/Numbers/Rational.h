#ifndef H_NUMBERS_INCLUDE_RATIONAL_H_H
#define H_NUMBERS_INCLUDE_RATIONAL_H_H

#include "Algorithms/Numerical.h" // For gcd.

#include <iostream>
#include <cstdint> // For std::intmax_t.

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
template <typename T>
    requires std::is_arithmetic_v<T>
class RationalTemplated
{
  public:
    using value_type = T;

    /**
     * @brief Type representing whether the number is normal, inifinity
     * (positive or negative) or an invalid number (NAN: Not-A-Number).
     *
     */
    enum class Type
    {
        normal,
        positive_infinity,
        negative_infinity,
        nan,
    };

    /***************************************************************************
     * Constructors section                                                    *
     **************************************************************************/

    /**
     * @brief Constructor taking numerator and denominator values as
     * parameteres. Default values are zero for the numerator and one for the
     * denominator.
     *
     * @remarks Make sure the sign of the number is represented by numerator.
     * Normalize the number using greatest common divisor.
     *
     * @param[in] num Numerator.
     * @param[in] den Denominator.
     */
    explicit constexpr RationalTemplated(
        const value_type num = m_default_numerator,
        const value_type den = m_default_denominator)
        : m_num {num}, m_den {den}
    {
        m_fix_sign();
        normalize_by_gcd(m_num, m_den);
    }

    /**
     * @brief Copy constructor.
     *
     * @remark No need to modify the number, because the source number is
     * already in the proper form.
     *
     * @param[in] other Number to copy from.
     */
    explicit constexpr RationalTemplated(const RationalTemplated& other)
        : m_num {other.numerator()}, m_den {other.denominator()}
    {
    }

    /**
     * @brief Move constructor.
     *
     * @param[in] other Number to copy from.
     */
    explicit constexpr RationalTemplated(RationalTemplated&& other)
        : m_num {std::move(other.numerator())},
          m_den {std::move(other.denominator())}
    {
    }

    /**
     * @brief Copy assignment operator.
     *
     * @param[in] other Number to copy from.
     * @return constexpr RationalTemplated& Reference to the current instance.
     */
    constexpr RationalTemplated& operator=(const RationalTemplated& other)
    {
        if (this != &other)
        {
            m_num = other.numerator();
            m_den = other.denominator();
        }
        return *this;
    }

    /**
     * @brief Move assignment operator.
     *
     * @param[in] other Number to copy from.
     * @return constexpr RationalTemplated& Reference to the current instance.
     */
    constexpr RationalTemplated& operator=(const RationalTemplated&& other)
    {
        m_num = std::move(other.numerator());
        m_den = std::move(other.denominator());
        return *this;
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
    constexpr const value_type& numerator() const& { return m_num; }

    /**
     * @brief Get numerator of the rational number.
     *
     * @return constexpr value_type r-value of the numerator value.
     */
    constexpr value_type numerator() const&& { return std::move(m_num); }

    /**
     * @brief Get denominator of the rational number.
     *
     * @return constexpr const value_type& Const l-value reference to the
     * denominator value.
     */
    constexpr const value_type& denominator() const& { return m_den; }

    /**
     * @brief Get denominator of the rational number.
     *
     * @return constexpr value_type r-value of the denominator value.
     */
    constexpr value_type denominator() const&& { return std::move(m_den); }

    /**
     * @brief Get type of the number, meaning whether it is a normal value,
     * infinity (positive or negative) or nan.
     *
     * @return constexpr Type type of the number.
     */
    constexpr Type type() const
    {
        if (m_den != m_zero)
        {
            return Type::normal;
        }
        else if (m_num > 0)
        {
            return Type::positive_infinity;
        }
        else if (m_num < 0)
        {
            return Type::negative_infinity;
        }
        else
        {
            return Type::nan;
        }
    }

    /***************************************************************************
     * End of Getters section                                                  *
     **************************************************************************/

    /***************************************************************************
     * Boolean operators section                                               *
     **************************************************************************/

    /**
     * @brief Check if the number represents a positive infinity.
     *
     * @return true The number represents a positive infinity.
     * @return false The number doesn't represents a positive infinity.
     */
    constexpr bool is_positive_infinity() const
    {
        return type() == Type::positive_infinity;
    }

    /**
     * @brief Check if the number represents a negative infinity.
     *
     * @return true The number represents a negative infinity.
     * @return false The number doesn't represents a negative infinity.
     */
    constexpr bool is_negative_infinity() const
    {
        return type() == Type::negative_infinity;
    }

    /**
     * @brief Check if the number represents infinity.
     *
     * @return true The number represents infinity.
     * @return false The number doesn't represents infinity.
     */
    constexpr bool is_infinity() const
    {
        const auto t {type()};
        return (t == Type::positive_infinity) && (t == Type::negative_infinity);
    }

    /**
     * @brief Check if the number represents an invalid number (NAN: that is,
     * Not-A-Number).
     *
     * @return true The number represents NAN.
     * @return false The number doesn't represents NAN.
     */
    constexpr bool is_nan() const { return type() == Type::nan; }

    /**
     * @brief Equality operator.
     *
     * @remarks Nan is not equal to Nan (or any other number). Positivie
     * infinity equals to positive infinity. The same for the negative infinity.
     *
     * @param[in] other Rational number to compare to.
     * @return true if numbers are equal.
     * @return false if numbers are not equal.
     */
    constexpr bool operator==(const RationalTemplated& other) const
    {
        return !m_nans(other) && (m_num == other.numerator())
               && (m_den == other.denominator());
    }

    /**
     * @brief Inequality operator.
     *
     * @param[in] other Rational number to compare to.
     * @return true if numbers are not equal.
     * @return false if numbers are equal.
     */
    constexpr bool operator!=(const RationalTemplated& other) const
    {
        return !(operator==(other));
    }

    /**
     * @brief Less than operator.
     *
     * @param[in] other Number to compare with.
     * @return true if the current number is smaller than the other.
     * @return false if the current number is not smaller than the other.
     */
    constexpr bool operator<(const RationalTemplated& other) const
    {
        return !m_nans(other)
               && (is_negative_infinity()
                   || (m_num * other.denominator()
                       < m_den * other.numerator()));
    }

    /**
     * @brief Greater than operator.
     *
     * @param[in] other Number to compare with.
     * @return true if the current number is greater than the other.
     * @return false if the current number is not greater than the other.
     */
    constexpr bool operator>(const RationalTemplated& other) const
    {
        return !m_nans(other) && (other < *this);
    }

    /**
     * @brief Less than or equal operator.
     *
     * @param[in] other Number to compare with.
     * @return true if the current number is smaller or equal than the other.
     * @return false if the current number is not smaller or equal than the
     * other.
     */
    constexpr bool operator<=(const RationalTemplated& other) const
    {
        return !m_nans(other) && !(operator>(other));
    }

    /**
     * @brief Greater than or equal operator.
     *
     * @param[in] other Number to compare with.
     * @return true if the current number is greater or equal than the other.
     * @return false if the current number is not greater or equal than the
     * other.
     */
    constexpr bool operator>=(const RationalTemplated& other) const
    {
        return !m_nans(other) && !(operator<(other));
    }

    /***************************************************************************
     * End of Boolean operators section                                        *
     **************************************************************************/

    /***************************************************************************
     * Typecasts operators section                                             *
     **************************************************************************/

    /**
     * @brief Cast the rational number to a single value of the specified type.
     *
     * @tparam U An arithmetic type.
     * @return U The converted value.
     */
    template <typename U>
        requires std::is_arithmetic_v<U>
    constexpr operator U() const
    {
        return static_cast<U>(m_num) / m_den;
    }

    /***************************************************************************
     * End of Typecasts operators section                                      *
     **************************************************************************/

    /***************************************************************************
     * Mathematical operators section                                          *
     **************************************************************************/
    constexpr RationalTemplated& operator+=(const RationalTemplated& other)
    {
        return m_add(other.numerator(), other.denominator());
    }

    constexpr RationalTemplated& operator-=(const RationalTemplated& other)
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

  private:
    value_type m_num; // Numerator.
    value_type m_den; // Denominator.

    static constexpr value_type m_zero {static_cast<value_type>(0)};

    static constexpr value_type m_default_numerator {m_zero};
    static constexpr value_type m_default_denominator {
        static_cast<value_type>(1)};

    /**
     * @brief Fix the sign of the rational number, so that the denominator is
     * never negative.
     */
    constexpr void m_fix_sign()
    {
        // If the denominator is less than zero, then move the minus sign to
        // numerator. Therefore, if the numerator was negative, it becomes
        // positive. If it was zero, it remains zero.
        if (m_den < m_zero)
        {
            m_num = -m_num;
            m_den = -m_den;
        }
    }

    /**
     * @brief Helper function for addition and subtraction (for subtraction
     * using negative num).
     *
     * @param[in] num Numerator of another number.
     * @param[in] den Denominator of another number.
     *
     * @return constexpr Rational& Reference to the current instance.
     */
    constexpr RationalTemplated& m_add(value_type num, value_type den)
    {
        m_num = m_num * den + m_den * num;
        m_den *= den;

        normalize_by_gcd(m_num, m_den);

        return *this;
    }

    /**
     * @brief Helper function to inform if one of the numbers is a NAN, then any
     * comparison (=, <, <=, >, >=) between these two numbers should return
     * false, except inequality (!=), which should always return true;
     *
     * @param[in] other Number to compare with.
     * @return true if at least one is NAN.
     * @return false if both are not NAN.
     */
    constexpr bool m_nans(const RationalTemplated& other) const
    {
        return is_nan() || other.is_nan();
    }
};

/**
 * @brief Sum operator.
 *
 * @remark Returning as const to avoid assigning value to the intermidate
 * result, like (a + b = c). Const will not make the assigned value as const be
 * default.
 *
 * @tparam T Underlying type.
 * @param[in] lhs Left-hand-side operand.
 * @param[in] rhs Rgiht-hand-side operand.
 * @return const RationalTemplated<T> Rational number reprsenting the sum of the
 * two operands.
 */
template <typename T>
const RationalTemplated<T> operator+(
    const RationalTemplated<T>& lhs, const RationalTemplated<T>& rhs)
{
    return RationalTemplated {RationalTemplated {lhs} += rhs};
}

/**
 * @brief Subtraction operator.
 *
 * @remark Returning as const to avoid assigning value to the intermidate
 * result, like (a - b = c). Const will not make the assigned value as const be
 * default.
 *
 * @tparam T Underlying type.
 * @param[in] lhs Left-hand-side operand.
 * @param[in] rhs Rgiht-hand-side operand.
 * @return const RationalTemplated<T> Rational number reprsenting the difference
 * of the two operands.
 */
template <typename T>
const RationalTemplated<T> operator-(
    const RationalTemplated<T>& lhs, const RationalTemplated<T>& rhs)
{
    return RationalTemplated {RationalTemplated {lhs} -= rhs};
}

/**
 * @brief Multiplication operator.
 *
 * @remark Returning as const to avoid assigning value to the intermidate
 * result, like (a * b = c). Const will not make the assigned value as const be
 * default.
 *
 * @tparam T Underlying type.
 * @param[in] lhs Left-hand-side operand.
 * @param[in] rhs Rgiht-hand-side operand.
 * @return const RationalTemplated<T> Rational number reprsenting the product of
 * the two operands.
 */
template <typename T>
constexpr const RationalTemplated<T> operator*(
    const RationalTemplated<T>& lhs, const RationalTemplated<T>& rhs)
{
    return RationalTemplated {RationalTemplated {lhs} *= rhs};
}

/**
 * @brief Division operator.
 *
 * @remark Returning as const to avoid assigning value to the intermidate
 * result, like (a / b = c). Const will not make the assigned value as const be
 * default.
 *
 * @tparam T Underlying type.
 * @param[in] lhs Left-hand-side operand.
 * @param[in] rhs Rgiht-hand-side operand.
 * @return const RationalTemplated<T> Rational number reprsenting the fraction
 * of the two operands.
 */
template <typename T>
const RationalTemplated<T> operator/(
    const RationalTemplated<T>& lhs, const RationalTemplated<T>& rhs)
{
    return RationalTemplated {RationalTemplated {lhs} /= rhs};
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
inline constexpr const RationalTemplated<T> reciprocal(
    const RationalTemplated<T>& number)
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

using Rational = RationalTemplated<int>;

} // namespace Marvin

#endif // H_NUMBERS_INCLUDE_RATIONAL_H_H
