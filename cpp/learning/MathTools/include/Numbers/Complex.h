#ifndef H_INCLUDE_NUMBERS_COMPLEX_H_H
#define H_INCLUDE_NUMBERS_COMPLEX_H_H

namespace Marvin
{

/**
 * @brief Class to represent complex number.
 * @remarks Requiring integral or floating point types as the inner type. The
 * integral types are allowed for testing purposes, if only integer arithmetics
 * is assumed.
 *
 * @tparam T type parameter to have complex numbers with specified inner type.
 */
template <typename T>
    requires std::integral<T> || std::floating_point<T>
class ComplexTemplated
{
  public:
    using value_type = T;

    /***************************************************************************
     * Constructors section                                                    *
     **************************************************************************/

    /**
     * @brief Construct a complex number from two parts: real and imaginary. If
     * only the real part is provided, then the imaginary part is set to zero.
     *
     * @param[in] r Real part.
     * @param[in] i Imaginary part (default = zero).
     */
    explicit ComplexTemplated(value_type r = 0, value_type i = 0)
        : m_real {r}, m_imag {i}
    {
        m_print_created("constructor");
    }

    /**
     * @brief Construct a complex number from another number: basically, copying
     * real and imaginary parts.
     *
     * @param[in] other The number to copy from.
     */
    explicit ComplexTemplated(const ComplexTemplated& other)
        : m_real {other.real()}, m_imag {other.imag()}
    {
        m_print_created("copy constructor");
    }

    /**
     * @brief Copy assignment operator: copying real and imaginary parts from
     * the other number.
     *
     * @param[in] other Number to copy from.
     * @return ComplexTemplated& Reference to the current object.
     */
    ComplexTemplated& operator=(const ComplexTemplated& other)
    {
        if (this != &other)
        {
            m_real = other.real();
            m_imag = other.imag();
        }

        m_print_created("copy assignment");
        return *this;
    }

    /**
     * @brief Move constructor: moving real and imaginary parts.
     *
     * @param[in] other The number to move.
     */
    ComplexTemplated(ComplexTemplated&& other) noexcept
        : m_real {other.real()}, m_imag {other.imag()}
    {
        m_print_created("move constructor");
    }

    /**
     * @brief Move assignment operator: moving real and imaginary parts from
     * the other number.
     *
     * @param[in] other Number to move.
     * @return ComplexTemplated& Reference to the current object.
     */
    ComplexTemplated& operator=(const ComplexTemplated&& other) noexcept
    {
        if (this != &other)
        {
            m_real = other.real();
            m_imag = other.imag();
        }

        m_print_created("move assignment");
        return *this;
    }

    /***************************************************************************
     * End of constructors section                                             *
     **************************************************************************/

    /***************************************************************************
     * Getters section                                                         *
     **************************************************************************/

    value_type real() const noexcept
    {
        return m_real;
    }

    value_type imag() const noexcept
    {
        return m_imag;
    }

    /***************************************************************************
     * End of Getters section                                                  *
     **************************************************************************/

  private:
    value_type m_real;
    value_type m_imag;

    void m_print_created(std::string_view name)
    {
        std::cout << "Complex: called " << name << " for " << *this << "\n";
    }
};

template <typename T>
std::ostream& operator<<(
    std::ostream& stream, const ComplexTemplated<T>& complex)
{
    stream << '(' << complex.real() << ", " << complex.imag() << ')';
    return stream;
}

// Just using ComplexTemplated with type specified.
using Complex = ComplexTemplated<double>;

} // namespace Marvin

#endif // H_INCLUDE_NUMBERS_COMPLEX_H_H
