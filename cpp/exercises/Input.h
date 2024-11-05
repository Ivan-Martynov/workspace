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
static T get_input(
    std::string_view message = "", std::string_view error_message = "")
{
    while (true)
    {
        std::cout << message;

        T value {};
        if (std::cin >> value; !std::cin)
        {
            std::cin.clear();
            std::cerr << error_message;
            continue;
        }

        // Clear any extraneous input.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        return value;
    }
}

template <typename T>
static T get_input(std::span<const T> valid_values,
    std::string_view message = "", std::string_view error_message = "")
{
    while (true)
    {
        std::cout << message;

        T value {};
        if (std::cin >> value; !std::cin)
        {
            std::cin.clear();
            std::cerr << error_message;
            continue;
        }

        // Clear any extraneous input.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (std::any_of(valid_values.cbegin(), valid_values.cend(),
                [&value](const T& c) { return value == c; }))
        {
            return value;
        }
    }
}

} // namespace Input

#endif // H_INPUT_H_H
