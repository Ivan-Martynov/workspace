#ifndef H_NUMBERS_INCLUDE_RATIONAL_H_H
#define H_NUMBERS_INCLUDE_RATIONAL_H_H

#include <cstdint> // For std::intmax_t
#include <numeric>

namespace Marvin
{

/**
 * @brief Simple class representing rational numbers with numerator and
 * denominator as integers.
 *
 */
class Rational
{
  public:
    // Specify type for numerator and denominator.
    using value_type = std::intmax_t;

  private:
    value_type m_num;
    value_type m_den;

    constexpr explicit Rational(
        const value_type num, const value_type den, const value_type div)
        : m_num {num / div}, m_den {den / div}
    {
    }

  public:
    constexpr explicit Rational(
        const value_type num = 0, const value_type den = 1)
        : Rational(num, den, std::gcd(num, den))
    {
    }

    constexpr value_type numerator() const
    {
        return m_num;
    }

    constexpr value_type denominator() const
    {
        return m_den;
    }
};


} // namespace Marvin

#endif // H_NUMBERS_INCLUDE_RATIONAL_H_H
