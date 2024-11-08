#include "Input.h"
#include "Random.h"

#include <iostream>

class Potion
{
  public:
    enum Type
    {
        health,
        stength,
        poision,
        max_type,
    };

    enum Size
    {
        small,
        medium,
        large,
        max_size,
    };

    std::array<std::string_view, max_type> type_names {
        "Health", "Stength", "Poision"};

    std::array<std::string_view, max_size> size_names {
        "Small", "Medium", "Large"};

    Potion(Type type, Size size) : m_type {type}, m_size {size} {}

    Type type() const
    {
        return m_type;
    }

    Size size() const
    {
        return m_size;
    }

    std::string name() const
    {
        return std::string {size_names[m_size]} + " potion of "
               + std::string {type_names[m_type]};
    }

    static Potion get_random_potion()
    {
        return Potion {static_cast<Type>(Random::get(0, max_type - 1)),
            static_cast<Size>(Random::get(0, max_size - 1))};
    }

  private:
    Type m_type;
    Size m_size;
};


class Creature
{
  public:
    Creature(std::string_view name, char symbol, int hp, int dpa, int gold)
        : m_name {name}, m_symbol {symbol}, m_hp {hp}, m_dpa {dpa},
          m_gold {gold}
    {
    }

    std::string_view name() const
    {
        return m_name;
    }

    char symbol() const
    {
        return m_symbol;
    }

    int hp() const
    {
        return m_hp;
    }

    int dpa() const
    {
        return m_dpa;
    }

    int gold() const
    {
        return m_gold;
    }

    void reduce_health(int hp)
    {
        m_hp -= hp;
    }

    bool is_dead() const
    {
        return m_hp <= 0;
    }

    void add_gold(int gold)
    {
        m_gold += gold;
    }

  protected:
    std::string_view m_name;
    char m_symbol;
    int m_hp;
    int m_dpa;
    int m_gold;
};

class Player : public Creature
{
  public:
    explicit Player(std::string_view name) : Creature(name, '@', 10, 1, 0) {}

    int level() const
    {
        return m_level;
    }

    void level_up()
    {
        ++m_level;
        ++m_dpa;
    }

    bool has_won() const
    {
        return m_level >= 20;
    }

    void drink_potion(const Potion& potion)
    {
        switch (potion.type())
        {
            case Potion::health:
            {
                switch (potion.size())
                {
                    case Potion::small:
                        m_hp += 2;
                        break;
                    case Potion::medium:
                        m_hp += 5;
                        break;
                    case Potion::large:
                        m_hp += 10;
                        break;
                    default:
                        break;
                }
            }
            break;

            case Potion::stength:
            {
                switch (potion.size())
                {
                    case Potion::small:
                        m_dpa += 1;
                        break;
                    case Potion::medium:
                        m_dpa += 2;
                        break;
                    case Potion::large:
                        m_dpa += 3;
                        break;
                    default:
                        break;
                }
            }
            break;

            case Potion::poision:
            {
                switch (potion.size())
                {
                    case Potion::small:
                        m_hp -= 1;
                        break;
                    case Potion::medium:
                        m_hp -= 2;
                        break;
                    case Potion::large:
                        m_hp -= 3;
                        break;
                    default:
                        break;
                }
            }
            break;

            default:
                break;
        }
    }

  private:
    int m_level {1};
};

class Monster : public Creature
{
  private:
    static inline Creature m_monster_data[] {{"dragon", 'D', 20, 4, 100},
        {"orc", 'o', 4, 2, 25}, {"slime", 's', 1, 1, 10}};

  public:
    enum Type
    {
        dragon,
        orc,
        slime,
        max_types,
    };
    static_assert(std::size(m_monster_data) == max_types);

    Monster(Type type) : Creature {m_monster_data[type]} {}

    static Monster get_random_monster()
    {
        return Monster {static_cast<Type>(Random::get(0, max_types - 1))};
    }
};

static void attack_monster(Player& player, Monster& monster)
{
    std::cout << "You hit the " << monster.name() << " for " << player.dpa()
              << " damange.\n";

    monster.reduce_health(player.dpa());
    if (monster.is_dead())
    {
        player.level_up();
        player.add_gold(monster.gold());
        std::cout << "You killed the " << monster.name()
                  << ".\nYou are now level " << player.level()
                  << ".\nYou found " << monster.gold() << " gold.\n";

        if (Random::get(1, 10) < 4)
        {
            if (Input::get_input<char>(
                    std::span<const char>(std::array<char, 2>({'y', 'n'})),
                    "You found a mythical potion! Do you want to drink it? "
                    "[y/n]: ",
                    "Invalid input, try again.")
                == 'y')
            {
                auto potion {Potion::get_random_potion()};
                std::cout << "You drank a " << potion.name() << "\n";
                player.drink_potion(potion);
            }
        }
    }
}

static void attack_player(Monster& monster, Player& player)
{
    if (monster.is_dead())
    {
        return;
    }

    std::cout << "The " << monster.name() << " hit you for " << monster.dpa()
              << " damange.\n";

    player.reduce_health(monster.dpa());
    if (!player.is_dead())
    {
        std::cout << "Your health is " << player.hp() << ".\n";
    }
}

static void fight_monster(Player& player)
{
    Monster monster {Monster::get_random_monster()};
    std::cout << "You have encountered a vile " << monster.name() << " ("
              << monster.symbol() << ").\n";

    constexpr char valid_inputs[] {'r', 'f', 'R', 'F'};
    while (!monster.is_dead() && !player.is_dead())
    {
        char c {Input::get_input<char>(std::span<const char>(valid_inputs),
            "(R)un or (F)ight: ", "Invalid input, try again.")};
        if ((c == 'f') || (c == 'F'))
        {
            attack_monster(player, monster);
        }
        else
        {
            if (Random::get(0, 1) == 1)
            {
                std::cout << "You successfully fled.\n";
                return;
            }
            else
            {
                std::cout << "You failed to flee.\n";
            }
        }

        attack_player(monster, player);
    }
}

int main()
{
    Player player {Input::get_input<std::string>("Enter your name: ")};
    std::cout << "Welcome, " << player.name() << ".\nYou have " << player.hp()
              << " health and are carrying " << player.gold() << " gold\n";

    while (!player.is_dead() && !player.has_won())
    {
        fight_monster(player);
    }

    if (player.has_won())
    {
        std::cout << "You won with " << player.gold()
                  << " gold in your pockets!\n ";
    }
    else
    {
        std::cout << "You died at level " << player.level() << " and with "
                  << player.gold()
                  << " gold. Don't waste it all... on the other side.\n";
    }

    return 0;
}
