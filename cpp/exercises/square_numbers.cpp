#include "Random.h"

#include <iostream>

template <typename T> static T get_input(std::string_view message = "")
{
    constexpr std::string_view invalid_input_message {
        "That wasn't a valid input. Try again.\n"};

    while (true)
    {
        std::cout << message;

        T value {};
        std::cin >> value;

        auto clear_input_lambda {[]() {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }};

        if (!std::cin)
        {
            std::cin.clear();
            std::cerr << invalid_input_message;
            clear_input_lambda();
            continue;
        }

        // Clear any extraneous input.
        clear_input_lambda();

        return value;
    }
}

int main()
{
    int start {get_input<int>("Start where? ")};
    const int count {get_input<int>("How many? ")};
    const int multiplier {Random::get(2, 4)};

    std::vector<int> numbers(count);
    for (int i {0}; i < count; ++i, ++start)
    {
        numbers[i] = start * start * multiplier;
    }

    std::cout << "I generated " << count
              << " square numbers. Do you know what each number is after "
                 "multiplying it by "
              << multiplier << "?\n";

    for (int i {count - 1}; i >= 0; --i)
    {
        const int guess {get_input<int>("> ")};
        const auto found {std::ranges::find(numbers, guess)};
        if (found != numbers.cend())
        {
            numbers.erase(found);
            std::cout << "Nice! ";
            if (i)
            {
                std::cout << i << " numbers left.\n";
            }
            else
            {
                std::cout << "You found all numbers, good job!\n";
            }
        }
        else
        {
            const auto closest {std::min_element(numbers.cbegin(),
                numbers.cend(), [guess](const auto a, const auto b)
                { return std::abs(a - guess) < std::abs(b - guess); })};
            std::cout << guess << " is wrong! Try " << *closest
                      << " next time.\n";
            break;
        }
    }

    std::cout << "\n";

    return 0;
}
