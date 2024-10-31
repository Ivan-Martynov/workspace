#include "Random.h"

#include <iostream>
#include <vector>
#include <array>

namespace Potion
{

enum Type
{
    healing,
    mana,
    speed,
    invisibility,
    max_potions,
};

constexpr std::array<Type, max_potions> types {
    healing, mana, speed, invisibility};
constexpr std::array prices {20, 30, 12, 50};
constexpr std::array<std::string_view, max_potions> names {
    "healing", "mana", "speed", "invisibility"};

static_assert(
    (std::size(prices) == max_potions) && (std::size(names) == max_potions));

} // namespace Potion

class Player
{
  private:
    static constexpr int s_min_gold {80};
    static constexpr int s_max_gold {120};

    std::string m_name;
    std::array<int, Potion::max_potions> m_inventory {};
    int m_gold;

  public:
    Player(const std::string_view name)
        : m_name {name}, m_gold {Random::get(s_min_gold, s_max_gold)}
    {
    }

    std::string_view name() const
    {
        return m_name;
    }

    int gold() const
    {
        return m_gold;
    }

    bool buy(Potion::Type type)
    {
        std::cout << m_gold << " vs " << Potion::prices[type] << "\n";
        if (m_gold < Potion::prices[type])
        {
            return false;
        }
        else
        {
            m_gold -= Potion::prices[type];
            ++m_inventory[type];
            return true;
        }
    }

    int inventory(Potion::Type p) const
    {
        return m_inventory[p];
    }
};

static Potion::Type get_input_selection()
{
    constexpr std::string_view invalid_input_message {
        "That wasn't a valid input. Try again.\n"};

    while (true)
    {
        std::cout << "Enter the number of the potion you'd like to buy or 'q' "
                     "to quit: ";

        char letter {};
        std::cin >> letter;

        auto ignore_line_lambda {[]() {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }};

        if (!std::cin)
        {
            std::cin.clear();
            ignore_line_lambda();
            continue;
        }

        // If extraneous input treat as failure.
        if (!std::cin.eof() && (std::cin.peek() != '\n'))
        {
            std::cerr << invalid_input_message;
            ignore_line_lambda();
            continue;
        }

        if (std::tolower(letter) == 'q')
        {
            return Potion::max_potions;
        }

        const int i {letter - '0'};
        if ((i >= 0) && (i < Potion::max_potions))
        {
            return static_cast<Potion::Type>(i);
        }
        else
        {
            std::cerr << invalid_input_message;
            ignore_line_lambda();
        }
    }
}

static void shop(Player& player)
{
    while (true)
    {
        std::cout << "\nHere is our selection for today:\n";

        for (const auto p : Potion::types)
        {
            std::cout << p << ") " << Potion::names[p] << " costs "
                      << Potion::prices[p] << "\n";
        }

        const Potion::Type potion {get_input_selection()};
        if (potion == Potion::max_potions)
        {
            break;
        }

        if (player.buy(potion))
        {
            std::cout << "You purchased a potion of " << Potion::names[potion]
                      << ". You have " << player.gold() << " left.\n";
        }
        else
        {
            std::cout << "You cannot afford that.\n";
        }
    }
}

static std::string get_input()
{
    while (true)
    {
        std::cout << "Enter your name: ";

        std::string name {};
        std::getline(std::cin >> std::ws, name);

        if (!std::cin)
        {
            std::cin.clear();
            std::cerr << "That wasn't a valid input. Try again.\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        return name;
    }
}

static void print_inventory(const Player& player)
{
    std::cout << "\nYour inventory contains: \n";
    for (auto p : Potion::types)
    {
        const auto n {player.inventory(p)};
        if (n > 0)
        {
            std::cout << n << " x potion of " << Potion::names[p] << "\n";
        }
    }
    std::cout << "You left with " << player.gold() << " gold remaining.\n";
}

int main()
{
    std::cout << "Welcome to Roscoe's potion emporium!\n";
    Player player {get_input()};
    std::cout << "Hello, " << player.name() << " you have " << player.gold()
              << " gold.\n";

    shop(player);

    print_inventory(player);

    std::cout << "\nThanks for shopping at Roscoe's potion emporium!\n";

    return 0;
}
