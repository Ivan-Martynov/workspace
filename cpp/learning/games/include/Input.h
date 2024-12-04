#ifndef H_INPUT_H_H
#define H_INPUT_H_H

#include <iostream>
#include <limits>
#include <span>
#include <algorithm>

/**
 * @brief Namespace containing helper functions to get input from a user.
 *
 */
namespace Input
{

/**
 * @brief Clear the stream from extra input.
 *
 * @param[in] stream Stream to clear.
 */
void ignore_extra(std::istream& stream)
{
    stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**
 * @brief Clear std::cin from extra input.
 */
void ignore_extra()
{
    ignore_extra(std::cin);
}

/**
 * @brief Check if input stream has extra input.
 *
 * @param[in] stream Stream to inspect.
 * @return true There are characters left.
 * @return false No extra characters.
 */
bool has_unextracted_input(std::istream& stream)
{
    return (!stream.eof()) && (stream.peek() != '\n');
}

/**
 * @brief Check if std::cin has extra characters.
 *
 * @return true There are characters left.
 * @return false No extra characters.
 */
bool has_unextracted_input()
{
    return has_unextracted_input(std::cin);
}

/**
 * @brief Get the user input from a stream. Clears the stream from extra
 * characters.
 *
 * @tparam T Type of object/variable to return.
 * @param[in] stream Stream to read from.
 * @param[in] message Message to describe what is to be done, for example,
 * "Enter a number: ".
 * @param[in] error_message Message to show in case of error, for example,
 * "Invalid input."
 * @return T Object/variable retrived.
 */
template <typename T>
T get_input(std::istream& stream, std::string_view message = "",
    std::string_view error_message = "")
{
    while (true)
    {
        std::cout << message;

        T value {};
        if (stream >> value; stream.fail()) // Previous extraction failed.
        {
            stream.clear(); // Put back to normal operation mode.
            std::cerr << error_message;
            ignore_extra(stream);
            continue;
        }

        if (has_unextracted_input(stream))
        {
            // Clear any extraneous input.
            ignore_extra(stream);
        }

        return value;
    }
}

/**
 * @brief Get the user input from console. Clears the stream from extra
 * characters.
 *
 * @tparam T Type of object/variable to return.
 * @param[in] message Message to describe what is to be done, for example,
 * "Enter a number: ".
 * @param[in] error_message Message to show in case of error, for example,
 * "Invalid input."
 * @return T Object/variable retrived.
 */
template <typename T>
T get_input(std::string_view message = "", std::string_view error_message = "")
{
    return get_input<T>(std::cin, message, error_message);
}

/**
 * @brief Get the user input from a stream. Clears the stream from extra
 * characters.
 *
 * @tparam T Type of object/variable to return.
 * @param[in] stream Stream to read from.
 * @param[in] valid_values List of acceptable values. If none is entered, the
 * user is asked to enter a value again.
 * @param[in] message Message to describe what is to be done, for example,
 * "Enter a number: ".
 * @param[in] error_message Message to show in case of error, for example,
 * "Invalid input."
 * @return T Object/variable retrived.
 */
template <typename T>
T get_input(std::istream& stream, std::span<const T> valid_values,
    std::string_view message = "", std::string_view error_message = "")
{
    while (true)
    {
        const T value {get_input<T>(stream, message, error_message)};
        for (const auto& v : valid_values)
        {
            if (value == v)
            {
                return value;
            }
        }
    }
}

/**
 * @brief Get the user input from console. Clears the stream from extra
 * characters.
 *
 * @tparam T Type of object/variable to return.
 * @param[in] valid_values List of acceptable values. If none is entered, the
 * user is asked to enter a value again.
 * @param[in] message Message to describe what is to be done, for example,
 * "Enter a number: ".
 * @param[in] error_message Message to show in case of error, for example,
 * "Invalid input."
 * @return T Object/variable retrived.
 */
template <typename T>
T get_input(std::span<const T> valid_values, std::string_view message = "",
    std::string_view error_message = "")
{
    return get_input<T>(std::cin, valid_values, message, error_message);
}

} // namespace Input

#endif // H_INPUT_H_H
